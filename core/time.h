#ifndef ih_core_time_h
#define ih_core_time_h

#include "ih/core/days_hours_minutes_seconds.h"

struct ih_core_time_t;
typedef struct ih_core_time_t ih_core_time_t;

ih_core_time_t *ih_core_time_create(unsigned long seconds);

void ih_core_time_destroy(ih_core_time_t *time);

void ih_core_time_get_days_hours_minutes_seconds(ih_core_time_t *time,
    ih_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
