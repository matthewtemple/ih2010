#include "ih/core/core.h"
#include "ih/eos/actor.h"
#include "ih/eos/constants.h"
#include "ih/eos/system.h"
#include "ih/external/external.h"

struct ih_eos_system_t {
  ih_box_system_t *box;
  ih_core_score_solution_f score_solution;
  ih_core_goal_t goal;
  ih_core_actorey_t actorey;
  void *context;
  ih_audit_log_t *log;
};

void *ih_eos_system_create(ih_core_score_solution_f score_solution,
    ih_core_goal_t goal, void *context, ih_container_array_t *initial_solutions,
    ih_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  ih_eos_system_t *system;
  ih_box_coordinate_t dimension_coordinate;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->goal = goal;
    system->context = context;
    system->log = log;
    ih_core_seed_random_witih_time();
    ih_box_coordinate_init_witih_xyz(&dimension_coordinate,
        IH_EOS_BOX_DIMENSION, IH_EOS_BOX_DIMENSION, 1);
    ih_eos_actor_init_actorey(&system->actorey);
    system->box = ih_core_create_actor_box(system, &dimension_coordinate,
        initial_solutions, &system->actorey, log);
    if (!system->box) {
      ih_audit_log_trace(log, "bios", "ih_core_create_actor_box");
      free(system);
      system = NULL;
    }

  } else {
    ih_audit_log_trace(log, "eos", "malloc");
  }

  return system;
}

void ih_eos_system_destroy(void *system_object)
{
  assert(system_object);
  ih_eos_system_t *system;

  system = system_object;

  ih_box_system_destroy(system->box);
  free(system);
}

ih_box_system_t *ih_eos_system_get_box(ih_eos_system_t *system)
{
  return system->box;
}

void *ih_eos_system_get_context(ih_eos_system_t *system)
{
  return system->context;
}

ih_core_goal_t ih_eos_system_get_goal(ih_eos_system_t *system)
{
  return system->goal;
}

ih_audit_log_t *ih_eos_system_get_log(ih_eos_system_t *system)
{
  return system->log;
}

ih_core_score_solution_f ih_eos_system_get_score_solution
(ih_eos_system_t *system)
{
  return system->score_solution;
}

ih_container_array_t *ih_eos_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  assert(system_object);
  ih_eos_system_t *system;
  ih_container_array_t *solutions;

  system = system_object;

  solutions = ih_core_create_solutions_from_box(system->box,
      max_solution_count, ih_eos_actor_get_solution, system->score_solution,
      system->goal, ih_eos_actor_compare_maximize,
      ih_eos_actor_compare_minimize, ih_eos_actor_copy, system->context,
      system->log);
  if (!solutions) {
    ih_audit_log_trace(system->log, "eos", "create_solutions_from_box");
  }

  return solutions;
}

void ih_eos_system_init_searchey(ih_search_searchey_t *searchey)
{
  ih_search_searchey_init(searchey, ih_eos_system_create,
      ih_eos_system_destroy, ih_eos_system_get_solutions_copy,
      ih_eos_system_search);
}

void ih_eos_system_search(void *system_object)
{
  assert(system_object);
 ih_eos_system_t *system;
  ih_eos_actor_t *actor;
  unsigned long i;

  system = system_object;

  for (i = 0; i < 32; i++) {
    actor = ih_box_system_find_random(system->box);
    ih_eos_actor_act(actor);
  }
}
