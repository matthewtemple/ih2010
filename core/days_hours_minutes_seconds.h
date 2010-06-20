#ifndef ih_core_days_hours_minutes_seconds_h
#define ih_core_days_hours_minutes_seconds_h

struct ih_core_days_hours_minutes_seconds_t {
  unsigned short days;
  unsigned short hours;
  unsigned short minutes;
  unsigned short seconds;
};
typedef struct ih_core_days_hours_minutes_seconds_t
ih_core_days_hours_minutes_seconds_t;

char *create_string_from_days_hours_minutes_seconds
(ih_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds);

#endif
