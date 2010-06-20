#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/ping.h"

struct ih_net_ping_t {
  char *string;
};

ih_core_bool_t ih_net_ping_add_to_message(void *ping_object,
    ih_core_message_t *message)
{
  assert(ping_object);
  assert(message);
  ih_core_bool_t success;
  ih_net_ping_t *ping;

  ping = ping_object;

  success = ih_core_message_add_string(message, ping->string);

  return success;
}

ih_net_ping_t *ih_net_ping_create(const char *string)
{
  assert(string);
  ih_net_ping_t *ping;

  ping = malloc(sizeof *ping);
  if (ping) {
    ping->string = strdup(string);
    if (!ping->string) {
      ih_core_trace("strdup");
      free(ping);
      ping = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return ping;
}

ih_net_ping_t *ih_net_ping_create_from_message(ih_core_message_t *message)
{
  assert(message);
  ih_net_ping_t *ping;
  char *string;

  string = ih_core_message_take_string(message);
  if (string) {
    ping = ih_net_ping_create(string);
    if (!ping) {
      ih_core_trace("ih_net_ping_create");
    }
    free(string);
  } else {
    ih_core_trace("ih_core_message_take_string");
    ping = NULL;
  }

  return ping;
}

void ih_net_ping_destroy(ih_net_ping_t *ping)
{
  assert(ping);

  free(ping->string);
  free(ping);
}

char *ih_net_ping_get_string(ih_net_ping_t *ping)
{
  return ping->string;
}
