#ifndef ih_core_unsigned_short_h
#define ih_core_unsigned_short_h

#include "ih/core/object.h"
#include "ih/core/types.h"

int ih_core_unsigned_short_compare(void *unsigned_short_object_a,
    void *unsigned_short_object_b);

void *ih_core_unsigned_short_copy(void *unsigned_short_object);

void ih_core_unsigned_short_destroy(void *unsigned_short_object);

void ih_core_unsigned_short_print(void *unsigned_short_object);

#endif
