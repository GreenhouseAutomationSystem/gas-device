#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include "esp_system.h"
#include <sntp.h>
#include <time.h>

#define SECS_IN_WEEK (86400 * 7)
#define SECS_IN_DAY 86400
#define SECS_IN_HOUR 3600
#define SECS_IN_MIN 60

/**
 * @brief Set timezone. Initialize NTP
 *
 */
void time_init();

/**
 * @brief Get time now, stored to output string.
 *
 * @param string where will be time printed
 * @param max_size allocated size of output string
 */
void time_now(char *string, size_t max_size);

/**
 * @brief Get time now in seconds of week. Monday 00:00:00 is 0.
 *
 * @return int time in secs of week
 */
int time_now_to_sec();

/**
 * @brief Get time in seconds of week. Monday 00:00:00 is 0.
 *
 * @param time
 * @return int time in secs of week
 */
int time_to_sec(struct tm *time);

/**
 * @brief Get time from seconds of week
 *
 * @param secs input time
 * @param time output time
 */
void sec_to_time(int secs, struct tm *time);

#endif