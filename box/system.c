#include "ih/box/system.h"
#include "ih/container/array.h"

struct ih_box_system_t {
  ih_box_coordinate_t dimension_coordinate;
  unsigned long volume;
  ih_container_array_t *cells;
  ih_box_object_get_cell_f get_cell;
  ih_box_object_set_cell_f set_cell;
  ih_core_destroy_f destroy_object;
  ih_audit_log_t *log;
};

struct ih_box_cell_t;
typedef struct ih_box_cell_t ih_box_cell_t;

struct ih_box_cell_t {
  ih_box_system_t *system;
  ih_box_coordinate_t coordinate;
  ih_box_cell_t *neighbors[3][3][3];
  void *object;
};

#ifdef IH_BUILD_DEVELOPMENT
static ih_core_bool_t coordinate_in_range(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);
#endif

static ih_core_bool_t create_cells(ih_box_system_t *system);

static ih_box_cell_t *find_cell(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);

static unsigned long get_cells_array_index(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);

static int ih_box_cell_compare(void *cell_a_void, void *cell_b_void);

static void *ih_box_cell_copy(void *cell_void);

static ih_box_cell_t *ih_box_cell_create(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate);

static void ih_box_cell_destroy(void *cell_void);

#ifdef IH_BUILD_DEVELOPMENT
static ih_core_bool_t relative_coordinate_in_range
(ih_box_coordinate_t *relative_coordinate);
#endif

static void set_neighbor_references(ih_box_system_t *system);

static void set_neighbor_references_for_cell(ih_box_system_t *system,
    ih_box_cell_t *cell);

#ifdef IH_BUILD_DEVELOPMENT
ih_core_bool_t coordinate_in_range(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  ih_core_bool_t in_range;

  if ((coordinate->x <= system->dimension_coordinate.x)
      && (coordinate->y <= system->dimension_coordinate.y)
      && (coordinate->z <= system->dimension_coordinate.z)) {
    in_range = ih_core_bool_true;
  } else {
    in_range = ih_core_bool_false;
  }

  return in_range;
}
#endif

ih_core_bool_t create_cells(ih_box_system_t *system)
{
  assert(system);
  ih_core_bool_t success;
  ih_box_coordinate_t coordinate;
  ih_box_cell_t *cell;
  unsigned long cells_array_index;

  success = ih_core_bool_true;

  system->cells = ih_container_array_create(system->volume,
      ih_box_cell_compare, ih_box_cell_copy, ih_box_cell_destroy);
  if (system->cells) {
    for (coordinate.x = 0; coordinate.x < system->dimension_coordinate.x;
         coordinate.x++) {
      for (coordinate.y = 0; coordinate.y < system->dimension_coordinate.y;
           coordinate.y++) {
        for (coordinate.z = 0; coordinate.z < system->dimension_coordinate.z;
             coordinate.z++) {
          cell = ih_box_cell_create(system, &coordinate);
          if (cell) {
            cells_array_index = get_cells_array_index(system, &coordinate);
            ih_container_array_add(system->cells, cells_array_index, cell);
          } else {
            success = ih_core_bool_false;
            ih_audit_log_trace(system->log, "box", "ih_box_cell_create");
          }
        }
      }
    }
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(system->log, "box", "ih_container_array_create");
  }

  return success;
}

ih_box_cell_t *find_cell(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  unsigned long cells_array_index;

  cells_array_index = get_cells_array_index(system, coordinate);
  return ih_container_array_find(system->cells, cells_array_index);
}

unsigned long get_cells_array_index(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  return (coordinate->x * system->dimension_coordinate.y
      * system->dimension_coordinate.z)
    + (coordinate->y * system->dimension_coordinate.z)
    + coordinate->z;
}

int ih_box_cell_compare(void *cell_a_void, void *cell_b_void)
{
  ih_core_trace_exit("TODO: implement");
  return 0;
}

void *ih_box_cell_copy(void *cell_void)
{
  ih_core_trace_exit("TODO: implement");
  return NULL;
}

ih_box_cell_t *ih_box_cell_create(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  ih_box_cell_t *cell;
  unsigned char each_x;
  unsigned char each_y;
  unsigned char each_z;

  cell = malloc(sizeof *cell);
  if (cell) {
    cell->system = system;
    ih_box_coordinate_init_witih_coordinate(&cell->coordinate, coordinate);
    cell->object = NULL;
    for (each_x = 0; each_x < 3; each_x++) {
      for (each_y = 0; each_y < 3; each_y++) {
        for (each_z = 0; each_z < 3; each_z++) {
          cell->neighbors[each_x][each_y][each_z] = NULL;
        }
      }
    }
  } else {
    ih_core_trace("malloc");
  }

  return cell;
}

void ih_box_cell_destroy(void *cell_void)
{
  assert(cell_void);
  ih_box_cell_t *cell;
  ih_box_system_t *system;

  cell = cell_void;
  system = cell->system;

  if (cell->object) {
    system->set_cell(cell->object, NULL);
    if (system->destroy_object) {
      system->destroy_object(cell->object);
    }
  }

  free(cell_void);
}

void ih_box_system_add(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate, void *object)
{
  assert(system);
  assert(coordinate);
  assert(coordinate_in_range(system, coordinate));
  assert(object);
  ih_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  cell->object = object;
  system->set_cell(object, cell);
}

void ih_box_system_add_random(ih_box_system_t *system, void *object)
{
  assert(system);
  assert(object);
  ih_box_coordinate_t coordinate;

  ih_box_coordinate_init_witih_random(&coordinate,
      &system->dimension_coordinate);
  ih_box_system_add(system, &coordinate, object);
}

ih_box_system_t *ih_box_system_create
(ih_box_coordinate_t *dimension_coordinate,
    ih_box_object_get_cell_f get_cell, ih_box_object_set_cell_f set_cell,
    ih_core_compare_f compare_objects, ih_core_copy_f copy_object,
    ih_core_destroy_f destroy_object, ih_audit_log_t *log)
{
  assert(dimension_coordinate);
  assert(dimension_coordinate->x >= 1);
  assert(dimension_coordinate->y >= 1);
  assert(dimension_coordinate->z >= 1);
  assert(get_cell);
  assert(set_cell);
  assert(compare_objects);
  assert(copy_object);
  assert(log);
  ih_box_system_t *system;
  ih_core_bool_t so_far_so_good;

  system = malloc(sizeof *system);
  if (system) {
    ih_box_coordinate_copy(&system->dimension_coordinate,
        dimension_coordinate);
    system->get_cell = get_cell;
    system->set_cell = set_cell;
    system->destroy_object = destroy_object;
    system->log = log;
    system->volume = dimension_coordinate->x * dimension_coordinate->y
      * dimension_coordinate->z;
    if (create_cells(system)) {
      so_far_so_good = ih_core_bool_true;
      set_neighbor_references(system);
    } else {
      so_far_so_good = ih_core_bool_false;
      ih_audit_log_trace(log, "box", "create_cells");
    }
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (!so_far_so_good && system) {
    if (system->cells) {
      ih_container_array_destroy(system->cells);
    }
    free(system);
    system = NULL;
  }

  return system;
}

void ih_box_system_destroy(ih_box_system_t *system)
{
  assert(system);
  ih_container_array_destroy(system->cells);
  free(system);
}

void *ih_box_system_find(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  assert(coordinate_in_range(system, coordinate));
  ih_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  return cell->object;
}

void *ih_box_system_find_random(ih_box_system_t *system)
{
  assert(system);
  ih_box_cell_t *cell;

  cell = ih_container_array_find_random(system->cells);
  return cell->object;
}

void *ih_box_system_find_relative(ih_box_system_t *system,
    void *object, ih_box_coordinate_t *relative_coordinate)
{
  assert(system);
  assert(object);
  assert(relative_coordinate);
  assert(relative_coordinate_in_range(relative_coordinate));
  ih_box_cell_t *cell;
  ih_box_cell_t *neighbor;
  void *neighbor_object;

  cell = system->get_cell(object);
  neighbor = cell->neighbors[relative_coordinate->x][relative_coordinate->y]
    [relative_coordinate->z];
  if (neighbor) {
    neighbor_object = neighbor->object;
  } else {
    neighbor_object = NULL;
  }

  return neighbor_object;
}

unsigned long ih_box_system_get_volume(ih_box_system_t *system)
{
  return system->volume;
}

void *ih_box_system_iterate_next(ih_box_system_t *system)
{
  assert(system);
  ih_box_cell_t *cell;
  void *object;

  cell = ih_container_array_iterate_next(system->cells);
  if (cell) {
    object = cell->object;
  } else {
    object = NULL;
  }

  return object;
}

void ih_box_system_iterate_start(ih_box_system_t *system)
{
  ih_container_array_iterate_start(system->cells);
}

void ih_box_system_move_absolute(ih_box_system_t *system, void *object,
    ih_box_coordinate_t *destination_coordinate)
{
  assert(system);
  assert(object);
  assert(destination_coordinate);
  ih_box_cell_t *source_cell;
  ih_box_cell_t *destination_cell;

  source_cell = system->get_cell(object);
  destination_cell = find_cell(system, destination_coordinate);

  if (source_cell != destination_cell) {
    ih_box_system_remove(system, destination_coordinate);
    ih_box_system_add(system, destination_coordinate, object);
    source_cell->object = NULL;
  }
}

void ih_box_system_move_relative(ih_box_system_t *system, void *object,
    ih_box_coordinate_t *relative_coordinate)
{
  assert(system);
  assert(object);
  assert(relative_coordinate);
  ih_box_cell_t *source_cell;
  ih_box_cell_t *destination_cell;

  source_cell = system->get_cell(object);
  destination_cell = source_cell->neighbors[relative_coordinate->x]
    [relative_coordinate->y][relative_coordinate->z];

  if (source_cell != destination_cell) {
    ih_box_system_remove(system, &destination_cell->coordinate);
    ih_box_system_add(system, &destination_cell->coordinate, object);
    source_cell->object = NULL;
  }
}

void ih_box_system_remove(ih_box_system_t *system,
    ih_box_coordinate_t *coordinate)
{
  assert(system);
  assert(coordinate);
  ih_box_cell_t *cell;

  cell = find_cell(system, coordinate);
  if (cell->object) {
    system->set_cell(cell->object, NULL);
    if (system->destroy_object) {
      system->destroy_object(cell->object);
    }
    cell->object = NULL;
  }
}

void ih_box_system_replace(ih_box_system_t *system, void *destination_object,
    void *source_object)
{
  assert(system);
  assert(destination_object);
  assert(source_object);
  ih_box_cell_t *destination_cell;

  destination_cell = system->get_cell(destination_object);

  ih_box_system_remove(system, &destination_cell->coordinate);
  ih_box_system_add(system, &destination_cell->coordinate, source_object);
}

void ih_box_system_replace_random(ih_box_system_t *system, void *object)
{
  assert(system);
  assert(object);
  ih_box_coordinate_t coordinate;

  ih_box_coordinate_init_witih_random(&coordinate,
      &system->dimension_coordinate);

  ih_box_system_remove(system, &coordinate);
  ih_box_system_add(system, &coordinate, object);
}

void ih_box_system_swap(ih_box_system_t *system, void *object_a,
    void *object_b)
{
  assert(system);
  assert(object_a);
  assert(object_b);
  ih_box_cell_t *cell_a;
  ih_box_cell_t *cell_b;

  cell_a = system->get_cell(object_a);
  cell_b = system->get_cell(object_b);

  cell_a->object = object_b;
  cell_b->object = object_a;

  system->set_cell(object_a, cell_b);
  system->set_cell(object_b, cell_a);
}

#ifdef IH_BUILD_DEVELOPMENT
ih_core_bool_t relative_coordinate_in_range
(ih_box_coordinate_t *relative_coordinate)
{
  assert(relative_coordinate);
  ih_core_bool_t in_range;

  if ((relative_coordinate->x < 3) && (relative_coordinate->y < 3)
      && (relative_coordinate->z < 3)) {
    in_range = ih_core_bool_true;
  } else {
    in_range = ih_core_bool_false;
  }

  return in_range;
}
#endif

void set_neighbor_references(ih_box_system_t *system)
{
  assert(system);
  ih_box_cell_t *cell;
  unsigned long each_cell;

  for (each_cell = 0; each_cell < system->volume; each_cell++) {
    cell = ih_container_array_find(system->cells, each_cell);
    set_neighbor_references_for_cell(system, cell);
  }
}

void set_neighbor_references_for_cell(ih_box_system_t *system,
    ih_box_cell_t *cell)
{
  assert(system);
  assert(cell);
  ih_box_coordinate_t neighbor_coordinate;
  ih_box_coordinate_t relative_coordinate;
  ih_box_cell_t *neighbor;

  for (relative_coordinate.x = 0;
       relative_coordinate.x < 3;
       relative_coordinate.x++) {
    for (relative_coordinate.y = 0;
         relative_coordinate.y < 3;
         relative_coordinate.y++) {
      for (relative_coordinate.z = 0;
           relative_coordinate.z < 3;
           relative_coordinate.z++) {
        neighbor_coordinate.x = ih_core_wrap_index
          (cell->coordinate.x + (relative_coordinate.x - 1),
              system->dimension_coordinate.x);
        neighbor_coordinate.y = ih_core_wrap_index
          (cell->coordinate.y + (relative_coordinate.y - 1),
              system->dimension_coordinate.y);
        neighbor_coordinate.z = ih_core_wrap_index
          (cell->coordinate.z + (relative_coordinate.z - 1),
              system->dimension_coordinate.z);
        neighbor = find_cell(system, &neighbor_coordinate);
        cell->neighbors[relative_coordinate.x][relative_coordinate.y]
          [relative_coordinate.z] = neighbor;
      }
    }
  }
}
