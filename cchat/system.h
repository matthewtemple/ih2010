#ifndef ih_cchat_system_h
#define ih_cchat_system_h

#include "ih/external/external.h"

struct ih_cchat_system_t;
typedef struct ih_cchat_system_t ih_cchat_system_t;

ih_cchat_system_t *ih_cchat_system_create();

void ih_cchat_system_destroy(ih_cchat_system_t *system);

#endif
