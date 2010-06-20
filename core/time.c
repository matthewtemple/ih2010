#include "ih/core/constants.h"
#include "ih/core/time.h"
#include "ih/core/tools.h"
#include "ih/core/types.h"
#include "ih/external/external.h"

struct ih_core_time_t {
  unsigned long seconds;
};

ih_core_time_t *ih_core_time_create(unsigned long seconds)
{
  ih_core_time_t *time;

  time = malloc(sizeof *time);
  if (time) {
    time->seconds = seconds;
  } else {
    ih_core_trace("malloc");
  }

  return time;
}

void ih_core_time_destroy(ih_core_time_t *time)
{
  assert(time);
  free(time);
}

void ih_core_time_get_days_hours_minutes_seconds(ih_core_time_t *time,
    ih_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(time);
  assert(days_hours_minutes_seconds);
  unsigned long seconds_left;

  seconds_left = time->seconds;

  days_hours_minutes_seconds->days = seconds_left / IH_CORE_SECONDS_PER_DAY;
  seconds_left -= (days_hours_minutes_seconds->days
      * IH_CORE_SECONDS_PER_DAY);

  days_hours_minutes_seconds->hours = seconds_left / IH_CORE_SECONDS_PER_HOUR;
  seconds_left -= (days_hours_minutes_seconds->hours
      * IH_CORE_SECONDS_PER_HOUR);

  days_hours_minutes_seconds->minutes = seconds_left
    / IH_CORE_SECONDS_PER_MINUTE;
  seconds_left -= (days_hours_minutes_seconds->minutes
      * IH_CORE_SECONDS_PER_MINUTE);

  days_hours_minutes_seconds->seconds = seconds_left;

}
