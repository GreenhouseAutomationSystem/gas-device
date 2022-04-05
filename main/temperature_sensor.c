#include "temperature_sensor.h"

void temperature_sensor_init()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    for (size_t i = 0; i < TEMPERATURE_SENSOR_COUNT; i++)
    {
        adc1_config_channel_atten(TEMPERATURE_SENSOR_CHANNELS[i], ADC_ATTEN_DB_11);
    }
}

int temperature_get(temperature_sensor_t sensor)
{
    return adc1_get_raw(TEMPERATURE_SENSOR_CHANNELS[sensor]);
}