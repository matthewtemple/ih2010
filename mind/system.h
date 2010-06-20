#ifndef ih_mind_system_h
#define ih_mind_system_h

#include "ih/external/external.h"

struct ih_mind_system_t;
typedef struct ih_mind_system_t ih_mind_system_t;

ih_mind_system_t *ih_mind_system_create();

void ih_mind_system_destroy(ih_mind_system_t *system);

#endif
