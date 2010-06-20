#include "ih/ca/state.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_ca_state_t {
  ih_ca_t *cells;
  unsigned long cell_count;
};

int ih_ca_state_compare(void *state_object_a, void *state_object_b)
{
  ih_core_trace_exit("TODO: implement");
  return 0;
}

void *ih_ca_state_copy(void *state_object)
{
  assert(state_object);
  ih_ca_state_t *state;
  ih_ca_state_t *state_copy;
  unsigned long cells_memory_size;

  state = state_object;
  cells_memory_size = sizeof(ih_ca_t) * state->cell_count;

  state_copy = malloc(sizeof *state_copy);
  if (state_copy) {
    state_copy->cell_count = state->cell_count;
    state_copy->cells = malloc(cells_memory_size);
    if (state_copy->cells) {
      memcpy(state_copy->cells, state->cells, cells_memory_size);
    } else {
      ih_core_trace("malloc");
    }
  } else {
    ih_core_trace("malloc");
  }

  return state_copy;
}

ih_ca_state_t *ih_ca_state_create(unsigned long cell_count)
{
  ih_ca_state_t *state;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(sizeof(ih_ca_t) * cell_count);
    if (!state->cells) {
      ih_core_trace("malloc");
    }
  } else {
    ih_core_trace("malloc");
  }

  return state;
}

ih_ca_state_t *ih_ca_state_create_from_cells(ih_ca_t *cells,
    unsigned long cell_count)
{
  ih_ca_state_t *state;
  unsigned long cells_memory_size;

  cells_memory_size = sizeof(ih_ca_state_t) * cell_count;

  state = malloc(sizeof *state);
  if (state) {
    state->cell_count = cell_count;
    state->cells = malloc(cells_memory_size);
    if (state->cells) {
      memcpy(state->cells, cells, cells_memory_size);
    } else {
      ih_core_trace("malloc");
    }
  } else {
    ih_core_trace("malloc");
  }

  return state;
}

void ih_ca_state_destroy(void *state_object)
{
  assert(state_object);
  ih_ca_state_t *state;

  state = state_object;

  free(state->cells);
  free(state);
}

char *ih_ca_state_get_as_string(void *state_object)
{
  assert(state_object);
  ih_ca_state_t *state;
  unsigned long each_cell;
  char *string;
  char c;
  ih_ca_t *cell;
  unsigned long ih_ca_value;
  unsigned long cell_count;

  state = state_object;
  cell_count = state->cell_count;

  string = malloc(cell_count + 1);
  if (string) {
    for (each_cell = 0; each_cell < cell_count; each_cell++) {
      cell = &(*(state->cells + each_cell));
      ih_ca_value = cell->value;
      if (ih_ca_value < 10) {
        c = 48 + ih_ca_value;
      } else {
        c = 120;
      }
      *(string + each_cell) = c;
    }
    *(string + cell_count) = '\0';
  } else {
    ih_core_trace("malloc");
  }

  return string;
}

ih_ca_t *ih_ca_state_get_cell(ih_ca_state_t *state,
    unsigned long cell_index)
{
  return state->cells + cell_index;
}

unsigned long ih_ca_state_get_cell_count(ih_ca_state_t *state)
{
  return state->cell_count;
}

unsigned long ih_ca_state_get_cell_rule(ih_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).rule;
}

unsigned long ih_ca_state_get_cell_value(ih_ca_state_t *state,
    unsigned long cell_index)
{
  return (*(state->cells + cell_index)).value;
}

void ih_ca_state_print(ih_ca_state_t *state)
{
  ih_core_print(state, ih_ca_state_get_as_string);
}

void ih_ca_state_set_cell(ih_ca_state_t *state, unsigned long cell_index,
    ih_ca_t *cell)
{
  *(state->cells + cell_index) = ih_ca_copy(*cell);
}

void ih_ca_state_set_cell_rule(ih_ca_state_t *state,
    unsigned long cell_index, unsigned long ih_ca_rule)
{
  (*(state->cells + cell_index)).rule = ih_ca_rule;
}

void ih_ca_state_set_cell_value(ih_ca_state_t *state,
    unsigned long cell_index, unsigned long ih_ca_value)
{
  (*(state->cells + cell_index)).value = ih_ca_value;
}
