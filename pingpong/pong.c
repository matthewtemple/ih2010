#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/pingpong/pong.h"

struct ih_pingpong_pong_t {
  char *string;
};

ih_core_bool_t ih_pingpong_pong_add_to_message(void *pong_object,
    ih_core_message_t *message)
{
  assert(pong_object);
  assert(message);
  ih_core_bool_t success;
  ih_pingpong_pong_t *pong;

  pong = pong_object;

  if (ih_core_message_add_string(message, pong->string)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_core_message_add_string");
  }

  return success;
}

ih_pingpong_pong_t *ih_pingpong_pong_create(char *string)
{
  assert(string);
  ih_pingpong_pong_t *pong;

  pong = malloc(sizeof *pong);
  if (pong) {
    pong->string = ih_core_string_copy(string);
    if (!pong->string) {
      ih_core_trace("ih_core_string_copy");
      free(pong);
      pong = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return pong;
}

ih_pingpong_pong_t *ih_pingpong_pong_create_from_message(ih_core_message_t *message)
{
  assert(message);
  char *string;
  ih_pingpong_pong_t *pong;

  string = ih_core_message_take_string(message);
  if (string) {
    pong = ih_pingpong_pong_create(string);
    if (!pong) {
      ih_core_trace("ih_pingpong_pong_create");
    }
    free(string);
  } else {
    ih_core_trace("ih_core_message_take_string");
    pong = NULL;
  }

  return pong;
}

void ih_pingpong_pong_destroy(ih_pingpong_pong_t *pong)
{
  assert(pong);

  free(pong->string);
  free(pong);
}
