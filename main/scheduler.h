#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "interval.h"
#include "pin.h"
#include "sdkconfig.h"
#include "string.h"
#include "time_utils.h"
#include <esp_log.h>

#define SCHEDULE_MAX_INTERVALS CONFIG_SCHEDULER_MAX_RECORDS
#define MAX_SCHEDULES 8

typedef struct schedule
{
    pin_t pin;
    int period;
    int intervals_count;
    interval_t intervals[SCHEDULE_MAX_INTERVALS];
} schedule_t;

typedef struct scheduler
{
    TaskHandle_t task;
    schedule_t schedules[MAX_SCHEDULES];
} scheduler_t;

void scheduler_init(scheduler_t *scheduler);

void scheduler_start(scheduler_t *scheduler);

void scheduler_stop(scheduler_t *scheduler);

#endif