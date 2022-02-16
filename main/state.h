#ifndef STATE_H
#define STATE_H

#include "scheduler.h"
#include <parson.h>

#define MAX_SCHEDULES_LEN 8
const static int DEFAULT_SCHEDULE_PINS[MAX_SCHEDULES_LEN] = {15, 2, 0, 4, 16, 17, 5, 18};

typedef struct state
{
    char *name;
    char *status;
    scheduler_t scheduler;
} state_t;

void state_init(state_t *state);

void state_schedules_set(state_t *state, JSON_Array *json_schedules);

#endif