#include "device_twin.h"

static const char *TAG = "device_twin";

static const char *connectionString = CONFIG_IOTHUB_CONNECTION_STRING;

static const State state;

IOTHUB_DEVICE_CLIENT_LL_HANDLE iotHubClientHandle;

char *to_json(State *state)
{
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    (void)json_object_set_string(root_object, "status", state->status);

    char *result = json_serialize_to_string(root_value);
    json_value_free(root_value);

    return result;
}

void from_json(const char *json, DEVICE_TWIN_UPDATE_STATE update_state, State *out)
{
    JSON_Value *root_value = NULL;
    JSON_Object *root_object = NULL;

    root_value = json_parse_string(json);
    root_object = json_value_get_object(root_value);

    // Only desired properties:
    JSON_Value *status;

    if (update_state == DEVICE_TWIN_UPDATE_COMPLETE)
    {
        status = json_object_dotget_value(root_object, "desired.status");
    }
    else
    {
        status = json_object_get_value(root_object, "status");
    }

    if (status != NULL)
    {
        const char *status_string = json_value_get_string(status);

        if (status_string != NULL)
        {
            (void)strcpy(out->status, status_string);
        }
    }

    json_value_free(root_value);
}

void reportedStateCallback(int status_code, void *userContextCallback)
{
    (void)userContextCallback;
    printf("Device Twin reported properties update completed with result: %d\r\n", status_code);
}

void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char *payLoad, size_t size,
                        void *userContextCallback)
{
    (void)update_state;
    (void)size;

    State new_state;
    (void)memset(&new_state, 0, sizeof(State));
    from_json((const char *)payLoad, update_state, &new_state);

    State *state = (State *)userContextCallback;

    if (new_state.status != NULL)
    {
        if (strcmp(state->status, new_state.status) != 0)
        {
            printf("Received a new status = %s\n", new_state.status);
            (void)strcpy(state->status, new_state.status);
        }
    }

    char *reported_properties = to_json(state);
    (void)IoTHubDeviceClient_LL_SendReportedState(iotHubClientHandle, (const unsigned char *)reported_properties,
                                                  strlen(reported_properties), reportedStateCallback, NULL);
    free(reported_properties);
}

void device_twin_task(void *param)
{
    while (1)
    {
        IoTHubDeviceClient_LL_DoWork(iotHubClientHandle);
        ThreadAPI_Sleep(10);
    }
    vTaskDelete(NULL);
}

void device_twin_init()
{
    if (IoTHub_Init() != 0)
    {
        ESP_LOGE(TAG, "%s", "Failed to initialize the platform. Aborting...");
        abort();
    }

    iotHubClientHandle = IoTHubDeviceClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    if (iotHubClientHandle == NULL)
    {
        ESP_LOGE(TAG, "%s", "Failed to create IotHubClientHandle from connection string. Aborting...");
        abort();
    }

    // Verbose loging
    // bool traceOn = true;
    //(void)IoTHubDeviceClient_SetOption(iotHubClientHandle, OPTION_LOG_TRACE, &traceOn);

    memset(&state, 0, sizeof(State));

    IOTHUB_CLIENT_RESULT err =
        IoTHubDeviceClient_LL_SetDeviceTwinCallback(iotHubClientHandle, deviceTwinCallback, &state);

    if (err != IOTHUB_CLIENT_OK)
    {
        ESP_LOGE(TAG, "%s", "Failed to set DeviceTwin callback. Aborting...");
        abort();
    }

    if (xTaskCreate(&device_twin_task, "device_twin_task", 1024 * 6, NULL, 5, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "%s", "Failed to start client task, aborting...");
        abort();
    }
}
