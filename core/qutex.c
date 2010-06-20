#include "ih/core/mutex.h"
#include "ih/core/qutex.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_core_qutex_t {
  unsigned long shared_user_count;
  ih_core_bool_t exclusive_user;
  ih_core_mutex_t *mutex;
};

ih_core_qutex_t *ih_core_qutex_create()
{
  ih_core_qutex_t *qutex;

  qutex = malloc(sizeof *qutex);
  if (qutex) {
    qutex->shared_user_count = 0;
    qutex->exclusive_user = ih_core_bool_false;
    qutex->mutex = ih_core_mutex_create();
    if (!qutex->mutex) {
      ih_core_trace("ih_core_mutex_create");
      free(qutex);
      qutex = NULL;
    }
  } else {
    ih_core_trace("malloc() failed");
  }

  return qutex;
}

void ih_core_qutex_destroy(ih_core_qutex_t *qutex)
{
  assert(qutex);
  ih_core_mutex_destroy(qutex->mutex);
  free(qutex);
}

void ih_core_qutex_lock_exclusive(ih_core_qutex_t *qutex)
{
  assert(qutex);
  while (!ih_core_qutex_trylock_exclusive(qutex)) {}
}

void ih_core_qutex_lock_shared(ih_core_qutex_t *qutex)
{
  assert(qutex);
  while (!ih_core_qutex_trylock_shared(qutex)) {}
}

ih_core_bool_t ih_core_qutex_trylock_exclusive(ih_core_qutex_t *qutex)
{
  assert(qutex);
  ih_core_bool_t locked;

  ih_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user && (0 == qutex->shared_user_count)) {
      qutex->exclusive_user = ih_core_bool_true;
      locked = ih_core_bool_true;
    } else {
      locked = ih_core_bool_false;
    }
  }
  ih_core_mutex_unlock(qutex->mutex);

  return locked;
}

ih_core_bool_t ih_core_qutex_trylock_shared(ih_core_qutex_t *qutex)
{
  assert(qutex);
  ih_core_bool_t locked;

  ih_core_mutex_lock(qutex->mutex);
  {
    if (!qutex->exclusive_user) {
      qutex->shared_user_count++;
      locked = ih_core_bool_true;
    } else {
      locked = ih_core_bool_false;
    }
  }
  ih_core_mutex_unlock(qutex->mutex);

  return locked;
}

void ih_core_qutex_unlock_exclusive(ih_core_qutex_t *qutex)
{
  ih_core_mutex_lock(qutex->mutex);
  qutex->exclusive_user = ih_core_bool_false;
  ih_core_mutex_unlock(qutex->mutex);
}

void ih_core_qutex_unlock_shared(ih_core_qutex_t *qutex)
{
  ih_core_mutex_lock(qutex->mutex);
  qutex->shared_user_count--;
  ih_core_mutex_unlock(qutex->mutex);
}
