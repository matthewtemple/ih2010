#ifndef ih_core_qutex_h
#define ih_core_qutex_h

#include "ih/core/bool.h"
#include "ih/core/mutex.h"
#include "ih/external/external.h"

struct ih_core_qutex_t;
typedef struct ih_core_qutex_t ih_core_qutex_t;

ih_core_qutex_t *ih_core_qutex_create();

void ih_core_qutex_destroy(ih_core_qutex_t *qutex);

void ih_core_qutex_lock_exclusive(ih_core_qutex_t *qutex);

void ih_core_qutex_lock_shared(ih_core_qutex_t *qutex);

ih_core_bool_t ih_core_qutex_trylock_exclusive(ih_core_qutex_t *qutex);

ih_core_bool_t ih_core_qutex_trylock_shared(ih_core_qutex_t *qutex);

void ih_core_qutex_unlock_exclusive(ih_core_qutex_t *qutex);

void ih_core_qutex_unlock_shared(ih_core_qutex_t *qutex);

#endif
