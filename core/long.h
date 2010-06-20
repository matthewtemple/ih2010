#ifndef ih_core_long_h
#define ih_core_long_h

#include "ih/core/object.h"
#include "ih/core/objectey.h"
#include "ih/core/types.h"

ih_core_bool_t ih_core_long_add_to_message(void *long_object,
    ih_core_message_t *message);

int ih_core_long_compare(void *long_object_a, void *long_object_b);

void *ih_core_long_copy(void *long_object);

void *ih_core_long_create_from_message(ih_core_message_t *message);

void ih_core_long_destroy(void *long_object);

char *ih_core_long_get_as_string(void *long_object);

void ih_core_long_init_objectey(ih_core_objectey_t *objectey);

void ih_core_long_print(void *long_object);

#endif
