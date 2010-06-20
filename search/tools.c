#include "ih/core/constants.h"
#include "ih/search/tools.h"

static ih_core_bool_t init_box_add_random_actors(ih_box_system_t *box,
    ih_box_coordinate_t *dimension_coordinate,
    ih_core_create_actor_random_f create_actor_random, void *search_system,
    ih_audit_log_t *log);

static ih_core_bool_t init_box_add_actors_from_initial_solutions
(ih_box_system_t *box, ih_core_actorey_create_f actor_create,
    void *search_system, ih_container_array_t *initial_solutions,
    ih_audit_log_t *log);

ih_container_array_t *ih_core_create_solutions_from_box(ih_box_system_t *box,
    unsigned long max_solution_count, ih_core_get_solution_f get_solution,
    ih_core_score_solution_f score_solution, ih_core_goal_t goal,
    ih_core_compare_f compare_maximize, ih_core_compare_f compare_minimize,
    ih_core_copy_f copy_object, void *context, ih_audit_log_t *log)
{
  assert(box);
  assert(get_solution);
  assert(score_solution);
  assert(compare_maximize);
  assert(compare_minimize);
  assert(copy_object);
  assert(log);
  ih_container_array_t *solutions;
  ih_core_compare_f compare_objects;
  unsigned long box_volume;
  unsigned long index;
  ih_core_bitarray_t *solution;
  ih_core_bitarray_t *solution_copy;
  void *object;
  ih_container_array_t *objects;

  if (IH_CORE_GOAL_MAXIMIZE_SCORE == goal) {
    compare_objects = compare_maximize;
  } else {
    compare_objects = compare_minimize;
  }

  box_volume = ih_box_system_get_volume(box);
  max_solution_count = ih_core_min(max_solution_count, box_volume);

  objects = ih_container_array_create(box_volume, compare_objects,
      copy_object, IH_CORE_NO_DESTROY_FUNCTION);
  if (objects) {
    ih_box_system_iterate_start(box);
    index = 0;
    while ((object = ih_box_system_iterate_next(box))) {
      ih_container_array_add(objects, index, object);
      index++;
    }

    ih_container_array_sort(objects);

    solutions = ih_container_array_create(max_solution_count,
        ih_core_bitarray_compare, ih_core_bitarray_copy,
        ih_core_bitarray_destroy);
    if (solutions) {
      ih_container_array_iterate_start(objects);
      for (index = 0; index < max_solution_count; index++) {
        object = ih_container_array_iterate_next(objects);
        solution = get_solution(object);
        if (solution) {
          solution_copy = ih_core_bitarray_copy(solution);
          if (solution_copy) {
            ih_container_array_add(solutions, index, solution_copy);
          } else {
            ih_audit_log_trace(log, "ih", "ih_core_bitarray_copy");
          }
        } else {
          ih_audit_log_trace(log, "ih", "ih_core_bitarray_copy");
        }
        index++;
      }
    } else {
      ih_audit_log_trace(log, "ih", "ih_container_array_create");
    }
    ih_container_array_destroy(objects);
  } else {
    solutions = NULL;
    ih_audit_log_trace(log, "ih", "ih_container_array_create");
  }

  return solutions;
}

ih_box_system_t *ih_core_create_actor_box(void *search_system,
    ih_box_coordinate_t *dimension_coordinate,
    ih_container_array_t *initial_solutions, ih_core_actorey_t *actorey,
    ih_audit_log_t *log)
{
  assert(search_system);
  assert(dimension_coordinate);
  assert(actorey);
  assert(log);
  ih_box_system_t *box;

  box = ih_box_system_create(dimension_coordinate, actorey->get_box_cell,
      actorey->set_box_cell, actorey->compare, actorey->copy, actorey->destroy,
      log);
  if (box) {
    if (init_box_add_random_actors(box, dimension_coordinate,
            actorey->create_random, search_system, log)) {
      if (initial_solutions) {
        if (!init_box_add_actors_from_initial_solutions(box, actorey->create,
                search_system, initial_solutions, log)) {
          ih_box_system_destroy(box);
          box = NULL;
          ih_audit_log_trace
            (log, "ih", "init_box_add_actors_from_initial_solutions");
        }
      }
    } else {
      ih_box_system_destroy(box);
      box = NULL;
      ih_audit_log_trace(log, "ih", "init_box_add_random_actors");
    }
  } else {
    ih_audit_log_trace(log, "ih", "ih_box_system_create");
  }

  return box;
}

ih_core_bool_t init_box_add_random_actors(ih_box_system_t *box,
    ih_box_coordinate_t *dimension_coordinate,
    ih_core_create_actor_random_f create_actor_random, void *search_system,
    ih_audit_log_t *log)
{
  assert(box);
  assert(dimension_coordinate);
  assert(create_actor_random);
  assert(search_system);
  assert(log);
  ih_core_bool_t success;
  ih_box_coordinate_t coordinate;
  void *actor;

  success = ih_core_bool_true;

  for (coordinate.x = 0;
       coordinate.x < dimension_coordinate->x;
       coordinate.x++) {
    for (coordinate.y = 0;
         coordinate.y < dimension_coordinate->y;
         coordinate.y++) {
      for (coordinate.z = 0;
           coordinate.z < dimension_coordinate->z;
           coordinate.z++) {
        actor = create_actor_random(search_system);
        if (actor) {
          ih_box_system_add(box, &coordinate, actor);
        } else {
          success = ih_core_bool_false;
          ih_audit_log_trace(log, "cor3", "create_actor_random");
        }
      }
    }
  }

  return success;
}

/*  TODO: sort this array ourselves, by solution score  */
ih_core_bool_t init_box_add_actors_from_initial_solutions(ih_box_system_t *box,
    ih_core_actorey_create_f actor_create, void *search_system,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log)
{
  assert(box);
  assert(initial_solutions);
  assert(log);
  ih_core_bool_t success;
  ih_core_bitarray_t *bitarray;
  void *actor;

  success = ih_core_bool_true;

  ih_container_array_iterate_start(initial_solutions);
  while ((bitarray = ih_container_array_iterate_next(initial_solutions))) {
    actor = actor_create(search_system, bitarray);
    if (actor) {
      ih_box_system_replace_random(box, actor);
    } else {
      success = ih_core_bool_false;
      ih_audit_log_trace(log, "cor3", "actor_create");
    }
  }

  return success;
}

unsigned long ih_core_get_gene_start_address(ih_core_bitarray_t *solution,
    unsigned long gene_index)
{
  assert(solution);
  unsigned long address_of_gene_header;
  unsigned long start_address;

  address_of_gene_header = IH_CORE_SOLUTION_BIT_COUNT_2N * gene_index;
  start_address = ih_core_bitarray_get_unsigned_short(solution,
      address_of_gene_header) % IH_CORE_SOLUTION_BIT_COUNT;

  return start_address;
}
