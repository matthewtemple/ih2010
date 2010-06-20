#include "ih/core/run.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_core_run_t {
  ih_core_run_style_t run_style;

  ih_core_bool_t goal_reached;

  unsigned long iterations;
  unsigned long max_iterations;

  time_t start_time;
  unsigned long wall_time_expiration_seconds;
};

static void reset(ih_core_run_t *run);

void ih_core_run_consider_goal_reached(ih_core_run_t *run)
{
  run->goal_reached = ih_core_bool_true;
}

ih_core_run_t *ih_core_run_create(ih_core_run_style_t run_style)
{
  ih_core_run_t *run;

  run = malloc(sizeof *run);
  if (run) {
    run->run_style = run_style;
    reset(run);
  } else {
    ih_core_trace("malloc");
  }

  return run;
}

void ih_core_run_destroy(ih_core_run_t *run)
{
  assert(run);
  free(run);
}

void ih_core_run_increment_iterations(ih_core_run_t *run)
{
  run->iterations++;
}

ih_core_bool_t ih_core_run_is_complete(ih_core_run_t *run)
{
  assert(run);
  ih_core_bool_t is_complete;
  time_t current_time;

  switch (run->run_style) {

    case IH_CORE_RUN_STYLE_ITERATIONS:
      if (run->iterations >= run->max_iterations) {
        is_complete = ih_core_bool_true;
      } else {
        is_complete = ih_core_bool_false;
      }
      break;

    case IH_CORE_RUN_STYLE_WALL_TIME:
      time(&current_time);
      if ((current_time - run->start_time)
          >= run->wall_time_expiration_seconds) {
        is_complete = ih_core_bool_true;
      } else {
        is_complete = ih_core_bool_false;
      }
      break;

    case IH_CORE_RUN_STYLE_GOAL_REACHED:
      is_complete = run->goal_reached;
      break;

    case IH_CORE_RUN_STYLE_INFINITE:
      is_complete = ih_core_bool_false;
      break;

    case IH_CORE_RUN_STYLE_UNKNOWN:
    default:
      is_complete = ih_core_bool_false;
      break;
  }

  return is_complete;
}

void ih_core_run_reset(ih_core_run_t *run)
{
  reset(run);
}

void ih_core_run_set_max_iterations(ih_core_run_t *run,
    unsigned long max_iterations)
{
  run->max_iterations = max_iterations;
}

void ih_core_run_set_wall_time_expiration_seconds(ih_core_run_t *run,
    unsigned long seconds)
{
  run->wall_time_expiration_seconds = seconds;
}

void reset(ih_core_run_t *run)
{
  assert(run);

  run->goal_reached = ih_core_bool_false;
  run->iterations = 0;
  time(&run->start_time);
}
