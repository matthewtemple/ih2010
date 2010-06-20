#ifndef ih_classify_algorithm_h
#define ih_classify_algorithm_h

#include "ih/core/bool.h"
#include "ih/external/external.h"

/*!
 * identifier for a particular classify algorithm.  used by the
 * inhesion::classify system.
 */
enum ih_classify_algorithm_t {
  IH_CLASSIFY_ALGORITHM_ARCHETYPE,
};
typedef enum ih_classify_algorithm_t ih_classify_algorithm_t;

/*!
 * encode a string into a classify algorithm identifier.
 *
 * \param string the string representing the classify algorithm, to decode.
 *        one of {"archetype"}.
 * \param algorithm pointer to an ih_classify_algorithm_t to set.  if this
 *        function returns ih_core_bool_true, the algorithm pointed to by this
 *        parameter will be set to a valid algorithm.
 *
 * \return success or failure.
 */
ih_core_bool_t ih_classify_algorithm_get_from_string(char *string,
    ih_classify_algorithm_t *algorithm);

/*!
 * decode a classify algorithm identifier as a string.
 *
 * \param algorithm the algorithm you want represented as a string.
 *
 * \return a string representing the specified classify algorithm, or NULL if a
 *         bogus algorithm identifier was supplied.
 */
char *ih_classify_algorithm_get_string(ih_classify_algorithm_t algorithm);

#endif
