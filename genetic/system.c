#include "ih/audit/log.h"
#include "ih/container/array.h"
#include "ih/core/goal.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/genetic/system.h"
#include "ih/search/searchey.h"
#include "ih/search/tools.h"

struct ih_genetic_system_t {
};

void *ih_genetic_system_create(ih_core_score_solution_f score_solution,
    ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log)
{
  return NULL;
}

void ih_genetic_system_destroy(void *system_object)
{
}

ih_container_array_t *ih_genetic_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count)
{
  return NULL;
}

void ih_genetic_system_init_searchey(ih_search_searchey_t *searchey)
{
  ih_search_searchey_init(searchey, ih_genetic_system_create,
      ih_genetic_system_destroy, ih_genetic_system_get_solutions_copy,
      ih_genetic_system_search);
}

void ih_genetic_system_search(void *system_object)
{
}
