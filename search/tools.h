#ifndef ih_search_tools_h
#define ih_search_tools_h

#include "ih/box/system.h"
#include "ih/container/array.h"
#include "ih/core/actorey.h"
#include "ih/core/goal.h"
#include "ih/external/external.h"

typedef void *(*ih_core_create_actor_f)(void *search_system,
    ih_core_bitarray_t *solution);

typedef void *(*ih_core_create_actor_random_f)(void *search_system);

typedef ih_core_bitarray_t *(*ih_core_get_solution_f)(void *object);

/*!
 * prototype for a score-solution function.  various inhesion systems search
 * for solutions to a problem.  this function scores a particular solution, to
 * direct the search.  define a function with this signature for use with some
 * inhesion systems.  systems that find solutions can either minimize or
 * maximize solution scores.
 *
 * \param context a pointer to your domain's custom context.  if you use some
 *        external context in the calculation of a solution's score, that
 *        context is available via this pointer.  you pass in the context to
 *        various inhesion system functions, then it is made available to your
 *        score-solution function as the value of this parameter.
 * \param solution the solution data.
 * \param score the solution's score.  implement your score_solution_f to set
 *        the value of the associated double, based on the solution data.
 *
 * \return success of the call
 */
typedef ih_core_bool_t (*ih_core_score_solution_f)(void *context,
    ih_core_bitarray_t *solution, double *score);

ih_box_system_t *ih_core_create_actor_box(void *search_system,
    ih_box_coordinate_t *dimension_coordinate,
    ih_container_array_t *initial_solutions, ih_core_actorey_t *actorey,
    ih_audit_log_t *log);

ih_container_array_t *ih_core_create_solutions_from_box(ih_box_system_t *box,
    unsigned long max_solution_count, ih_core_get_solution_f get_solution,
    ih_core_score_solution_f score_solution, ih_core_goal_t goal,
    ih_core_compare_f compare_maximize, ih_core_compare_f compare_minimize,
    ih_core_copy_f copy_object, void *context, ih_audit_log_t *log);

unsigned long ih_core_get_gene_start_address(ih_core_bitarray_t *solution,
    unsigned long gene_index);

#endif
