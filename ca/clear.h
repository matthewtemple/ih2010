#ifndef ih_ca_clear_h
#define ih_ca_clear_h

#include "ih/ca/system.h"
#include "ih/external/external.h"

ih_ca_t ih_ca_clear_calculate_new_cell_state(ih_ca_system_t *system,
    unsigned long cell_index);

void *ih_ca_clear_create_context(void *name_object);

unsigned long ih_ca_clear_get_relative_cell_index(ih_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void ih_ca_clear_destroy_context(void *context_object);

void ih_ca_clear_end_time_step(ih_ca_system_t *system);

void ih_ca_clear_init_systemey(ih_ca_systemey_t *systemey,
    void *name_object);

void ih_ca_clear_start_time_step(ih_ca_system_t *system);

#endif
