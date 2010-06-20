#ifndef ih_container_array_h
#define ih_container_array_h

#include "ih/container/declarations.h"
#include "ih/core/bool.h"
#include "ih/core/message.h"
#include "ih/core/object.h"

void ih_container_array_add(ih_container_array_t *array, unsigned long index,
    void *object);

ih_core_bool_t ih_container_array_add_to_message(ih_container_array_t *array,
    ih_core_message_t *message,
    ih_core_message_add_to_message_f add_to_message);

void ih_container_array_clear(ih_container_array_t *array);

int ih_container_array_compare(void *array_object_a, void *array_object_b);

void *ih_container_array_copy(void *array_object);

unsigned long ih_container_array_count_non_null_objects
(ih_container_array_t *array);

ih_container_array_t *ih_container_array_create(unsigned long initial_size,
    ih_core_compare_f compare, ih_core_copy_f copy, ih_core_destroy_f destroy);

ih_container_array_t *ih_container_array_create_from_message
(ih_core_compare_f compare_object, ih_core_copy_f copy,
    ih_core_destroy_f destroy, ih_core_message_t *message,
    ih_core_message_create_from_message_f create_from_message);

ih_container_array_t *ih_container_array_create_strings_from_string
(char *string, char *separators);

void ih_container_array_destroy(void *array_object);

void ih_container_array_expunge_set(ih_container_array_t *array,
    ih_container_set_t *expunge_these);

void ih_container_array_expunge_set_inverse(ih_container_array_t *array,
    ih_container_set_t *expunge_all_but_these);

void *ih_container_array_find(ih_container_array_t *array,
    unsigned long index);

void *ih_container_array_find_random(ih_container_array_t *array);

void *ih_container_array_find_first(ih_container_array_t *array);

void *ih_container_array_find_last(ih_container_array_t *array);

void **ih_container_array_get_array(ih_container_array_t *array);

char *ih_container_array_get_as_delimited_string(ih_container_array_t *array,
    ih_core_get_as_string_f get_as_string, char *delimiter);

ih_core_compare_f ih_container_array_get_compare(ih_container_array_t *array);

ih_core_copy_f ih_container_array_get_copy(ih_container_array_t *array);

ih_core_destroy_f ih_container_array_get_destroy(ih_container_array_t *array);

unsigned long ih_container_array_get_size(ih_container_array_t *array);

void *ih_container_array_iterate_next(ih_container_array_t *array);

void ih_container_array_iterate_remove(ih_container_array_t *array);

void ih_container_array_iterate_start(ih_container_array_t *array);

void ih_container_array_print(ih_container_array_t *array);

void ih_container_array_remove(ih_container_array_t *array,
    unsigned long index);

void ih_container_array_replace(ih_container_array_t *array,
    unsigned long index, void *object);

void ih_container_array_replace_random(ih_container_array_t *array,
    void *object);

ih_core_bool_t ih_container_array_resize(ih_container_array_t *array,
    unsigned long new_size);

void ih_container_array_sort(ih_container_array_t *array);

#endif
