#ifndef ih_core_string_h
#define ih_core_string_h

#include "ih/core/object.h"
#include "ih/core/objectey.h"
#include "ih/core/types.h"

typedef char * ih_core_string_t;

ih_core_bool_t ih_core_string_add_to_message(void *string_object,
    ih_core_message_t *message);

int ih_core_string_compare(void *string_object_a, void *string_object_b);

void *ih_core_string_copy(void *string_object);

void *ih_core_string_create_from_message(ih_core_message_t *message);

void ih_core_string_destroy(void *string_object);

char *ih_core_string_get_as_string(void *string_object);

unsigned long ih_core_string_hash(void *string_object);

void ih_core_string_init_objectey(ih_core_objectey_t *objectey);

void ih_core_string_print(void *string_object);

ih_core_string_t ih_core_string_substring(ih_core_string_t string,
    unsigned long start, unsigned long length);

#endif
