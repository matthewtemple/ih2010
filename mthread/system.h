#ifndef ih_mthread_system_h
#define ih_mthread_system_h

#include "ih/external/external.h"

struct ih_mthread_system_t;
typedef struct ih_mthread_system_t ih_mthread_system_t;

ih_mthread_system_t *ih_mthread_system_create();

void ih_mthread_system_destroy(ih_mthread_system_t *system);

#endif
