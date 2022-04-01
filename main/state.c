#include "state.h"
#include "time_utils.h"
#include <esp_log.h>
#include <time.h>

void state_init(state_t *state)
{
    state->name = "GAS-0.1.0";
    state->status = "active";
    scheduler_init(&state->scheduler);
}

void state_schedules_set(state_t *state, JSON_Array *json_schedules)
{
    schedule_t *schedules = state->scheduler.schedules;

    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        schedule_t *schedule = &schedules[s];
        JSON_Object *json_schedule = json_array_get_object(json_schedules, s);

        pin_init(&schedule->pin, json_object_get_number(json_schedule, "pinNumber"));
        schedule->period = json_object_get_number(json_schedule, "period");

        JSON_Array *intervals = json_object_get_array(json_schedule, "intervals");
        schedule->intervals_count = json_array_get_count(intervals);

        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            interval_t *interval = &schedule->intervals[i];
            JSON_Object *json_interval = json_array_get_object(intervals, i);

            interval->start = json_object_get_number(json_interval, "start");
            interval->end = json_object_get_number(json_interval, "end");
        }
    }
}
