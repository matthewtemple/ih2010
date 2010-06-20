#include "ih/core/long.h"
#include "ih/core/message.h"
#include "ih/core/short.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/server_stats.h"

ih_core_bool_t ih_net_server_add_stats_to_message
(ih_net_server_stats_t *server_stats, ih_core_message_t *message)
{
  assert(message);
  assert(server_stats);
  ih_core_bool_t success;

  success = ih_core_bool_true;

  if (!ih_core_long_add_to_message(&server_stats->engine_count, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->server_send_message_failure_count, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->engine_handled_message_count, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->engine_cant_handle_message_now_count, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->discarded_message_for_nonexistent_client_count,
          message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->discarded_message_for_unregistered_engine_count,
          message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message
      (&server_stats->discarded_message_engine_inbox_add_failed_count,
          message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message(&server_stats->uptime_seconds, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_long_add_to_message(&server_stats->connected_client_count,
          message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_short_add_to_message(&server_stats->max_threads, message)) {
    success = ih_core_bool_false;
  }

  if (!ih_core_short_add_to_message(&server_stats->thread_count, message)) {
    success = ih_core_bool_false;
  }

  return success;
}

ih_net_server_stats_t *ih_net_server_create_stats_from_message
(ih_core_message_t *message)
{
  assert(message);
  ih_net_server_stats_t *server_stats;

  server_stats = malloc(sizeof *server_stats);
  if (server_stats) {
    server_stats->engine_count = ih_core_message_take_long_value(message);

    server_stats->server_send_message_failure_count
      = ih_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_now_count
      = ih_core_message_take_long_value(message);

    server_stats->engine_handled_message_count
      = ih_core_message_take_long_value(message);

    server_stats->engine_cant_handle_message_count
      = ih_core_message_take_long_value(message);

    server_stats->discarded_message_for_nonexistent_client_count
      = ih_core_message_take_long_value(message);

    server_stats->discarded_message_for_unregistered_engine_count
      = ih_core_message_take_long_value(message);

    server_stats->discarded_message_engine_inbox_add_failed_count
      = ih_core_message_take_long_value(message);

    server_stats->uptime_seconds
      = ih_core_message_take_long_value (message);

    server_stats->connected_client_count
      = ih_core_message_take_long_value(message);

    server_stats->max_threads = ih_core_message_take_short_value(message);

    server_stats->thread_count = ih_core_message_take_short_value(message);
  } else {
    ih_core_trace("malloc");
  }

  return server_stats;
}

void ih_net_server_stats_init(ih_net_server_stats_t *server_stats)
{
  server_stats->server_send_message_failure_count = 0;
  server_stats->engine_cant_handle_message_now_count = 0;
  server_stats->engine_handled_message_count = 0;
  server_stats->engine_cant_handle_message_count = 0;
  server_stats->discarded_message_for_nonexistent_client_count = 0;
  server_stats->discarded_message_for_unregistered_engine_count = 0;
  server_stats->discarded_message_engine_inbox_add_failed_count = 0;
}
