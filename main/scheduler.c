#include "scheduler.h"

static const char *TAG = "scheduler";

void schedule_task(void *_schedules)
{
    schedule_t *schedules = (schedule_t *)_schedules;

    int time = time_now_to_sec();

    // Find starting indexes
    int index[MAX_SCHEDULES];
    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        schedule_t *schedule = &schedules[s];
        ESP_LOGI(TAG, "max %d", schedule->period);
        ESP_LOGI(TAG, "count %d", schedule->intervals_count);
        ESP_LOGI(TAG, "pin %d", schedule->pin.number);
        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            ESP_LOGI(TAG, "start %d", schedule->intervals[i].start);
            ESP_LOGI(TAG, "end %d", schedule->intervals[i].end);
        }

        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            // TODO: check if we can not miss any time record coz of < <=
            if (time % schedule->period < schedule->intervals[i].end)
            {
                index[s] = i;
                break;
            }
        }

        if (index[s] >= schedule->intervals_count)
        {
            index[s] = 0;
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

            bool level = schedule->pin.level;
            bool change = level ? schedule->intervals[index[s]].end <= (time % schedule->period)
                                : schedule->intervals[index[s]].start <= (time % schedule->period) &&
                                      (time % schedule->period) < schedule->intervals[index[s]].end;

            if (change)
            {
                pin_set(&schedule->pin, !level);
            }

            index[s] += change && level;

            if (index[s] >= schedule->intervals_count)
            {
                index[s] = 0;
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
