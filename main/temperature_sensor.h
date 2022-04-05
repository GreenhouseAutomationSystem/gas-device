#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <driver/adc.h>

typedef enum
{
    TEMPERATURE_SENSOR_0,
    TEMPERATURE_SENSOR_1,
    TEMPERATURE_SENSOR_COUNT,
} temperature_sensor_t;

static adc2_channel_t TEMPERATURE_SENSOR_CHANNELS[TEMPERATURE_SENSOR_COUNT] = {ADC1_CHANNEL_6, ADC1_CHANNEL_7};

void temperature_sensor_init();
int temperature_get(temperature_sensor_t sensor);

#endif