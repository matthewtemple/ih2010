#ifndef ih_eos_system_h
#define ih_eos_system_h

#include "ih/core/core.h"
#include "ih/eos/declarations.h"
#include "ih/external/external.h"
#include "ih/search/searchey.h"
#include "ih/search/tools.h"

void *ih_eos_system_create(ih_core_score_solution_f score_solution,
    ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log);

void ih_eos_system_destroy(void *system_object);

ih_box_system_t *ih_eos_system_get_box(ih_eos_system_t *system);

void *ih_eos_system_get_context(ih_eos_system_t *system);

ih_core_goal_t ih_eos_system_get_goal(ih_eos_system_t *system);

ih_audit_log_t *ih_eos_system_get_log(ih_eos_system_t *system);

ih_core_score_solution_f ih_eos_system_get_score_solution
(ih_eos_system_t *system);

ih_container_array_t *ih_eos_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void ih_eos_system_init_searchey(ih_search_searchey_t *searchey);

void ih_eos_system_search(void *system_object);

#endif
