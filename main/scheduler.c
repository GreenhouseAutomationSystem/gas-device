#include "scheduler.h"

static const char *TAG = "scheduler";

void schedule_task(void *_schedules)
{
    schedule_t *schedules = (schedule_t *)_schedules;

    int time = time_now_to_sec();

    // Flatten schedules to array
    int time_axis_lengths[MAX_SCHEDULES] = {0};
    int time_axes[MAX_SCHEDULES][SCHEDULE_MAX_INTERVALS * 2] = {0};
    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        schedule_t *schedule = &schedules[s];
        time_axis_lengths[s] = schedule->intervals_count * 2;
        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            interval_t interval = schedule->intervals[i];

            time_axes[s][2 * i] = interval.start;
            time_axes[s][(2 * i) + 1] = interval.end;
        }
    }

    // Find starting indexes
    int index[MAX_SCHEDULES] = {0};
    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        schedule_t *schedule = &schedules[s];
        int *time_axis = time_axes[s];
        int time_axis_length = time_axis_lengths[s];

        ESP_LOGI(TAG, "max %d", schedule->period);
        ESP_LOGI(TAG, "count %d", schedule->intervals_count);
        ESP_LOGI(TAG, "pin %d", schedule->pin.number);
        for (size_t i = 0; i < schedule->intervals_count; i++)
        {
            ESP_LOGI(TAG, "start %d", schedule->intervals[i].start);
            ESP_LOGI(TAG, "end %d", schedule->intervals[i].end);
        }

        int schedule_time = time % schedule->period;
        for (size_t i = 0; i < time_axis_length; i++)
        {
            // TODO: check if we can not miss any time record coz of < <=
            if (schedule_time < time_axis[i])
            {
                index[s] = i;
                pin_set(&schedule->pin, i % 2);
                break;
            }
        }
    }

    for (size_t s = 0; s < MAX_SCHEDULES; s++)
    {
        ESP_LOGI(TAG, "axis lenght %d", time_axis_lengths[s]);
        for (size_t i = 0; i < time_axis_lengths[s]; i++)
        {
            ESP_LOGI(TAG, "key: %d, value: %d", i, time_axes[s][i]);
        }
    }

    TickType_t last_wake_time = xTaskGetTickCount();
    while (true)
    {
        for (size_t s = 0; s < MAX_SCHEDULES; s++)
        {
            schedule_t *schedule = &schedules[s];
            int *time_axis = time_axes[s];
            int time_axis_length = time_axis_lengths[s];

            if (time_axis_length <= 0)
            {
                continue;
            }

            int idx = index[s];
            int period = schedule->period;
            int schedule_time = time % period;

            if (time_axis[idx] <= schedule_time || (idx == time_axis_length - 1 && schedule_time < time_axis[idx - 1]))
            {
                pin_set(&schedule->pin, !schedule->pin.level);
                index[s] = (idx + 1) % time_axis_length;
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
    xTaskCreate(schedule_task, TAG, 8 * 1024, scheduler->schedules, configMAX_PRIORITIES - 1, &scheduler->task);
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
