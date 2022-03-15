#include "device_twin.h"

static const char *TAG = "device_twin";

static const char *connection_string = CONFIG_IOTHUB_CONNECTION_STRING;

IOTHUB_DEVICE_CLIENT_LL_HANDLE iothub_client_handle;

void reported_state_callback(int status_code, void *userContextCallback)
{
    (void)userContextCallback;
    printf("Device Twin reported properties update completed with result: %d\r\n", status_code);
}

void device_twin_callback(DEVICE_TWIN_UPDATE_STATE update_state, const unsigned char *payload, size_t size,
                          void *userContextCallback)
{
    JSON_Value *root_value = json_parse_string((const char *)payload);
    JSON_Object *root_object = json_value_get_object(root_value);
    if (update_state == DEVICE_TWIN_UPDATE_COMPLETE)
    {
        root_object = json_object_get_object(root_object, "desired");
    }

    char *str = json_serialize_to_string_pretty(root_value);
    printf("%s/n", str);
    json_free_serialized_string(str);

    JSON_Array *schedules = json_object_get_array(root_object, "schedules");
    if (schedules != NULL)
    {
        ESP_LOGI(TAG, "%s", "Setting schedules.");
        ctrl_schedules_set(schedules);
    }

    json_value_free(root_value);
}

void device_reporter_task(void *param)
{
    while (true)
    {

        JSON_Value *root_value = json_value_init_object();
        JSON_Object *root_object = json_value_get_object(root_value);

        wifi_ap_record_t ap_info;
        esp_wifi_sta_get_ap_info(&ap_info);

        json_object_set_number(root_object, "rssi", ap_info.rssi);

        char *str = json_serialize_to_string_pretty(root_value);
        printf("%s/n", str);
        json_free_serialized_string(str);

        char *message_string = json_serialize_to_string(root_value);
        IOTHUB_MESSAGE_HANDLE message = IoTHubMessage_CreateFromString(message_string);
        IoTHubDeviceClient_LL_SendEventAsync(iothub_client_handle, message, NULL, NULL);

        IoTHubMessage_Destroy(message);
        json_free_serialized_string(message_string);

        ThreadAPI_Sleep(30000);
    }
}

void device_twin_task(void *param)
{
    while (true)
    {
        IoTHubDeviceClient_LL_DoWork(iothub_client_handle);
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

    iothub_client_handle = IoTHubDeviceClient_LL_CreateFromConnectionString(connection_string, MQTT_Protocol);
    if (iothub_client_handle == NULL)
    {
        ESP_LOGE(TAG, "%s", "Failed to create iothub_client_handle from connection string. Aborting...");
        abort();
    }

    // Verbose loging
    // bool traceOn = true;
    //(void)IoTHubDeviceClient_SetOption(iothub_client_handle, OPTION_LOG_TRACE, &traceOn);

    IOTHUB_CLIENT_RESULT err =
        IoTHubDeviceClient_LL_SetDeviceTwinCallback(iothub_client_handle, device_twin_callback, NULL);

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

    if (xTaskCreate(&device_reporter_task, "device_reporter_task", 1024 * 2, NULL, 5, NULL) != pdPASS)
    {
        ESP_LOGE(TAG, "%s", "Failed to start reporter task, aborting...");
        abort();
    }
}
