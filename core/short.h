#ifndef ih_core_short_h
#define ih_core_short_h

#include "ih/core/object.h"
#include "ih/core/types.h"

ih_core_bool_t ih_core_short_add_to_message(void *short_object,
    ih_core_message_t *message);

int ih_core_short_compare(void *short_object_a, void *short_object_b);

void *ih_core_short_copy(void *short_object);

void *ih_core_short_take_from_message(ih_core_message_t *message);

void ih_core_short_destroy(void *short_object);

void ih_core_short_print(void *short_object);

#endif
