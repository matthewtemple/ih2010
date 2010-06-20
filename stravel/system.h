#ifndef ih_stravel_system_h
#define ih_stravel_system_h

#include "ih/external/external.h"

struct ih_stravel_system_t;
typedef struct ih_stravel_system_t ih_stravel_system_t;

ih_stravel_system_t *ih_stravel_system_create();

void ih_stravel_system_destroy(ih_stravel_system_t *system);

#endif
