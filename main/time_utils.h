#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "esp_system.h"
#include <sntp.h>
#include <time.h>

#define SECS_IN_WEEK (86400 * 7)
#define SECS_IN_DAY 86400
#define SECS_IN_HOUR 3600
#define SECS_IN_MIN 60

void time_now(char *string, size_t max_size);

int time_now_to_sec();

int time_to_sec(struct tm *time);

void sec_to_time(int secs, struct tm *time);

#endif