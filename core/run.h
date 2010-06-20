#ifndef ih_core_run_h
#define ih_core_run_h

#include "ih/core/types.h"

enum ih_core_run_style_t {
  IH_CORE_RUN_STYLE_UNKNOWN,
  IH_CORE_RUN_STYLE_ITERATIONS,
  IH_CORE_RUN_STYLE_WALL_TIME,
  IH_CORE_RUN_STYLE_GOAL_REACHED,
  IH_CORE_RUN_STYLE_INFINITE
};
typedef enum ih_core_run_style_t ih_core_run_style_t;

struct ih_core_run_t;
typedef struct ih_core_run_t ih_core_run_t;

void ih_core_run_consider_goal_reached(ih_core_run_t *run);

ih_core_run_t *ih_core_run_create(ih_core_run_style_t run_style);

void ih_core_run_destroy(ih_core_run_t *run);

void ih_core_run_increment_iterations(ih_core_run_t *run);

ih_core_bool_t ih_core_run_is_complete(ih_core_run_t *run);

void ih_core_run_reset(ih_core_run_t *run);

void ih_core_run_set_max_iterations(ih_core_run_t *run,
    unsigned long max_iterations);

void ih_core_run_set_wall_time_expiration_seconds(ih_core_run_t *run,
    unsigned long seconds);

#endif
