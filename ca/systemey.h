#ifndef ih_ca_systemey_h
#define ih_ca_systemey_h

#include "ih/ca/cell.h"
#include "ih/ca/declarations.h"
#include "ih/core/color.h"
#include "ih/external/external.h"

#define IH_CA_NO_CREATE_CONTEXT_FUNCTION NULL
#define IH_CA_NO_DESTROY_CONTEXT_FUNCTION NULL
#define IH_CA_NO_GET_CELL_COLOR_FUNCTION NULL
#define IH_CA_NO_END_TIME_STEP_FUNCTION NULL
#define IH_CA_NO_START_TIME_STEP_FUNCTION NULL

typedef ih_ca_t (*ih_ca_system_calculate_new_cell_state_f)
(ih_ca_system_t *system, unsigned long cell_index);

typedef void *(*ih_ca_system_create_context_f)(void *name_object);

typedef void (*ih_ca_system_destroy_context_f)(void *context_object);

typedef void (*ih_ca_system_end_time_step_f)(ih_ca_system_t *system);

typedef void (*ih_ca_system_get_cell_color_f)(ih_ca_t *cell,
    ih_core_color_t *color);

typedef unsigned long (*ih_ca_system_get_relative_cell_index_f)
(ih_ca_system_t *system, unsigned long cell_index,
    unsigned long relationship);

typedef void (*ih_ca_system_start_time_step_f)(ih_ca_system_t *system);

struct ih_ca_systemey_t {
  void *name_object;

  ih_ca_system_calculate_new_cell_state_f calculate_new_cell_state;
  ih_ca_system_create_context_f create_context;
  ih_ca_system_destroy_context_f destroy_context;
  ih_ca_system_end_time_step_f end_time_step;
  ih_ca_system_get_cell_color_f get_cell_color;
  ih_ca_system_get_relative_cell_index_f get_relative_cell_index;
  ih_ca_system_start_time_step_f start_time_step;
};
typedef struct ih_ca_systemey_t ih_ca_systemey_t;

void ih_ca_systemey_init(ih_ca_systemey_t *systemey,
    void *name_object,
    ih_ca_system_calculate_new_cell_state_f calculate_new_cell_state,
    ih_ca_system_create_context_f create_context,
    ih_ca_system_destroy_context_f destroy_context,
    ih_ca_system_end_time_step_f end_time_step,
    ih_ca_system_get_cell_color_f get_cell_color,
    ih_ca_system_get_relative_cell_index_f get_relative_cell_index,
    ih_ca_system_start_time_step_f start_time_step);

#endif
