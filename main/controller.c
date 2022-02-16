#include "controller.h"

static state_t state;

void ctrl_init()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);

    state_init(&state);
}

void ctrl_schedules_set(JSON_Array *schedules)
{
    scheduler_stop(&state.scheduler);
    state_schedules_set(&state, schedules);
    scheduler_start(&state.scheduler);
}
