#ifndef ih_core_pair_h
#define ih_core_pair_h

#include "ih/core/object.h"
#include "ih/core/objectey.h"

#define IH_CORE_PAIR_DESTROYS ih_core_bool_true
#define IH_CORE_PAIR_DONT_DESTROY ih_core_bool_false

struct ih_core_pair_t;
typedef struct ih_core_pair_t ih_core_pair_t;

int ih_core_pair_compare(void *pair_object_a, void *pair_object_b);

int ih_core_pair_compare_left(void *pair_object_a, void *pair_object_b);

int ih_core_pair_compare_right(void *pair_object_a, void *pair_object_b);

void *ih_core_pair_copy(void *pair_object);

ih_core_pair_t *ih_core_pair_create(void *left, ih_core_objectey_t *left_objectey,
    void *right, ih_core_objectey_t *right_objectey, ih_core_bool_t destroys);

ih_core_pair_t *ih_core_pair_create_decoy(void *left,
    ih_core_objectey_t *left_objectey);

void ih_core_pair_destroy(void *pair_object);

void ih_core_pair_destroy_decoy(ih_core_pair_t *pair);

char *ih_core_pair_get_as_string(void *pair_object);

void *ih_core_pair_get_left(ih_core_pair_t *pair);

ih_core_objectey_t *ih_core_pair_get_left_objectey(ih_core_pair_t *pair);

void *ih_core_pair_get_right(ih_core_pair_t *pair);

ih_core_objectey_t *ih_core_pair_get_right_objectey(ih_core_pair_t *pair);

void ih_core_pair_init_objectey(ih_core_objectey_t *objectey);

#endif
