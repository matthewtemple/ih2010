#ifndef ih_ca_crow_h
#define ih_ca_crow_h

#include "ih/ca/system.h"

ih_ca_t ih_ca_crow_calculate_new_cell_state(ih_ca_system_t *system,
    unsigned long cell_index);

unsigned long ih_ca_crow_get_relative_cell_index(ih_ca_system_t *system,
    unsigned long cell_index, unsigned long relationship);

void ih_ca_crow_init_systemey(ih_ca_systemey_t *systemey, void *name_object);

#endif