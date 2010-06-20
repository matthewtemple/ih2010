#ifndef ih_core_object_h
#define ih_core_object_h

#include "ih/core/bool.h"

#define IH_CORE_NO_COMPARE_FUNCTION NULL
#define IH_CORE_NO_COPY_FUNCTION NULL
#define IH_CORE_NO_DESTROY_FUNCTION NULL

typedef int (*ih_core_compare_f)(void *a_object, void *b_object);

typedef int (*ih_core_compare_const_f)(const void *a_object,
    const void *b_object);

typedef ih_core_bool_t (*ih_core_condition_f)(void *object);

typedef void *(*ih_core_copy_f)(void *object);

typedef void *(*ih_core_create_f)();

typedef void (*ih_core_destroy_f)(void *object);

typedef char *(*ih_core_get_as_string_f)(void *object);

typedef void *(*ih_core_get_object_f)(void *object);

typedef unsigned long (*ih_core_hasih_f)(void *object);

void ih_core_print(void *object, ih_core_get_as_string_f get_as_string);

typedef void (*ih_core_set_object_f)(void *object, void *value);

#endif
