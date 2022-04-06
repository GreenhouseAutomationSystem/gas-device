#include "scheduler.h"

static const char *TAG = "scheduler";

void schedule_task(void *_schedules)
{
    schedule_t *schedules = (schedule_t *)_schedules;

    int time = time_now_to_sec();

    // Find starting indexes
    int index[MAX_SCHEDULES] = {0};
    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        schedule_t *schedule = &schedules[s];

        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            if (time % schedule->period < schedule->intervals[i].end)
            {
                index[s] = i;
                break;
            }
        }
    }

    TickType_t last_wake_time = xTaskGetTickCount();
    while (true)
    {
        for (size_t s = 0; s < MAX_SCHEDULES; s++)
        {
            schedule_t *schedule = &schedules[s];

            if (schedule->intervals_count == 0)
            {
                continue;
            }

            int idx = index[s];
            interval_t interval = schedule->intervals[idx];
            int local_time = time % schedule->period;

            if (interval.start <= local_time && local_time < interval.end)
            {
                if (!schedule->pin.level)
                {
                    pin_set(&schedule->pin, true);
                }
            }
            else if (schedule->pin.level)
            {
                idx = index[s] = (idx + 1) % schedule->intervals_count;
                if (interval.end != schedule->period || schedule->intervals[idx].start != 0)
                {
                    pin_set(&schedule->pin, false);
                }
            }
        }

        vTaskDelayUntil(&last_wake_time, 100 / portTICK_PERIOD_MS);
        time = time_now_to_sec();
    }
}

void scheduler_init(scheduler_t *scheduler)
{
    memset(scheduler, 0, sizeof(scheduler_t));
}

void scheduler_start(scheduler_t *scheduler)
{
    if (scheduler->task != NULL)
    {
        scheduler_stop(scheduler);
    }

    ESP_LOGI(TAG, "Creating task");
    // Stack size once hit 1756 B
    xTaskCreate(schedule_task, TAG, 2 * 1024, scheduler->schedules, configMAX_PRIORITIES - 1, &scheduler->task);
}

void scheduler_stop(scheduler_t *scheduler)
{
    if (scheduler->task == NULL)
    {
        return;
    }

    ESP_LOGI(TAG, "Deleting task");
    vTaskDelete(scheduler->task);
    scheduler->task = NULL;
}
