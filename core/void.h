#ifndef ih_net_void_h
#define ih_net_void_h

#include "ih/core/objectey.h"
#include "ih/external/external.h"

int ih_core_void_compare(void *object_a, void *object_b);

void *ih_core_void_copy(void *object);

void ih_core_void_destroy(void *object);

char *ih_core_void_get_as_string(void *object);

void ih_core_void_init_objectey(ih_core_objectey_t *objectey);

#endif
