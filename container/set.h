#ifndef ih_container_set_h
#define ih_container_set_h

#include "ih/container/list.h"
#include "ih/core/declarations.h"
#include "ih/core/message.h"
#include "ih/core/object.h"
#include "ih/core/types.h"

ih_core_bool_t ih_container_set_absorb(ih_container_set_t *set,
    ih_container_set_t *absorb_these);

ih_core_bool_t ih_container_set_absorb_array(ih_container_set_t *set,
    ih_container_array_t *absorb_these);

ih_core_bool_t ih_container_set_absorb_copy(ih_container_set_t *set,
    ih_container_set_t *absorb_these);

ih_core_bool_t ih_container_set_absorb_list_copy(ih_container_set_t *set,
    ih_container_list_t *absorb_these);

ih_core_bool_t ih_container_set_add(ih_container_set_t *set, void *object);

ih_core_bool_t ih_container_set_add_replace(ih_container_set_t *set,
    void *object);

ih_core_bool_t ih_container_set_add_to_message(ih_container_set_t *set,
    ih_core_message_t *message, ih_core_message_add_to_message_f add_to_message);

void ih_container_set_clear(ih_container_set_t *set);

int ih_container_set_compare(void *set_object_a, void *set_object_b);

ih_core_bool_t ih_container_set_contains(ih_container_set_t *superset,
    ih_container_set_t *subset);

void *ih_container_set_copy(void *set_object);

ih_container_set_t *ih_container_set_create(ih_core_compare_f compare,
    ih_core_copy_f copy, ih_core_destroy_f destroy);

ih_container_set_t *ih_container_set_create_from_message
(ih_core_compare_f compare, ih_core_copy_f copy, ih_core_destroy_f destroy,
    ih_core_message_t *message,
    ih_core_message_create_from_message_f create_from_message);

ih_container_set_t *ih_container_set_create_from_union(ih_container_set_t *set_a,
    ih_container_set_t *set_b);

void ih_container_set_destroy(void *set_object);

ih_container_set_t *ih_container_set_difference(ih_container_set_t *set_a,
    ih_container_set_t *set_b);

void ih_container_set_dont_destroys(ih_container_set_t *set);

ih_core_bool_t ih_container_set_expunge(ih_container_set_t *set,
    ih_container_set_t *expunge_these);

void *ih_container_set_find(ih_container_set_t *set, void *decoy_object);

void *ih_container_set_find_any(ih_container_set_t *set);

void *ih_container_set_find_copy(ih_container_set_t *set, void *decoy_object);

void *ih_container_set_find_take(ih_container_set_t *set, void *decoy_object);

ih_container_array_t *ih_container_set_get_as_array(ih_container_set_t *set);

ih_container_list_t *ih_container_set_get_as_list(ih_container_set_t *set);

char *ih_container_set_get_as_delimited_string(ih_container_set_t *set,
    ih_core_get_as_string_f get_as_string, const char *delimiter);

ih_core_compare_f ih_container_set_get_compare_function(ih_container_set_t *set);

ih_core_copy_f ih_container_set_get_copy_function(ih_container_set_t *set);

ih_core_destroy_f ih_container_set_get_destroy_function(ih_container_set_t *set);

unsigned long ih_container_set_get_size(ih_container_set_t *set);

void *ih_container_set_iterate_next(ih_container_set_t *set);

void ih_container_set_iterate_remove(ih_container_set_t *set);

void ih_container_set_iterate_start(ih_container_set_t *set);

void ih_container_set_lock(ih_container_set_t *set);

ih_core_bool_t ih_container_set_overlaps(ih_container_set_t *set_a,
    ih_container_set_t *set_b);

void ih_container_set_print(ih_container_set_t *set,
    ih_core_get_as_string_f get_as_string);

ih_core_bool_t ih_container_set_remove(ih_container_set_t *set, void *object);

void ih_container_set_unlock(ih_container_set_t *set);

#endif
