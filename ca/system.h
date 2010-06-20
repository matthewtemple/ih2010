#ifndef ih_ca_system_h
#define ih_ca_system_h

#include "ih/ca/state.h"
#include "ih/ca/systemey.h"
#include "ih/container/array.h"
#include "ih/core/run.h"

ih_ca_system_t *ih_ca_system_create
(ih_container_array_t *initial_state_history,
    unsigned long initial_time_step_count, ih_ca_systemey_t *systemey);

void ih_ca_system_default_get_cell_color(ih_ca_t *cell,
    ih_core_color_t *color);

void ih_ca_system_destroy(ih_ca_system_t *system);

unsigned long ih_ca_system_get_age_in_time_steps(ih_ca_system_t *system);

unsigned long ih_ca_system_get_cell_count(ih_ca_system_t *system);

ih_ca_t *ih_ca_system_get_cell(ih_ca_system_t *system,
    unsigned long time_step, unsigned long cell_index);

void ih_ca_system_get_cell_color_binary(ih_ca_t *cell,
    ih_core_color_t *color);

void *ih_ca_system_get_context(ih_ca_system_t *system);

ih_ca_t *ih_ca_system_get_current_cell(ih_ca_system_t *system,
    unsigned long cell_index);

ih_ca_state_t *ih_ca_system_get_current_state(ih_ca_system_t *system);

void *ih_ca_system_get_name(ih_ca_system_t *system);

ih_ca_t *ih_ca_system_get_relative_cell(ih_ca_system_t *system,
    long relative_time_step, unsigned long cell_index);

ih_ca_state_t *ih_ca_system_get_state(ih_ca_system_t *system,
    unsigned long time_step);

void ih_ca_system_run(ih_ca_system_t *system, ih_core_run_t *run);

ih_core_bool_t ih_ca_system_save_snapshot_jpeg(ih_ca_system_t *system,
    char *filename);

ih_core_bool_t ih_ca_system_save_snapshot_text(ih_ca_system_t *system,
    char *filename);

void ih_ca_system_set_cell(ih_ca_system_t *system, unsigned long time_step,
    unsigned long cell_index, ih_ca_t *cell);

void ih_ca_system_set_current_cell(ih_ca_system_t *system,
    unsigned long cell_index, ih_ca_t *cell);

void ih_ca_system_set_random_seed(ih_ca_system_t *system,
    unsigned long seed);

void ih_ca_system_set_relative_cell(ih_ca_system_t *system,
    long relative_time_step, unsigned long cell_index, ih_ca_t *cell);

#endif
