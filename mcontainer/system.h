#ifndef ih_mcontainer_system_h
#define ih_mcontainer_system_h

#include "ih/external/external.h"

struct ih_mcontainer_system_t;
typedef struct ih_mcontainer_system_t ih_mcontainer_system_t;

ih_mcontainer_system_t *ih_mcontainer_system_create();

void ih_mcontainer_system_destroy(ih_mcontainer_system_t *system);

#endif
