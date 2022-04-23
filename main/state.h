#ifndef STATE_H
#define STATE_H

#include "scheduler.h"
#include <parson.h>

#define MAX_SCHEDULES_LEN 4

typedef struct state
{
    scheduler_t scheduler;
} state_t;

void state_init(state_t *state);

void state_schedules_set(state_t *state, JSON_Array *json_schedules);

#endif