#ifndef ih_config_tools_h
#define ih_config_tools_h

#include "ih/core/bool.h"
#include "ih/external/external.h"

void ih_config_disable_if_running_batcih_tests(int argc, char *argv[]);

ih_core_bool_t ih_config_running_batcih_tests(int argc, char *argv[]);

#endif
