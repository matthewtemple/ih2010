#include "ih/core/mutex.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_core_mutex_t {
  pthread_mutex_t mutex;
};

ih_core_mutex_t *ih_core_mutex_create()
{
  ih_core_mutex_t *mutex;

  mutex = malloc(sizeof *mutex);
  if (mutex) {
    if (0 != pthread_mutex_init(&mutex->mutex, NULL)) {
      ih_core_trace("pthread_mutex_init");
      free(mutex);
      mutex = NULL;
    }
  } else {
    ih_core_trace("malloc() failed");
  }

  return mutex;
}

void ih_core_mutex_destroy(ih_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_destroy(&mutex->mutex)) {
    ih_core_trace("pthread_mutex_destroy");
  }
  free(mutex);
}

void ih_core_mutex_lock(ih_core_mutex_t *mutex)
{
  assert(mutex);

  if (0 != pthread_mutex_lock(&mutex->mutex)) {
    ih_core_trace("pthread_mutex_lock");
  }
}

ih_core_bool_t ih_core_mutex_trylock(ih_core_mutex_t *mutex)
{
  assert(mutex);
  ih_core_bool_t success;

  if (0 == pthread_mutex_lock(&mutex->mutex)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

void ih_core_mutex_unlock(ih_core_mutex_t *mutex)
{
  if (0 != pthread_mutex_unlock(&mutex->mutex)) {
    ih_core_trace("pthread_mutex_unlock");
  }
}
