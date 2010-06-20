#ifndef ih_ca_k3_h
#define ih_ca_k3_h

#include "ih/ca/system.h"

ih_ca_t ih_ca_k3_calculate_new_cell_state(ih_ca_system_t *system,
    unsigned long cell_index);

void *ih_ca_k3_create_context(void *parameter_object);

void ih_ca_k3_destroy_context(void *context_object);

void ih_ca_k3_get_cell_color(ih_ca_t *cell, ih_core_color_t *color);

unsigned long ih_ca_k3_get_relative_cell_index(ih_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void ih_ca_k3_init_systemey(ih_ca_systemey_t *systemey, void *name_object);

#endif
