#ifndef ih_ca_state_h
#define ih_ca_state_h

#include "ih/ca/cell.h"

struct ih_ca_state_t;
typedef struct ih_ca_state_t ih_ca_state_t;

int ih_ca_state_compare(void *state_object_a, void *state_object_b);

void *ih_ca_state_copy(void *state_object);

ih_ca_state_t *ih_ca_state_create(unsigned long cell_count);

ih_ca_state_t *ih_ca_state_create_from_cells(ih_ca_t *cells,
    unsigned long cell_count);

void ih_ca_state_destroy(void *state_object);

char *ih_ca_state_get_as_string(void *state_object);

ih_ca_t *ih_ca_state_get_cell(ih_ca_state_t *state,
    unsigned long cell_index);

unsigned long ih_ca_state_get_cell_count(ih_ca_state_t *state);

unsigned long ih_ca_state_get_cell_rule(ih_ca_state_t *state,
    unsigned long cell_index);

unsigned long ih_ca_state_get_cell_value(ih_ca_state_t *state,
    unsigned long cell_index);

void ih_ca_state_print(ih_ca_state_t *state);

void ih_ca_state_set_cell(ih_ca_state_t *state, unsigned long cell_index,
    ih_ca_t *cell);

void ih_ca_state_set_cell_rule(ih_ca_state_t *state,
    unsigned long cell_index, unsigned long ih_ca_rule);

void ih_ca_state_set_cell_value(ih_ca_state_t *state,
    unsigned long cell_index, unsigned long ih_ca_value);

#endif
