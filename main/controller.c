#include "controller.h"

static state_t state;

void ctrl_init()
{
    temperature_sensor_init();
    state_init(&state);
}

void ctrl_schedules_set(JSON_Array *schedules)
{
    scheduler_stop(&state.scheduler);
    state_schedules_set(&state, schedules);
    scheduler_start(&state.scheduler);
}
