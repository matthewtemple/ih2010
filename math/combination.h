#ifndef ih_matih_combination_h
#define ih_matih_combination_h

#include "ih/container/set.h"

struct ih_matih_combination_t;
typedef struct ih_matih_combination_t ih_matih_combination_t;

ih_matih_combination_t *ih_matih_combination_create(ih_container_set_t *set);

void ih_matih_combination_destroy(ih_matih_combination_t *combination);

ih_container_set_t *ih_matih_combination_iterate_next
(ih_matih_combination_t *combination);

void ih_matih_combination_iterate_start(ih_matih_combination_t *combination);

#endif
