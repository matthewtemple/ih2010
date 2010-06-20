#include "ih/core/tools.h"
#include "ih/search/algorithm.h"

ih_core_bool_t ih_search_algorithm_get_from_string(char *string,
    ih_search_algorithm_t *algorithm)
{
  assert(string);
  assert(algorithm);
  ih_core_bool_t success;

  if (0 == strcmp(string, "bios")) {
    success = ih_core_bool_true;
    *algorithm = IH_SEARCH_ALGORITHM_BIOS;

  } else if (0 == strcmp(string, "cor3")) {
    success = ih_core_bool_true;
    *algorithm = IH_SEARCH_ALGORITHM_COR3;

  } else if (0 == strcmp(string, "eos")) {
    success = ih_core_bool_true;
    *algorithm = IH_SEARCH_ALGORITHM_EOS;

  } else if (0 == strcmp(string, "genetic")) {
    success = ih_core_bool_true;
    *algorithm = IH_SEARCH_ALGORITHM_GENETIC;

  } else {
    success = ih_core_bool_false;
  }

  return success;
}

char *ih_search_algorithm_get_string(ih_search_algorithm_t algorithm)
{
  char *string;

  switch (algorithm) {
    case IH_SEARCH_ALGORITHM_BIOS:
      string = "bios";
      break;
    case IH_SEARCH_ALGORITHM_COR3:
      string = "cor3";
      break;
    case IH_SEARCH_ALGORITHM_EOS:
      string = "eos";
      break;
    case IH_SEARCH_ALGORITHM_GENETIC:
      string = "genetic";
      break;
    default:
      string = NULL;
      break;
  }

  return string;
}
