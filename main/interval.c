#include "interval.h"

void interval_init(interval_t *interval)
{
    interval->start = 0;
    interval->end = 0;
}

void intervals_init(interval_t intervals[], int intervals_length)
{
    for (size_t i = 0; i < intervals_length; i++)
    {
        interval_init(&intervals[i]);
    }
}

int interval_compare(const void *interval_a, const void *interval_b)
{
    return ((interval_t *)interval_a)->start - ((interval_t *)interval_b)->start;
}

int interval_size(interval_t *interval)
{
    return interval->end - interval->start;
}

void interval_zero_end(interval_t *in, interval_t *out)
{
    out->start = 0;
    out->end = in->end;
}

void interval_start_max(interval_t *in, interval_t *out)
{
    out->start = in->start;
    out->end = INTERVAL_MAX;
}