#ifndef ih_search_algorithm_h
#define ih_search_algorithm_h

#include "ih/external/external.h"
#include "ih/core/bool.h"

/*!
 * identifier for a particular search algorithm.  used by the inhesion::search
 * system.
 */
enum ih_search_algorithm_t {
  IH_SEARCH_ALGORITHM_BIOS,
  IH_SEARCH_ALGORITHM_COR3,
  IH_SEARCH_ALGORITHM_EOS,
  IH_SEARCH_ALGORITHM_GENETIC,
};
typedef enum ih_search_algorithm_t ih_search_algorithm_t;

/*!
 * encode a string into a search algorithm identifier.
 *
 * \param string the string representing the search algorithm, to decode.  one
 *        of {"bios","cor3","eos","genetic"}.
 * \param algorithm pointer to an ih_search_algorithm_t to set.  if this
 *        function returns ih_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
ih_core_bool_t ih_search_algorithm_get_from_string(char *string,
    ih_search_algorithm_t *algorithm);

/*!
 * decode a search algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified search algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *ih_search_algorithm_get_string(ih_search_algorithm_t algorithm);

#endif
