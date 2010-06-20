#ifndef ih_box_system_h
#define ih_box_system_h

#include "ih/audit/log.h"
#include "ih/box/coordinate.h"
#include "ih/external/external.h"

typedef void * (*ih_box_object_get_cell_f)(void *object);

typedef void (*ih_box_object_set_cell_f)(void *object, void *cell);

struct ih_box_system_t;
typedef struct ih_box_system_t ih_box_system_t;

void ih_box_system_add(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate, void *object);

void ih_box_system_add_random(ih_box_system_t *system, void *object);

ih_box_system_t *ih_box_system_create
(ih_box_coordinate_t *dimension_coordinate,
    ih_box_object_get_cell_f get_cell, ih_box_object_set_cell_f set_cell,
    ih_core_compare_f compare_objects, ih_core_copy_f copy_object,
    ih_core_destroy_f destroy_object, ih_audit_log_t *log);

void ih_box_system_destroy(ih_box_system_t *system);

void *ih_box_system_find(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);

void *ih_box_system_find_random(ih_box_system_t *system);

void *ih_box_system_find_relative(ih_box_system_t *system, void *object,
    ih_box_coordinate_t *relative_coordinate);

unsigned long ih_box_system_get_volume(ih_box_system_t *system);

void *ih_box_system_iterate_next(ih_box_system_t *system);

void ih_box_system_iterate_start(ih_box_system_t *system);

void ih_box_system_move_absolute(ih_box_system_t *system, void *object,
    ih_box_coordinate_t *destination_coordinate);

void ih_box_system_move_relative(ih_box_system_t *system, void *object,
    ih_box_coordinate_t *relative_coordinate);

void ih_box_system_remove(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);

void ih_box_system_replace(ih_box_system_t *system, void *destination_object,
    void *source_object);

void ih_box_system_replace_random(ih_box_system_t *system, void *object);

void ih_box_system_swap(ih_box_system_t *system, void *object_a,
    void *object_b);

#endif
