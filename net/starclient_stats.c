#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/starclient_stats.h"

ih_core_bool_t ih_net_starclient_add_stats_to_message
(ih_net_starclient_stats_t *starclient_stats, ih_core_message_t *message)
{
  assert(message);
  assert(starclient_stats);
  ih_core_bool_t success;

  if (ih_core_message_add_long
      (message, &starclient_stats->connected_server_count)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_net_starclient_stats_t *ih_net_starclient_create_stats_from_message
(ih_core_message_t *message)
{
  assert(message);
  ih_net_starclient_stats_t *starclient_stats;

  starclient_stats = malloc(sizeof *starclient_stats);
  if (starclient_stats) {
    starclient_stats->connected_server_count
      = ih_core_message_take_long_value(message);
  } else {
    ih_core_trace("malloc");
  }

  return starclient_stats;
}
