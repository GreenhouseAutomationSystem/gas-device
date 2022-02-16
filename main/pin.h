#ifndef PIN_H
#define PIN_H

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct pin
{
    bool level;
    gpio_num_t number;
} pin_t;

void pin_init(pin_t *pin, gpio_num_t gpio_num);

void pin_set(pin_t *pin, bool level);

#endif