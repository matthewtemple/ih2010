#include "ih/external/external.h"
#include "ih/container/array.h"
#include "ih/cor3/actor.h"
#include "ih/cor3/constants.h"
#include "ih/cor3/system.h"
#include "ih/core/constants.h"
#include "ih/core/tools.h"

struct ih_cor3_system_t {
  ih_box_system_t *box;
  ih_core_score_solution_f score_solution;
  ih_core_goal_t goal;
  ih_core_actorey_t actorey;
  void *context;
  ih_audit_log_t *log;
};

void *ih_cor3_system_create(ih_core_score_solution_f score_solution,
    ih_core_goal_t goal, void *context, ih_container_array_t *initial_solutions,
    ih_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  ih_cor3_system_t *system;
  ih_box_coordinate_t dimension_coordinate;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->goal = goal;
    system->context = context;
    system->log = log;
    ih_core_seed_random_witih_time();
    ih_box_coordinate_init_witih_xyz(&dimension_coordinate,
        IH_COR3_BOX_DIMENSION, IH_COR3_BOX_DIMENSION, IH_COR3_BOX_DIMENSION);
    ih_cor3_actor_init_actorey(&system->actorey);
    system->box = ih_core_create_actor_box(system, &dimension_coordinate,
        initial_solutions, &system->actorey, log);
    if (!system->box) {
      ih_audit_log_trace(log, "cor3", "ih_core_create_actor_box");
      free(system);
      system = NULL;
    }
  } else {
    ih_audit_log_trace(log, "cor3", "malloc");
  }

  return system;
}

void ih_cor3_system_destroy(void *system_object)
{
  assert(system_object);
  ih_cor3_system_t *system;

  system = system_object;

  ih_box_system_destroy(system->box);
  free(system);
}

ih_box_system_t *ih_cor3_system_get_box(ih_cor3_system_t *system)
{
  return system->box;
}

void *ih_cor3_system_get_context(ih_cor3_system_t *system)
{
  return system->context;
}

ih_core_goal_t ih_cor3_system_get_goal(ih_cor3_system_t *system)
{
  return system->goal;
}

ih_audit_log_t *ih_cor3_system_get_log(ih_cor3_system_t *system)
{
  return system->log;
}

ih_core_score_solution_f ih_cor3_system_get_score_solution
(ih_cor3_system_t *system)
{
  return system->score_solution;
}

ih_container_array_t *ih_cor3_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  assert(system_object);
  ih_cor3_system_t *system;
  ih_container_array_t *solutions;

  system = system_object;

  solutions = ih_core_create_solutions_from_box(system->box,
      max_solution_count, ih_cor3_actor_get_solution, system->score_solution,
      system->goal, ih_cor3_actor_compare_maximize,
      ih_cor3_actor_compare_minimize, ih_cor3_actor_copy, system->context,
      system->log);
  if (!solutions) {
    ih_audit_log_trace(system->log, "cor3", "create_solutions_from_box");
  }

  return solutions;
}

void ih_cor3_system_init_searchey(ih_search_searchey_t *searchey)
{
  ih_search_searchey_init(searchey, ih_cor3_system_create,
      ih_cor3_system_destroy, ih_cor3_system_get_solutions_copy,
      ih_cor3_system_search);
}

void ih_cor3_system_search(void *system_object)
{
  assert(system_object);
  ih_cor3_system_t *system;
  ih_cor3_actor_t *actor;

  system = system_object;

  actor = ih_box_system_find_random(system->box);
  ih_cor3_actor_act(actor);
}
