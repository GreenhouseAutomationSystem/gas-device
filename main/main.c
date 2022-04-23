#include "device_client.h"
#include "time_utils.h"
#include "wifi.h"
#include <nvs_flash.h>
#include <stdio.h>
#include "controller.h"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ctrl_init();

    wifi_init();

    device_client_init();
}
