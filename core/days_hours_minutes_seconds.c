#include "ih/core/days_hours_minutes_seconds.h"
#include "ih/core/tools.h"
#include "ih/core/types.h"
#include "ih/external/external.h"

char *create_string_from_days_hours_minutes_seconds
(ih_core_days_hours_minutes_seconds_t *days_hours_minutes_seconds)
{
  assert(days_hours_minutes_seconds);
  char *string;
  int asprintf_result;

  asprintf_result = asprintf(&string, "%02i:%02i:%02i:%02i",
      days_hours_minutes_seconds->days, days_hours_minutes_seconds->hours,
      days_hours_minutes_seconds->minutes,
      days_hours_minutes_seconds->seconds);
  if (-1 == asprintf_result) {
    string = NULL;
    ih_core_trace("asprintf");
  }

  return string;
}
