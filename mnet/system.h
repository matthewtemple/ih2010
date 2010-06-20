#ifndef ih_mnet_system_h
#define ih_mnet_system_h

#include "ih/external/external.h"

struct ih_mnet_system_t;
typedef struct ih_mnet_system_t ih_mnet_system_t;

ih_mnet_system_t *ih_mnet_system_create();

void ih_mnet_system_destroy(ih_mnet_system_t *system);

#endif
