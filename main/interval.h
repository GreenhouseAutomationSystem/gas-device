#ifndef INTERVAL_H
#define INTERVAL_H

#include "sys/types.h"
#include "time_utils.h"
#include <parson.h>

#define INTERVAL_MAX (SECS_IN_WEEK - 1)

typedef struct interval
{
    int start;
    int end;
} interval_t;

void interval_init(interval_t *interval);

void intervals_init(interval_t intervals[], int intervals_length);

int interval_compare(const void *interval_a, const void *interval_b);

int interval_size(interval_t *interval);

void interval_zero_end(interval_t *in, interval_t *out);

void interval_start_max(interval_t *in, interval_t *out);

#endif