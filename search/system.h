#ifndef ih_search_system_h
#define ih_search_system_h

#include "ih/external/external.h"
#include "ih/search/algorithm.h"

struct ih_search_system_t;
typedef struct ih_search_system_t ih_search_system_t;

/*!
 * create an inhesion::search system.  the resulting pointer is a context
 * variable that other search methods need.  when you're finished using the
 * search, call ih_search_system_destroy() to release resources the search is
 * using.
 *
 * \param score_solution a function with the signature specified by
 *        ih_core_score_solution_f.  this will be used to direct the search
 *        through possibility space.
 * \param goal either IH_CORE_GOAL_MAXIMIZE_SCORE or
 *        IH_CORE_GOAL_MINIMIZE_SCORE, telling the search to find solutions
 *        that either maximize or minimize solutions' scores.
 * \param context pointer to a domain-specific context.  if your
 *        ih_core_score_solution_f needs to interact with your larger
 *        application context to do its work, pass in a pointer to that context
 *        here, or pass NULL otherwise.
 * \param initial_solutions array of solutions (each solution as an
 *        ih_core_bitarray_t pointer).  the search will be initialized with
 *        some of these solutions.  inhesion searches may or may not use all of
 *        the solutions you provide.  put solutions you favor earlier in the
 *        array.  inhesion's use of these solutions is stochastic...don't count
 *        on multiple identical search function calls producing identical
 *        results.
 * \param algorithm search algorithm to use
 * \param log pointer to an ih_audit_log_t where trace and log statements will
 *        be sent.
 *
 * \return pointer to an ih_search_system_t, or NULL if the call was
 *         unsuccessful.
 */
ih_search_system_t *ih_search_system_create
(ih_core_score_solution_f score_solution, ih_core_goal_t goal, void *context,
    ih_container_array_t *initial_solutions, ih_search_algorithm_t algorithm,
    ih_audit_log_t *log);

/*!
 * destroy an inhesion::search system.  release resources the search is using.
 *
 * \param system pointer to an inhesion::search system
 */
void ih_search_system_destroy(ih_search_system_t *system);

/*!
 * get copies of the solutions in the search.  this doesn't affect the contents
 * of the system.  when you want to get the solutions the search has found, to
 * apply to your domain problem, or to store for later use as initial solutions
 * in a search, call this function.  inhesion places better solutions earlier
 * in the returned array.
 *
 * \param system pointer to an inhesion::search system
 * \param max_solution_count maximum number of solutions to return.  in the
 *        demo version, the number of solutions returned is limited.  we know
 *        you can hack the library to make it fully functional.  we're not the
 *        karma police.  if you use this, license it.
 *
 * \return an array of ih_core_bitarray_t pointers
 */
ih_container_array_t *ih_search_system_get_solutions_copy
(ih_search_system_t *system, unsigned short max_solution_count);

/*!
 * search for solutions.  use the ih_core_score_solution_f passed to
 * ih_search_system_create() to guide the search.  call this method once, or
 * call it multiple times in a loop.  if called multiple times on the same
 * system, the search picks up where it left off each time you call
 * ih_search_system_search().
 *
 * \param system pointer to an inhesion::search system
 * \param max_wall_time_microseconds try to take no more than
 *        max_wall_time_microseconds to advance the search (there will be some
 *        minimum amount of time it takes for a call to this method to
 *        complete...above that, the max_wall_time_microseconds are used to
 *        pause the search).
 */
void ih_search_system_search(ih_search_system_t *system,
    unsigned long max_wall_time_microseconds);

#endif
