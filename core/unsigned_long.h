#ifndef ih_core_unsigned_long_h
#define ih_core_unsigned_long_h

#include "ih/core/objectey.h"

int ih_core_unsigned_long_compare(void *unsigned_long_object_a,
    void *unsigned_long_object_b);

void *ih_core_unsigned_long_copy(void *unsigned_long_object);

void ih_core_unsigned_long_destroy(void *unsigned_long_object);

char *ih_core_unsigned_long_get_as_string(void *unsigned_long_object);

void ih_core_unsigned_long_init_objectey(ih_core_objectey_t *objectey);

#endif
