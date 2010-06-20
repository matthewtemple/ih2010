#ifndef ih_core_double_h
#define ih_core_double_h

#include "ih/core/object.h"

ih_core_bool_t ih_core_double_add_to_message(void *double_object,
    ih_core_message_t *message);

int ih_core_double_compare(void *double_object_a, void *double_object_b);

void *ih_core_double_copy(void *double_object);

void *ih_core_double_create_from_message(ih_core_message_t *message);

void ih_core_double_destroy(void *double_object);

#endif
