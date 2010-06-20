#include "ih/core/buffer.h"
#include "ih/core/constants.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/shell/system.h"

#define INITIAL_IN_PROGRESS_INPUT ""

struct ih_core_shell_t {
  ih_container_list_t *new_input;
  pthread_mutex_t new_input_mutex;

  ih_core_buffer_t *in_progress_input;
  pthread_t shell_thread;
};

static void *shell_thread(void *shell_object);

void ih_core_shell_append_output(ih_core_shell_t *shell, char *output)
{
  assert(shell);
  assert(output);

  printf("%s", output);
}

ih_core_shell_t *ih_core_shell_create()
{
  ih_core_shell_t *shell;
  ih_core_bool_t so_far_so_good;

  shell = malloc(sizeof *shell);
  if (shell) {
    so_far_so_good = ih_core_bool_true;
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    shell->new_input = ih_container_list_create(ih_core_string_compare,
        ih_core_string_copy, IH_CORE_NO_DESTROY_FUNCTION);
    if (!shell->new_input) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (0 != pthread_mutex_init(&shell->new_input_mutex, NULL)) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    shell->in_progress_input = ih_core_buffer_create(INITIAL_IN_PROGRESS_INPUT,
        strlen(INITIAL_IN_PROGRESS_INPUT));
    if (!shell->in_progress_input) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("ih_core_buffer_create");
    }
  }

  if (!so_far_so_good && shell) {
    if (shell->new_input) {
      ih_container_list_destroy(shell->new_input);
    }
    if (shell->in_progress_input) {
      ih_core_buffer_destroy(shell->in_progress_input);
    }
    free(shell);
    shell = NULL;
  }

  return shell;
}

void ih_core_shell_destroy(ih_core_shell_t *shell)
{
  assert(shell);

  pthread_join(shell->shell_thread, NULL);
  ih_container_list_destroy(shell->new_input);
  pthread_mutex_destroy(&shell->new_input_mutex);
  ih_core_buffer_destroy(shell->in_progress_input);
  free(shell);

  ih_core_terminal_block();
}

ih_core_bool_t ih_core_shell_start(ih_core_shell_t *shell)
{
  assert(shell);
  ih_core_bool_t success;

  ih_core_terminal_nonblock();

  if (0 == pthread_create(&shell->shell_thread, NULL, shell_thread, shell)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_container_list_t *ih_core_shell_take_input(ih_core_shell_t *shell)
{
  assert(shell);
  ih_container_list_t *new_input;
  char *input_string;

  new_input = ih_container_list_create(ih_core_string_compare,
      ih_core_string_copy, ih_core_string_destroy);
  if (new_input) {

    pthread_mutex_lock(&shell->new_input_mutex);
    {
      ih_container_list_iterate_start(shell->new_input);
      while ((input_string
              = ih_container_list_iterate_next(shell->new_input))) {
        if (ih_container_list_add_last(new_input, input_string)) {
          ih_container_list_iterate_remove(shell->new_input);
        } else {
          ih_core_trace("ih_container_list_add_last");
        }
      }
    }
    pthread_mutex_unlock(&shell->new_input_mutex);

  } else {
    ih_core_trace("ih_container_list_create");
  }

  return new_input;
}

void *shell_thread(void *shell_object)
{
  assert(shell_object);
  ih_core_shell_t *shell;
  int c;
  char *input_string;
  unsigned long input_string_size;

  shell = shell_object;

  while (!ih_core_stop_requested) {
    if (ih_core_key_hit()) {
      c = getchar();
      if ('\n' == c) {
        input_string = ih_core_buffer_take_string(shell->in_progress_input,
            &input_string_size);
        if (input_string) {

          pthread_mutex_lock(&shell->new_input_mutex);
          {
            if (!ih_container_list_add_last(shell->new_input, input_string)) {
              ih_core_trace("ih_container_list_add_last");
            }
          }
          pthread_mutex_unlock(&shell->new_input_mutex);

        }
      } else {
        ih_core_buffer_append_char(shell->in_progress_input, c);
      }
    }
    usleep(IH_CORE_STANDARD_SLEEP_MICROSECONDS);
  }

  return NULL;
}
