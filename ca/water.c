#include "ih/ca/water.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

ih_ca_t ih_ca_water_calculate_new_cell_state(ih_ca_system_t *system,
    unsigned long cell_index)
{
  assert(system);
  void *name_object;
  char *name;
  unsigned long neighbor_0_index;
  unsigned long neighbor_1_index;
  unsigned long neighbor_2_index;
  unsigned long neighbor_3_index;
  unsigned long neighbor_4_index;
  unsigned long neighbor_5_index;
  unsigned long neighbor_6_index;
  unsigned long neighbor_7_index;
  unsigned long neighbor_8_index;
  ih_ca_t *neighbor_0_cell;
  ih_ca_t *neighbor_1_cell;
  ih_ca_t *neighbor_2_cell;
  ih_ca_t *neighbor_3_cell;
  ih_ca_t *neighbor_4_cell;
  ih_ca_t *neighbor_5_cell;
  ih_ca_t *neighbor_6_cell;
  ih_ca_t *neighbor_7_cell;
  ih_ca_t *neighbor_8_cell;
  unsigned long neighbor_0_value;
  unsigned long neighbor_1_value;
  unsigned long neighbor_2_value;
  unsigned long neighbor_3_value;
  unsigned long neighbor_4_value;
  unsigned long neighbor_5_value;
  unsigned long neighbor_6_value;
  unsigned long neighbor_7_value;
  unsigned long neighbor_8_value;
  unsigned short neighborhood;
  unsigned long new_cell_value;
  ih_ca_t new_cell_state;

  name_object = ih_ca_system_get_name(system);
  name = name_object;

  neighbor_0_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 0);
  neighbor_1_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 1);
  neighbor_2_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 2);
  neighbor_3_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 3);
  neighbor_4_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 4);
  neighbor_5_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 5);
  neighbor_6_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 6);
  neighbor_7_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 7);
  neighbor_8_index = ih_ca_water_get_relative_cell_index
    (system, cell_index, 8);

  neighbor_0_cell = ih_ca_system_get_relative_cell
    (system, -2, neighbor_0_index);
  neighbor_1_cell = ih_ca_system_get_relative_cell
    (system, -2, neighbor_1_index);
  neighbor_2_cell = ih_ca_system_get_relative_cell
    (system, -2, neighbor_2_index);
  neighbor_3_cell = ih_ca_system_get_relative_cell
    (system, -1, neighbor_3_index);
  neighbor_4_cell = ih_ca_system_get_relative_cell
    (system, -1, neighbor_4_index);
  neighbor_5_cell = ih_ca_system_get_relative_cell
    (system, -1, neighbor_5_index);
  neighbor_6_cell = ih_ca_system_get_current_cell(system, neighbor_6_index);
  neighbor_7_cell = ih_ca_system_get_current_cell(system, neighbor_7_index);
  neighbor_8_cell = ih_ca_system_get_current_cell(system, neighbor_8_index);

  neighbor_0_value = neighbor_0_cell->value;
  neighbor_1_value = neighbor_1_cell->value;
  neighbor_2_value = neighbor_2_cell->value;
  neighbor_3_value = neighbor_3_cell->value;
  neighbor_4_value = neighbor_4_cell->value;
  neighbor_5_value = neighbor_5_cell->value;
  neighbor_6_value = neighbor_6_cell->value;
  neighbor_7_value = neighbor_7_cell->value;
  neighbor_8_value = neighbor_8_cell->value;

  neighborhood = (neighbor_0_value * 256) + (neighbor_1_value * 128)
    + (neighbor_2_value * 64) + (neighbor_3_value * 32)
    + (neighbor_4_value * 16) + (neighbor_5_value * 8)
    + (neighbor_6_value * 4) + (neighbor_7_value * 2) + neighbor_8_value;

  if ('0' == *(name + neighborhood)) {
    new_cell_value = 0;
  } else {
    new_cell_value = 1;
  }

  new_cell_state.value = new_cell_value;
  ih_ca_init(&new_cell_state, new_cell_value, IH_CA_NO_RULE);

  return new_cell_state;
}

unsigned long ih_ca_water_get_relative_cell_index(ih_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship)
{
  assert(system);
  unsigned long cell_count;
  unsigned long relative_cell_index;

  cell_count = ih_ca_system_get_cell_count(system);

  if (0 == cell_index) {
    switch (relationship) {
      case 0:
      case 3:
      case 6:
        relative_cell_index = cell_count - 1;
        break;
      case 1:
      case 4:
      case 7:
        relative_cell_index = cell_index;
        break;
      case 2:
      case 5:
      case 8:
        relative_cell_index = cell_index + 1;
        break;
      default:
        ih_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  } else if ((cell_count - 1) == cell_index) {
    switch (relationship) {
      case 0:
      case 3:
      case 6:
        relative_cell_index = cell_index - 1;
        break;
      case 1:
      case 4:
      case 7:
        relative_cell_index = cell_index;
        break;
      case 2:
      case 5:
      case 8:
        relative_cell_index = 0;
        break;
      default:
        ih_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  } else {
    switch (relationship) {
      case 0:
      case 3:
      case 6:
        relative_cell_index = cell_index - 1;
        break;
      case 1:
      case 4:
      case 7:
        relative_cell_index = cell_index;
        break;
      case 2:
      case 5:
      case 8:
        relative_cell_index = cell_index + 1;
        break;
      default:
        ih_core_trace("invalid cell relationship index");
        relative_cell_index = cell_index;
        break;
    }

  }

  return relative_cell_index;
}

void ih_ca_water_init_systemey(ih_ca_systemey_t *systemey, void *name_object)
{
  ih_ca_systemey_init(systemey, name_object,
      ih_ca_water_calculate_new_cell_state, IH_CA_NO_CREATE_CONTEXT_FUNCTION,
      IH_CA_NO_DESTROY_CONTEXT_FUNCTION, IH_CA_NO_END_TIME_STEP_FUNCTION,
      ih_ca_system_get_cell_color_binary,
      ih_ca_water_get_relative_cell_index,
      IH_CA_NO_START_TIME_STEP_FUNCTION);
}
