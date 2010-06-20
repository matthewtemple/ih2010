#ifndef ih_mtetris_system_h
#define ih_mtetris_system_h

#include "ih/external/external.h"

struct ih_mtetris_system_t;
typedef struct ih_mtetris_system_t ih_mtetris_system_t;

ih_mtetris_system_t *ih_mtetris_system_create();

void ih_mtetris_system_destroy(ih_mtetris_system_t *system);

#endif
