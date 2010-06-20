#ifndef mutex_h
#define mutex_h

#include "ih/core/bool.h"
#include "ih/external/external.h"

struct ih_core_mutex_t;
typedef struct ih_core_mutex_t ih_core_mutex_t;

ih_core_mutex_t *ih_core_mutex_create();

void ih_core_mutex_destroy(ih_core_mutex_t *mutex);

void ih_core_mutex_lock(ih_core_mutex_t *mutex);

ih_core_bool_t ih_core_mutex_trylock(ih_core_mutex_t *mutex);

void ih_core_mutex_unlock(ih_core_mutex_t *mutex);

#endif
