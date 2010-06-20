#include "ih/core/tools.h"
#include "ih/bios/system.h"
#include "ih/cor3/system.h"
#include "ih/eos/system.h"
#include "ih/genetic/system.h"
#include "ih/search/searchey.h"
#include "ih/search/system.h"

struct ih_search_system_t {
  ih_search_searchey_t searchey;
  void *search_object;
};

static void init_searchey(ih_search_searchey_t *searchey,
    ih_search_algorithm_t algorithm);

ih_search_system_t *ih_search_system_create
(ih_core_score_solution_f score_solution, ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_search_algorithm_t algorithm,
    ih_audit_log_t *log)
{
  assert(score_solution);
  assert(log);
  ih_search_system_t *search;

  search = malloc(sizeof *search);
  if (search) {
    init_searchey(&search->searchey, algorithm);
    search->search_object = search->searchey.create(score_solution, goal,
        context, initial_solutions, log);
    if (!search->search_object) {
      ih_core_trace("create");
      free(search);
      search = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return search;
}

void ih_search_system_destroy(ih_search_system_t *system)
{
  assert(system);
  system->searchey.destroy(system->search_object);
  free(system);
}

ih_container_array_t *ih_search_system_get_solutions_copy
(ih_search_system_t *system, unsigned short max_solution_count)
{
  assert(system);

#ifdef IH_BUILD_DEMO
  max_solution_count = ih_core_min(max_solution_count,
      IH_CORE_DEMO_SOLUTION_COUNT);
#endif

  return system->searchey.get_solutions_copy(system->search_object,
      max_solution_count);
}

void ih_search_system_search(ih_search_system_t *system,
    unsigned long max_wall_time_microseconds)
{
  assert(system);
  struct timeval start_time;

  gettimeofday(&start_time, NULL);

  do {
    system->searchey.search(system->search_object);
  } while (ih_core_time_is_remaining_microseconds
      (&start_time, max_wall_time_microseconds));
}

void init_searchey(ih_search_searchey_t *searchey,
    ih_search_algorithm_t algorithm)
{
  assert(searchey);

  switch (algorithm) {
    default:
    case IH_SEARCH_ALGORITHM_BIOS:
      ih_bios_system_init_searchey(searchey);
      break;
    case IH_SEARCH_ALGORITHM_COR3:
      ih_cor3_system_init_searchey(searchey);
      break;
    case IH_SEARCH_ALGORITHM_EOS:
      ih_eos_system_init_searchey(searchey);
      break;
    case IH_SEARCH_ALGORITHM_GENETIC:
      ih_genetic_system_init_searchey(searchey);
      break;
  }
}
