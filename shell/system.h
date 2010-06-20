#ifndef ih_shell_system_h
#define ih_shell_system_h

#include "ih/container/list.h"

struct ih_core_shell_t;
typedef struct ih_core_shell_t ih_core_shell_t;

void ih_core_shell_append_output(ih_core_shell_t *shell, char *output);

ih_core_shell_t *ih_core_shell_create();

void ih_core_shell_destroy(ih_core_shell_t *shell);

ih_core_bool_t ih_core_shell_start(ih_core_shell_t *shell);

ih_container_list_t *ih_core_shell_take_input(ih_core_shell_t *shell);

#endif
