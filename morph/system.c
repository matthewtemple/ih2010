#include "ih/core/tools.h"
#include "ih/morph/system.h"
#include "ih/search/system.h"

struct ih_morpih_system_t {
  ih_core_score_solution_f score_solution;
  ih_audit_log_t *log;
  ih_search_system_t *search;
};

ih_morpih_system_t *ih_morpih_system_create
(ih_core_score_solution_f score_solution, ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  ih_morpih_system_t *system;

  system = malloc(sizeof *system);
  if (system) {
    system->score_solution = score_solution;
    system->log = log;
    system->search = ih_search_system_create(score_solution, goal, context,
        initial_solutions, IH_SEARCH_ALGORITHM_COR3, log);
    if (!system->search) {
      ih_audit_log_trace(log, "mrph", "ih_search_system_create");
      free(system);
      system = NULL;
    }
  } else {
    ih_audit_log_trace(log, "mrph", "malloc");
  }

  return system;
}

void ih_morpih_system_destroy(ih_morpih_system_t *system)
{
  assert(system);

  ih_search_system_destroy(system->search);
  free(system);
}

ih_container_array_t *ih_morpih_system_get_solutions_copy
(ih_morpih_system_t *system, unsigned short max_solution_count)
{
  assert(system);

  return ih_search_system_get_solutions_copy
    (system->search, max_solution_count);
}

void ih_morpih_system_search(ih_morpih_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);

  return ih_search_system_search(system->search, max_wall_time_microseconds);
}
