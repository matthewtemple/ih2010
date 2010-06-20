#ifndef ih_cor3_system_h
#define ih_cor3_system_h

#include "ih/cor3/declarations.h"
#include "ih/core/goal.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/search/searchey.h"
#include "ih/search/tools.h"

void *ih_cor3_system_create(ih_core_score_solution_f score_solution,
    ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log);

void ih_cor3_system_destroy(void *system_object);

ih_box_system_t *ih_cor3_system_get_box(ih_cor3_system_t *system);

void *ih_cor3_system_get_context(ih_cor3_system_t *system);

ih_core_goal_t ih_cor3_system_get_goal(ih_cor3_system_t *system);

ih_audit_log_t *ih_cor3_system_get_log(ih_cor3_system_t *system);

ih_core_score_solution_f ih_cor3_system_get_score_solution
(ih_cor3_system_t *system);

ih_container_array_t *ih_cor3_system_get_solutions_copy(void *system_object,
    unsigned short max_solution_count);

void ih_cor3_system_init_searchey(ih_search_searchey_t *searchey);

void ih_cor3_system_search(void *system_object);

#endif
