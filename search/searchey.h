#ifndef ih_search_searchey_h
#define ih_search_searchey_h

#include "ih/core/goal.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/search/tools.h"

typedef void * (*ih_search_searchey_create_f)
(ih_core_score_solution_f score_solution, ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_audit_log_t *log);

typedef void (*ih_search_searchey_destroy_f)(void *search_object);

typedef ih_container_array_t * (*ih_search_searchey_get_solutions_copy_f)
  (void *search_object, unsigned short max_solution_count);

typedef void (*ih_search_searchey_search_f)(void *search_object);

struct ih_search_searchey_t {
  ih_search_searchey_create_f create;
  ih_search_searchey_destroy_f destroy;
  ih_search_searchey_get_solutions_copy_f get_solutions_copy;
  ih_search_searchey_search_f search;
};
typedef struct ih_search_searchey_t ih_search_searchey_t;

void ih_search_searchey_init(ih_search_searchey_t *searchey,
    ih_search_searchey_create_f create, ih_search_searchey_destroy_f destroy,
    ih_search_searchey_get_solutions_copy_f get_solutions_copy,
    ih_search_searchey_search_f search);

#endif
