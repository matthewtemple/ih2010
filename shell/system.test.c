#include "ih/container/list.h"
#include "ih/core/constants.h"
#include "ih/core/period.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/shell/system.h"

static void handle_signal(int signal);

void handle_signal(int signal)
{

  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      ih_core_stop_requested = ih_core_bool_true;
      break;
  }

}

int main(int argc, char *argv[])
{
  return 0;

  ih_core_shell_t *shell;
  ih_core_period_t *period;
  ih_container_list_t *input;
  char *command_string;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  shell = ih_core_shell_create();
  if (!shell) {
    ih_core_trace_exit("ih_core_shell_create");
  }

  if (!ih_core_shell_start(shell)) {
    ih_core_trace_exit("ih_core_shell_start");
  }

  period = ih_core_period_create(2);
  if (!period) {
    ih_core_trace_exit("ih_core_period_create");
  }

  while (!ih_core_stop_requested) {
    input = ih_core_shell_take_input(shell);
    if (input) {
      ih_container_list_iterate_start(input);
      while ((command_string = ih_container_list_iterate_next(input))) {
        printf(":%s:\n", command_string);
      }
      ih_container_list_destroy(input);
    }

    if (ih_core_bool_false && ih_core_period_once(period)) {
      ih_core_shell_append_output(shell, "ok\n");
    }

    usleep(IH_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  ih_core_period_destroy(period);
  ih_core_shell_destroy(shell);

  return 0;
}
