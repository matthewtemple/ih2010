#ifndef ih_core_goal_h
#define ih_core_goal_h

#include "ih/external/external.h"

/*!
 * the goal of a search or optimization.  specifies whether the search should
 * work at minimizing or maximizing solutions' scores.
 */
enum ih_core_goal_t {
  IH_CORE_GOAL_MAXIMIZE_SCORE,
  IH_CORE_GOAL_MINIMIZE_SCORE
};
typedef enum ih_core_goal_t ih_core_goal_t;

#endif
