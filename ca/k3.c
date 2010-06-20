#include "ih/ca/eca.h"
#include "ih/ca/k3.h"
#include "ih/core/constants.h"
#include "ih/core/bit.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct k3_cell_t {
  ih_core_bit_t a;
  ih_core_bit_t b;
  ih_core_bit_t c;
};
typedef struct k3_cell_t k3_cell_t;

struct k3_context_t {
  ih_core_bit_t rule[24];
  k3_cell_t map[8];
};
typedef struct k3_context_t k3_context_t;

ih_ca_t ih_ca_k3_calculate_new_cell_state(ih_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  unsigned long neighbor_0_index;
  unsigned long neighbor_1_index;
  unsigned long neighbor_2_index;
  ih_ca_t *neighbor_0_cell;
  ih_ca_t *neighbor_1_cell;
  ih_ca_t *neighbor_2_cell;
  unsigned long new_cell_value;
  ih_ca_t new_cell_state;
  ih_core_bit_t in_a;
  ih_core_bit_t in_b;
  ih_core_bit_t in_c;
  ih_core_bit_t link_number;
  ih_core_bit_t out_a;
  ih_core_bit_t out_b;
  ih_core_bit_t out_c;
  k3_context_t *context;

  context = ih_ca_system_get_context(system);

  neighbor_0_index
    = ih_ca_eca_get_relative_cell_index(system, cell_index, 0);
  neighbor_1_index
    = ih_ca_eca_get_relative_cell_index(system, cell_index, 1);
  neighbor_2_index
    = ih_ca_eca_get_relative_cell_index(system, cell_index, 2);

  neighbor_0_cell = ih_ca_system_get_current_cell(system, neighbor_0_index);
  neighbor_1_cell = ih_ca_system_get_current_cell(system, neighbor_1_index);
  neighbor_2_cell = ih_ca_system_get_current_cell(system, neighbor_2_index);

  in_a = neighbor_0_cell->value / 4;
  in_b = (neighbor_1_cell->value / 2) % 2;
  in_c = neighbor_2_cell->value % 2;

  link_number = (in_a) + (in_b * 2) + (in_c * 4);
  out_a = context->map[link_number].a;
  out_b = context->map[link_number].b;
  out_c = context->map[link_number].c;

  new_cell_value = (out_a) + (out_b * 2) + (out_c * 4);

  new_cell_state.value = new_cell_value;
  ih_ca_init(&new_cell_state, new_cell_value, IH_CA_NO_RULE);

  return new_cell_state;
}

void *ih_ca_k3_create_context(void *parameter_object)
{
  k3_context_t *context;
  unsigned long rule_number;
  unsigned long value;
  unsigned long place_value;
  unsigned short each_bit;
  unsigned short each_link;
  unsigned long div;

  context = malloc(sizeof *context);
  if (context) {
    rule_number = *((unsigned long *) parameter_object);

    value = rule_number;
    place_value = 8388608;  /*  2^23  */
    for (each_bit = 0; each_bit < 24; each_bit++) {
      div = value / place_value;
      *(context->rule + each_bit) = div;
      value = value % place_value;
      place_value /= 2;
      if (0 == place_value) {
        break;
      }
    }

    for (each_link = 0; each_link < 8; each_link++) {
      (*(context->map + each_link)).a = *(context->rule + (each_link * 3) + 0);
      (*(context->map + each_link)).b = *(context->rule + (each_link * 3) + 1);
      (*(context->map + each_link)).c = *(context->rule + (each_link * 3) + 2);
    }
  } else {
    ih_core_trace("malloc");
  }

  return context;
}

void ih_ca_k3_destroy_context(void *context_object)
{
  free(context_object);
}

void ih_ca_k3_get_cell_color(ih_ca_t *cell, ih_core_color_t *color)
{
  ih_core_bit_t a;
  ih_core_bit_t b;
  ih_core_bit_t c;

  a = cell->value % 2;
  b = (cell->value / 2) % 2;
  c = cell->value / 4;

  if (0 == a) {
    color->red = IH_CORE_MAX_COLOR;
  } else {
    color->red = 0;
  }
  if (0 == b) {
    color->green = IH_CORE_MAX_COLOR;
  } else {
    color->green = 0;
  }
  if (0 == c) {
    color->blue = IH_CORE_MAX_COLOR;
  } else {
    color->blue = 0;
  }
}

unsigned long ih_ca_k3_get_relative_cell_index(ih_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship)
{
  return ih_ca_eca_get_relative_cell_index(system, cell_index, relationship);
}

void ih_ca_k3_init_systemey(ih_ca_systemey_t *systemey, void *name_object)
{
  ih_ca_systemey_init(systemey, name_object,
      ih_ca_k3_calculate_new_cell_state, ih_ca_k3_create_context,
      ih_ca_k3_destroy_context, IH_CA_NO_END_TIME_STEP_FUNCTION,
      ih_ca_k3_get_cell_color, ih_ca_k3_get_relative_cell_index,
      IH_CA_NO_START_TIME_STEP_FUNCTION);
}
