#ifndef ih_container_list_h
#define ih_container_list_h

#include "ih/container/declarations.h"
#include "ih/core/message.h"
#include "ih/core/object.h"
#include "ih/core/objectey.h"
#include "ih/core/types.h"

#define IH_CONTAINER_LIST_NO_SIZE_LIMIT 0

struct ih_container_list_t;
typedef struct ih_container_list_t ih_container_list_t;

ih_core_bool_t ih_container_list_absorb(ih_container_list_t *list,
    ih_container_list_t *absorb_these);

ih_core_bool_t ih_container_list_add_to_message(ih_container_list_t *list,
    ih_core_message_t *message, ih_core_message_add_to_message_f add_to_message);

ih_core_bool_t ih_container_list_add_first(ih_container_list_t *list,
    void *object);

ih_core_bool_t ih_container_list_add_last(ih_container_list_t *list,
    void *object);

void ih_container_list_clear(ih_container_list_t *list);

int ih_container_list_compare(void *list_object_a, void *list_object_b);

void *ih_container_list_copy(void *list_object);

ih_container_list_t *ih_container_list_create(ih_core_compare_f compare,
    ih_core_copy_f copy, ih_core_destroy_f destroy);

ih_container_list_t *ih_container_list_create_from_array_n
(ih_container_array_t *array, unsigned long max_size);

ih_container_list_t *ih_container_list_create_from_message
(ih_core_compare_f compare, ih_core_copy_f copy, ih_core_destroy_f destroy,
    ih_core_message_t *message,
    ih_core_message_create_from_message_f create_from_message);

ih_container_list_t *ih_container_list_create_strings_from_string(char *string,
    const char *separators);

void ih_container_list_destroy(void *list_object);

void ih_container_list_dont_destroys(ih_container_list_t *list);

void *ih_container_list_find_at(ih_container_list_t *list, unsigned long index);

void *ih_container_list_find_first(ih_container_list_t *list);

void *ih_container_list_find_last(ih_container_list_t *list);

void *ih_container_list_find_random(ih_container_list_t *list);

char *ih_container_list_get_as_delimited_string(ih_container_list_t *list,
    ih_core_get_as_string_f get_as_string, const char *delimiter);

char *ih_container_list_get_as_string(void *list_object);

unsigned long ih_container_list_get_size(ih_container_list_t *list);

void ih_container_list_init_objectey(ih_core_objectey_t *objectey);

void *ih_container_list_iterate_next(ih_container_list_t *list);

void ih_container_list_iterate_remove(ih_container_list_t *list);

void ih_container_list_iterate_start(ih_container_list_t *list);

void ih_container_list_lock(ih_container_list_t *list);

void ih_container_list_print(void *list_object);

ih_core_bool_t ih_container_list_remove_first(ih_container_list_t *list);

ih_core_bool_t ih_container_list_remove_last(ih_container_list_t *list);

void ih_container_list_set_size_limit(ih_container_list_t *list,
    unsigned long size_limit);

void ih_container_list_unlock(ih_container_list_t *list);

#endif
