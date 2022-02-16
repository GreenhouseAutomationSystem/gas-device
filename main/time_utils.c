#include "time_utils.h"

void time_now(char *string, size_t max_size)
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    strftime(string, max_size, "%c", &timeinfo);
}

int time_now_to_sec()
{
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    return time_to_sec(&timeinfo);
}

int time_to_sec(struct tm *time)
{
    return SECS_IN_DAY * ((time->tm_wday + 6) % 7) + SECS_IN_HOUR * time->tm_hour + SECS_IN_MIN * time->tm_min +
           time->tm_sec;
}

void sec_to_time(int secs, struct tm *time)
{
    time->tm_wday = secs / SECS_IN_DAY % 7;
    secs -= time->tm_wday * SECS_IN_DAY;
    time->tm_wday = (time->tm_wday + 1) % 7;

    time->tm_hour = secs / SECS_IN_HOUR;
    secs -= time->tm_hour * SECS_IN_HOUR;

    time->tm_min = secs / SECS_IN_MIN;
    secs -= time->tm_min * SECS_IN_MIN;

    time->tm_sec = secs;
}