#ifndef DEVICE_TWIN_H
#define DEVICE_TWIN_H

#include "esp_log.h"
#include "esp_system.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "azure_c_shared_utility/platform.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azure_macro_utils/macro_utils.h"
#include "controller.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iothub.h"
#include "iothub_client_options.h"
#include "iothub_device_client.h"
#include "iothub_message.h"
#include "iothubtransportmqtt.h"
#include "parson.h"
#include "sdkconfig.h"
#include <esp_wifi.h>

void device_twin_init();

typedef struct
{
    char status[32];
} State;

#endif