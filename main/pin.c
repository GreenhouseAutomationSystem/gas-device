#include "pin.h"
#include "time_utils.h"
#include <esp_log.h>

static const char *TAG = "PIN";

#define PIN_DEFAULT_LEVEL false

void pin_init(pin_t *pin, gpio_num_t gpio_num)
{
    gpio_reset_pin(gpio_num);
    gpio_set_direction(gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num, PIN_DEFAULT_LEVEL);

    pin->number = gpio_num;
    pin->level = PIN_DEFAULT_LEVEL;
}

void pin_set(pin_t *pin, bool level)
{
    char time_buff[64];
    time_now(time_buff, 64);
    ESP_LOGI(TAG, "%s %s Pin number: %d", time_buff, level ? "Task on" : "Task off", pin->number);

    gpio_set_level(pin->number, level);
    pin->level = level;
}