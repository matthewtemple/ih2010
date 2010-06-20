#ifndef ih_core_int_h
#define ih_core_int_h

#include "ih/core/object.h"
#include "ih/core/objectey.h"

struct ih_core_int_t;
typedef struct ih_core_int_t ih_core_int_t;

int ih_core_int_compare(void *int_object_a, void *int_object_b);

void *ih_core_int_copy(void *int_object);

ih_core_int_t *ih_core_int_create();

void ih_core_int_destroy(void *int_object);

char *ih_core_int_get_as_string(void *int_object);

void ih_core_int_init_objectey(ih_core_objectey_t *objectey);

#endif
