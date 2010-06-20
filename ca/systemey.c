#include "ih/ca/system.h"
#include "ih/ca/systemey.h"
#include "ih/external/external.h"

void ih_ca_systemey_init
(ih_ca_systemey_t *systemey, void *name_object,
    ih_ca_system_calculate_new_cell_state_f calculate_new_cell_state,
    ih_ca_system_create_context_f create_context,
    ih_ca_system_destroy_context_f destroy_context,
    ih_ca_system_end_time_step_f end_time_step,
    ih_ca_system_get_cell_color_f get_cell_color,
    ih_ca_system_get_relative_cell_index_f get_relative_cell_index,
    ih_ca_system_start_time_step_f start_time_step)
{
  assert(systemey);
  assert(calculate_new_cell_state);

  systemey->name_object = name_object;
  systemey->calculate_new_cell_state = calculate_new_cell_state;
  systemey->create_context = create_context;
  systemey->destroy_context = destroy_context;
  systemey->end_time_step = end_time_step;

  if (get_cell_color) {
    systemey->get_cell_color = get_cell_color;
  } else {
    systemey->get_cell_color = ih_ca_system_default_get_cell_color;
  }

  systemey->get_relative_cell_index = get_relative_cell_index;
  systemey->start_time_step = start_time_step;

  assert(systemey->calculate_new_cell_state && systemey->get_cell_color);
}
