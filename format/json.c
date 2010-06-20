#include "ih/core/tools.h"
#include "ih/format/json.h"

struct ih_format_json_t {
};

ih_format_json_t *ih_format_json_create()
{
  ih_format_json_t *json;

  json = malloc(sizeof *json);
  if (json) {

  } else {
    ih_core_trace("malloc");
  }

  return json;
}

void ih_format_json_destroy(ih_format_json_t *json)
{
  assert(json);
  free(json);
}
