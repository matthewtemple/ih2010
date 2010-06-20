#ifndef ih_cor3_actor_h
#define ih_cor3_actor_h

#include "ih/cor3/declarations.h"
#include "ih/core/actorey.h"
#include "ih/external/external.h"

void ih_cor3_actor_act(ih_cor3_actor_t *actor);

int ih_cor3_actor_compare_maximize(void *actor_a_void, void *actor_b_void);

int ih_cor3_actor_compare_minimize(void *actor_a_void, void *actor_b_void);

void *ih_cor3_actor_copy(void *actor_void);

void *ih_cor3_actor_create(void *system_void, ih_core_bitarray_t *solution);

void *ih_cor3_actor_create_random(void *system_void);

void ih_cor3_actor_destroy(void *actor_void);

void *ih_cor3_actor_get_box_cell(void *actor_void);

ih_core_bitarray_t *ih_cor3_actor_get_solution(void *actor_void);

void ih_cor3_actor_init_actorey(ih_core_actorey_t *actorey);

void ih_cor3_actor_set_box_cell(void *actor_void, void *box_cell);

#endif
