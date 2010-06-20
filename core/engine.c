#include "ih/core/engine.h"
#include "ih/external/external.h"

static char *engine_names[IH_NET_ENGINE_TYPE_COUNT] = {
  "void",
  "test",
  "ping",
  "shell",
  "hyper",
};

char *ih_net_engine_get_name(unsigned long engine_id,
    ih_net_engine_get_name_f get_engine_name)
{
  char *engine_name;

  if (engine_id < IH_NET_ENGINE_TYPE_COUNT) {
    engine_name = engine_names[engine_id];
  } else {
    if (get_engine_name) {
      engine_name = get_engine_name(engine_id);
    } else {
      engine_name = "[unknown]";
    }
  }

  return engine_name;
}
