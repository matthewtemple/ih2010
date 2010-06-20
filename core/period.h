#ifndef ih_core_period_h
#define ih_core_period_h

#include "ih/core/types.h"
#include "ih/external/external.h"

struct ih_core_period_t;
typedef struct ih_core_period_t ih_core_period_t;

ih_core_period_t *ih_core_period_create(unsigned long seconds);

void ih_core_period_destroy(ih_core_period_t *period);

ih_core_bool_t ih_core_period_once(ih_core_period_t *period);

#endif
