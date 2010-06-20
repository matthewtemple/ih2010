#include "ih/container/set.h"
#include "ih/core/nameobject.h"
#include "ih/core/period.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/client.h"
#include "ih/net/starclient.h"
#include "ih/net/types.h"

/*
  TODO: do we need to remove messages from the unsent queue if they've been
  there too long?
*/

#define DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE 1024
#define MAINTAIN_PERIOD 8
#define MAX_CLIENTS 1024

struct message_handler_info_t {
  ih_net_engine_id_t engine_id;
  unsigned long message_type;
  ih_net_client_handle_message_f message_handler;
};
typedef struct message_handler_info_t message_handler_info_t;

struct ih_net_starclient_t {
  ih_container_list_t *star_arm_ips;
  unsigned short star_arm_port_min;
  unsigned short star_arm_port_max;

  char *node_server_exclude_ip;
  unsigned short node_server_exclude_min_port;
  unsigned short node_server_exclude_max_port;

  ih_container_set_t *clients;
  ih_container_list_t *client_list;

  ih_net_maintain_t maintain_schedule;
  ih_core_bool_t stop_requested;
  ih_core_period_t *maintain_period;

  ih_container_list_t *unsent_messages;
  unsigned long unsent_messages_queue_size;

  unsigned long last_star_arm_count;
  ih_core_bool_t need_to_print_stats;

  long message_type_counts[IH_NET_ENGINE_TYPE_COUNT];
  ih_container_list_t *message_handler_info;

  void *custom_client_context;
  ih_audit_log_t *log;
};

static ih_core_bool_t client_connected(ih_net_starclient_t *starclient,
    char *client_name);

static char *create_client_name(char *server_ip, unsigned short server_port);

static void establisih_connection(ih_net_starclient_t *starclient,
    char *client_name, char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context);

static ih_core_bool_t exclude_ip_port_combination
(ih_net_starclient_t *starclient, char *ip, unsigned short port);

static ih_net_client_t *find_client(ih_net_starclient_t *starclient,
    char *client_name);

static ih_net_client_t *get_random_client(ih_net_starclient_t *starclient);

static ih_core_bool_t ih_net_starclient_create_client_list
(ih_net_starclient_t *starclient);

static ih_core_bool_t ih_net_starclient_create_clients
(ih_net_starclient_t *starclient);

static ih_core_bool_t ih_net_starclient_create_maintain_period
(ih_net_starclient_t *starclient);

static ih_core_bool_t ih_net_starclient_create_message_handler_info
(ih_net_starclient_t *starclient);

static ih_core_bool_t ih_net_starclient_create_unsent_messages
(ih_net_starclient_t *starclient);

static void ih_net_starclient_create_rollback(ih_net_starclient_t *starclient);

static void print_stats(ih_net_starclient_t *starclient);

static ih_core_bool_t put_messsage_in_unsent_queue
(ih_net_starclient_t *starclient, ih_core_message_t *message);

static void re_route_unsent_messages(ih_net_starclient_t *starclient);

static void rebuild_client_list(ih_net_starclient_t *starclient);

static ih_core_bool_t register_engines_witih_client
(ih_net_starclient_t *starclient, ih_net_client_t *client);

static void register_message_handlers_witih_client
(ih_net_starclient_t *starclient, ih_net_client_t *client);

static void take_unsent_messages(ih_net_starclient_t *starclient,
    ih_net_client_t *client);

ih_core_bool_t client_connected(ih_net_starclient_t *starclient,
    char *client_name)
{
  assert(starclient);
  assert(client_name);
  ih_core_bool_t connected;
  ih_net_client_t *client;
  ih_core_nameobject_t *nameclient_decoy;

  client = find_client(starclient, client_name);
  if (client) {
    if (ih_net_client_is_connected_to_server(client)) {
      connected = ih_core_bool_true;
    } else {
      connected = ih_core_bool_false;
      take_unsent_messages(starclient, client);
      nameclient_decoy = ih_core_nameobject_create_decoy(client_name);
      if (nameclient_decoy) {
        if (ih_container_set_remove(starclient->clients, nameclient_decoy)) {
          printf("[star] lost connection with %s\n", client_name);
          starclient->need_to_print_stats = ih_core_bool_true;
        } else {
          ih_core_trace("this should never happen");
        }
        ih_core_nameobject_destroy_decoy(nameclient_decoy);
      } else {
        ih_core_trace("ih_core_nameobject_create_decoy");
      }
    }
  } else {
    connected = ih_core_bool_false;
  }

  return connected;
}

char *create_client_name(char *server_ip, unsigned short server_port)
{
  assert(server_ip);
  char *client_name;
  unsigned short server_ip_size;
  unsigned short server_port_string_size;
  unsigned short client_name_size;
  char *server_port_string;

  client_name = NULL;

  server_ip_size = strlen(server_ip);
  server_port_string = malloc(6);
  if (server_port_string) {
    if (snprintf(server_port_string, 6, "%i", server_port) >= 0) {
      server_port_string_size = strlen(server_port_string);
      client_name_size = server_ip_size + 1 + server_port_string_size;
      client_name = malloc(client_name_size + 1);
      if (client_name) {
        memcpy(client_name, server_ip, server_ip_size);
        memcpy(client_name + server_ip_size, ":", 1);
        memcpy(client_name + server_ip_size + 1, server_port_string,
            server_port_string_size);
        *(client_name + client_name_size) = '\0';
      } else {
        ih_core_trace("malloc");
      }
    } else {
      ih_core_trace("snprintf");
    }
  } else {
    ih_core_trace("malloc");
  }
  free(server_port_string);

  return client_name;
}

void establisih_connection(ih_net_starclient_t *starclient, char *client_name,
    char *server_ip, unsigned short server_min_port,
    unsigned short server_max_port, void *custom_client_context)
{
  assert(starclient);
  assert(client_name);
  ih_core_bool_t established_connection;
  ih_net_client_t *client;
  ih_core_nameobject_t *nameclient;

  established_connection = ih_core_bool_false;

  client = ih_net_client_create(server_ip, server_min_port, server_max_port,
      IH_NET_ENGINE_NO_GET_NAME_FUNCTION, custom_client_context,
      starclient->log);
  if (client) {
    nameclient = ih_core_nameobject_create(client_name, client,
        IH_CORE_NO_COPY_FUNCTION, ih_net_client_destroy,
        ih_net_client_get_as_string);
    if (nameclient) {
      if (ih_container_set_add(starclient->clients, nameclient)) {
        if (register_engines_witih_client(starclient, client)) {
          register_message_handlers_witih_client(starclient, client);
          printf("[star] connect to %s\n", client_name);
          starclient->need_to_print_stats = ih_core_bool_true;
          established_connection = ih_core_bool_true;
        } else {
          ih_core_trace("register_engines_witih_client");
        }
      } else {
        ih_core_trace("ih_container_set_add");
        ih_core_nameobject_destroy(nameclient);
      }
    } else {
      ih_core_trace("ih_core_nameobject_create");
    }
  }
  if (!established_connection) {
    ih_net_client_destroy(client);
  }
}

ih_core_bool_t exclude_ip_port_combination(ih_net_starclient_t *starclient,
    char *ip, unsigned short port)
{
  assert(starclient);
  assert(ip);
  assert(port);
  ih_core_bool_t exclude;

  if (((port >= starclient->node_server_exclude_min_port)
          && (port <= starclient->node_server_exclude_max_port))
      && (0 == strcmp(ip, starclient->node_server_exclude_ip))) {
    exclude = ih_core_bool_true;
  } else {
    exclude = ih_core_bool_false;
  }

  return exclude;
}

ih_net_client_t *find_client(ih_net_starclient_t *starclient, char *client_name)
{
  assert(starclient);
  assert(client_name);
  ih_core_nameobject_t *nameclient_decoy;
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;

  nameclient_decoy = ih_core_nameobject_create_decoy(client_name);
  if (nameclient_decoy) {
    nameclient = ih_container_set_find(starclient->clients, nameclient_decoy);
    ih_core_nameobject_destroy_decoy(nameclient_decoy);
    if (nameclient) {
      client = ih_core_nameobject_get_object(nameclient);
    } else {
      client = NULL;
    }
  } else {
    ih_core_trace_exit("ih_core_nameobject_create_decoy");
    client = NULL;
  }

  return client;
}

ih_net_client_t *get_random_client(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;

  nameclient = ih_container_list_find_random(starclient->client_list);
  if (nameclient) {
    client = ih_core_nameobject_get_object(nameclient);
  } else {
    client = NULL;
  }

  return client;
}

ih_core_bool_t ih_net_starclient_connect(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t connected_to_at_least_one;
  char *arm_ip;
  unsigned short port;
  char *client_name;
  unsigned long star_arm_count;

  ih_container_list_iterate_start(starclient->star_arm_ips);
  while ((arm_ip = ih_container_list_iterate_next(starclient->star_arm_ips))) {
    for (port = starclient->star_arm_port_min;
         port <= starclient->star_arm_port_max; port++) {
      if (!exclude_ip_port_combination(starclient, arm_ip, port)) {
        client_name = create_client_name(arm_ip, port);
        if (!client_connected(starclient, client_name)) {
          establisih_connection(starclient, client_name, arm_ip, port, port,
              starclient->custom_client_context);
        }
        free(client_name);
        client_name = NULL;
      }
    }
  }

  star_arm_count = ih_container_set_get_size(starclient->clients);
  if (star_arm_count != starclient->last_star_arm_count) {
    starclient->need_to_print_stats = ih_core_bool_true;
  }

  if (ih_container_set_get_size(starclient->clients) > 0) {
    connected_to_at_least_one = ih_core_bool_true;
  } else {
    connected_to_at_least_one = ih_core_bool_false;
  }

  return connected_to_at_least_one;
}

ih_net_starclient_t *ih_net_starclient_create(ih_container_list_t *star_arm_ips,
    unsigned short star_arm_port_min, unsigned short star_arm_port_max,
    char *node_server_exclude_ip, unsigned short node_server_exclude_min_port,
    unsigned short node_server_exclude_max_port, void *custom_client_context,
    ih_audit_log_t *log)
{
  assert(star_arm_ips);
  assert(log);
  ih_net_starclient_t *starclient;
  ih_core_bool_t so_far_so_good;
  unsigned long engine_id;

  starclient = malloc(sizeof *starclient);
  if (starclient) {
    starclient->log = log;
    starclient->stop_requested = ih_core_bool_false;
    starclient->last_star_arm_count = 0;
    starclient->need_to_print_stats = ih_core_bool_false;
    starclient->star_arm_ips = star_arm_ips;
    starclient->star_arm_port_min = star_arm_port_min;
    starclient->star_arm_port_max = star_arm_port_max;
    starclient->node_server_exclude_ip = node_server_exclude_ip;
    starclient->node_server_exclude_min_port = node_server_exclude_min_port;
    starclient->node_server_exclude_max_port = node_server_exclude_max_port;
    starclient->custom_client_context = custom_client_context;
    starclient->unsent_messages_queue_size
      = DEFAULT_UNSENT_MESSAGES_QUEUE_SIZE;
    for (engine_id = 0; engine_id < IH_NET_ENGINE_TYPE_COUNT; engine_id++) {
      *(starclient->message_type_counts + engine_id) = -1;
    }
    if (ih_net_starclient_create_clients(starclient)) {
      so_far_so_good = ih_core_bool_true;
    } else {
      so_far_so_good = ih_core_bool_false;
    }
  } else {
    ih_core_trace("malloc");
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_starclient_create_maintain_period(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_starclient_create_unsent_messages(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_starclient_create_client_list(starclient);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_starclient_create_message_handler_info(starclient);
  }

  if (!so_far_so_good && starclient) {
    ih_net_starclient_create_rollback(starclient);
    starclient = NULL;
  }

  assert(!starclient
      || (
        starclient->clients && starclient->maintain_period
        && starclient->unsent_messages && starclient->client_list
        )
    );
  return starclient;
}

ih_core_bool_t ih_net_starclient_create_client_list
(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t success;

  starclient->client_list = ih_container_list_create(ih_core_nameobject_compare,
      ih_core_nameobject_copy, IH_CORE_NO_DESTROY_FUNCTION);
  if (starclient->client_list) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_starclient_create_clients(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t success;

  starclient->clients = ih_container_set_create(ih_core_nameobject_compare,
      ih_core_nameobject_copy, ih_core_nameobject_destroy);
  if (starclient->clients) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_starclient_create_maintain_period
(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t success;

  starclient->maintain_period = ih_core_period_create(MAINTAIN_PERIOD);
  if (starclient->maintain_period) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_core_trace("new");
  }

  return success;
}

ih_core_bool_t ih_net_starclient_create_message_handler_info
(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t success;

  starclient->message_handler_info = ih_container_list_create
    (IH_CORE_NO_COMPARE_FUNCTION, IH_CORE_NO_COPY_FUNCTION, free);
  if (starclient->message_handler_info) {
    success = ih_core_bool_true;
  } else {
    ih_core_trace("ih_container_list_create");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t ih_net_starclient_create_unsent_messages
(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t success;

  starclient->unsent_messages = ih_container_list_create
    (IH_CORE_NO_COMPARE_FUNCTION, ih_core_message_copy,
        IH_CORE_NO_DESTROY_FUNCTION);
  if (starclient->unsent_messages) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

void ih_net_starclient_create_rollback(ih_net_starclient_t *starclient)
{
  assert(starclient);

  if (starclient->clients) {
    ih_container_set_destroy(starclient->clients);
  }
  if (starclient->maintain_period) {
    ih_core_period_destroy(starclient->maintain_period);
  }
  if (starclient->unsent_messages) {
    ih_container_list_destroy(starclient->unsent_messages);
  }
  if (starclient->client_list) {
    ih_container_list_destroy(starclient->client_list);
  }
  if (starclient->message_handler_info) {
    ih_container_list_destroy(starclient->message_handler_info);
  }
  free(starclient);
}

void ih_net_starclient_destroy(ih_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned int unsent_message_count;

  unsent_message_count
    = ih_container_list_get_size(starclient->unsent_messages);

  if (unsent_message_count > 0) {
    printf("[star] %u messages in unsent queue were lost\n",
        unsent_message_count);
  }

  ih_container_set_destroy(starclient->clients);
  ih_core_period_destroy(starclient->maintain_period);
  ih_container_list_destroy(starclient->unsent_messages);
  ih_container_list_destroy(starclient->client_list);
  ih_container_list_destroy(starclient->message_handler_info);
  free(starclient);
}

ih_net_client_t *ih_net_starclient_get_client(ih_net_starclient_t *starclient,
    int socket)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;
  ih_core_bool_t found_it;
  int each_socket;

  found_it = ih_core_bool_false;
  client = NULL;

  ih_container_list_iterate_start(starclient->client_list);
  while (!found_it && (nameclient = ih_container_list_iterate_next
          (starclient->client_list))) {
    client = ih_core_nameobject_get_object(nameclient);
    each_socket = ih_net_client_get_socket(client);
    if (each_socket == socket) {
      found_it = ih_core_bool_true;
    }
  }

  return client;
}

void ih_net_starclient_get_stats(ih_net_starclient_t *starclient,
    ih_net_starclient_stats_t *starclient_stats)
{
  assert(starclient);
  assert(starclient_stats);

  starclient_stats->connected_server_count
    = ih_container_set_get_size(starclient->clients);
}

void ih_net_starclient_process_messages(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;

  ih_container_set_iterate_start(starclient->clients);
  while ((nameclient = ih_container_set_iterate_next(starclient->clients))) {
    client = ih_core_nameobject_get_object(nameclient);
    ih_net_client_process_messages(client);
  }

  if (ih_core_period_once(starclient->maintain_period)) {
    ih_net_starclient_connect(starclient);
    rebuild_client_list(starclient);
    re_route_unsent_messages(starclient);
    if (starclient->need_to_print_stats) {
      print_stats(starclient);
    }
  }
}

ih_core_bool_t ih_net_starclient_register_engine(ih_net_starclient_t *starclient,
    ih_net_engine_id_t engine_id, unsigned long message_type_count)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;
  ih_core_bool_t success;

  success = ih_core_bool_true;

  *(starclient->message_type_counts + engine_id) = message_type_count;

  ih_container_set_iterate_start(starclient->clients);
  while ((nameclient = ih_container_set_iterate_next(starclient->clients))) {
    client = ih_core_nameobject_get_object(nameclient);
    if (!ih_net_client_register_engine(client, engine_id, message_type_count)) {
      success = ih_core_bool_false;
    }
  }

  return success;
}

void ih_net_starclient_register_message_handler(ih_net_starclient_t *starclient,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_client_handle_message_f message_handler)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;
  message_handler_info_t *message_handler_info;

  message_handler_info = malloc(sizeof *message_handler_info);
  if (message_handler_info) {
    message_handler_info->engine_id = engine_id;
    message_handler_info->message_type = message_type;
    message_handler_info->message_handler = message_handler;
    if (!ih_container_list_add_last
        (starclient->message_handler_info, message_handler_info)) {
      ih_core_trace("ih_container_list_add_last");
      free(message_handler_info);
    }
  } else {
    ih_core_trace("malloc");
  }

  ih_container_set_iterate_start(starclient->clients);
  while ((nameclient = ih_container_set_iterate_next(starclient->clients))) {
    client = ih_core_nameobject_get_object(nameclient);
    ih_net_client_register_message_handler
      (client, engine_id, message_type, message_handler);
  }
}

ih_core_bool_t ih_net_starclient_send_message_to_any_arm
(ih_net_starclient_t *starclient, ih_core_message_t *message)
{
  assert(starclient);
  assert(message);
  ih_net_client_t *random_client;
  ih_core_bool_t success;
  ih_core_bool_t sent_to_client;

  random_client = get_random_client(starclient);
  if (random_client) {
    if (ih_net_client_send_message(random_client, message)) {
      sent_to_client = ih_core_bool_true;
    } else {
      sent_to_client = ih_core_bool_false;
    }
  } else {
    sent_to_client = ih_core_bool_false;
  }

  if (sent_to_client) {
    success = ih_core_bool_true;
  } else {
    if (put_messsage_in_unsent_queue(starclient, message)) {
      success = ih_core_bool_true;
    } else {
      printf("[star] unsent message queue is full, unable to send message\n");
      success = ih_core_bool_false;
    }
  }

  return success;
}

ih_core_bool_t ih_net_starclient_send_message_to_all_arms
(ih_net_starclient_t *starclient, ih_core_message_t *message)
{
  assert(starclient);
  assert(message);
  ih_core_bool_t success;
  ih_core_nameobject_t *nameclient;
  ih_net_client_t *client;
  ih_core_message_t *message_copy;

  success = ih_core_bool_true;

  ih_container_set_iterate_start(starclient->clients);
  while ((nameclient = ih_container_set_iterate_next(starclient->clients))) {
    client = ih_core_nameobject_get_object(nameclient);
    message_copy = ih_core_message_copy(message);
    if (message_copy) {
      if (!ih_net_client_send_message(client, message_copy)) {
        success = ih_core_bool_false;
        ih_core_message_destroy(message_copy);
      }
    } else {
      success = ih_core_bool_false;
      ih_core_trace("ih_core_message_copy");
    }
  }

  ih_core_message_destroy(message);

  return success;
}

void ih_net_starclient_set_unsent_messages_queue_size
(ih_net_starclient_t *starclient, unsigned long queue_size)
{
  starclient->unsent_messages_queue_size = queue_size;
}

ih_core_bool_t ih_net_starclient_star_available(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_bool_t available;
  unsigned long client_count;

  client_count = ih_container_set_get_size(starclient->clients);

  if (client_count > 0) {
    available = ih_core_bool_true;
  } else {
    available = ih_core_bool_false;
  }

  return available;
}

void print_stats(ih_net_starclient_t *starclient)
{
  assert(starclient);
  unsigned long star_arm_count;

  star_arm_count = ih_container_set_get_size(starclient->clients);

  printf("[star] connected to %lu arms of the star\n", star_arm_count);
  starclient->last_star_arm_count = star_arm_count;
  starclient->need_to_print_stats = ih_core_bool_false;
}

ih_core_bool_t put_messsage_in_unsent_queue(ih_net_starclient_t *starclient,
    ih_core_message_t *message)
{
  assert(starclient);
  assert(message);
  unsigned long messages_in_queue;
  ih_core_bool_t success;

  messages_in_queue = ih_container_list_get_size(starclient->unsent_messages);

  if (starclient->unsent_messages_queue_size > messages_in_queue) {
    if (ih_container_list_add_last(starclient->unsent_messages, message)) {
      success = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
    }
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

void re_route_unsent_messages(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_message_t *message;
  ih_container_list_t *unsent_messages;

  unsent_messages = starclient->unsent_messages;

  ih_container_list_iterate_start(unsent_messages);
  while ((message = ih_container_list_iterate_next(unsent_messages))) {
    ih_net_starclient_send_message_to_any_arm(starclient, message);
    ih_container_list_iterate_remove(unsent_messages);
  }
}

void rebuild_client_list(ih_net_starclient_t *starclient)
{
  assert(starclient);
  ih_core_nameobject_t *nameclient;

  ih_container_list_clear(starclient->client_list);
  ih_container_set_iterate_start(starclient->clients);
  while ((nameclient = ih_container_set_iterate_next(starclient->clients))) {
    ih_container_list_add_last(starclient->client_list, nameclient);
  }
}

ih_core_bool_t register_engines_witih_client(ih_net_starclient_t *starclient,
    ih_net_client_t *client)
{
  assert(starclient);
  assert(client);
  ih_core_bool_t success;
  unsigned long engine_id;
  long message_type_count_long;
  unsigned long message_type_count;

  success = ih_core_bool_true;

  for (engine_id = 0; engine_id < IH_NET_ENGINE_TYPE_COUNT; engine_id++) {
    message_type_count_long = *(starclient->message_type_counts + engine_id);
    if (-1 != message_type_count_long) {
      message_type_count = (unsigned long) message_type_count_long;
      if (!ih_net_client_register_engine
          (client, engine_id, message_type_count)) {
        ih_core_trace("ih_net_client_register_engine");
        success = ih_core_bool_false;
      }
    }
  }

  return success;
}

void register_message_handlers_witih_client(ih_net_starclient_t *starclient,
    ih_net_client_t *client)
{
  assert(starclient);
  assert(client);
  message_handler_info_t *message_handler_info;

  ih_container_list_iterate_start(starclient->message_handler_info);
  while ((message_handler_info
          = ih_container_list_iterate_next(starclient->message_handler_info))) {
    ih_net_client_register_message_handler(client,
        message_handler_info->engine_id, message_handler_info->message_type,
        message_handler_info->message_handler);
  }
}

void take_unsent_messages(ih_net_starclient_t *starclient,
    ih_net_client_t *client)
{
  assert(starclient);
  assert(client);
  ih_container_list_t *unsent_messages;
  ih_core_message_t *message;
  unsigned long discarded_message_count;

  discarded_message_count = 0;

  unsent_messages = ih_net_client_take_unsent_messages(client);
  ih_container_list_iterate_start(unsent_messages);
  while ((message = ih_container_list_iterate_next(unsent_messages))) {
    if (!put_messsage_in_unsent_queue(starclient, message)) {
      discarded_message_count++;
      ih_core_message_destroy(message);
    }
  }
  ih_container_list_destroy(unsent_messages);

  if (discarded_message_count > 0) {
    printf("[star] unsent message queue is full, unable to take %lu "
        "unsent messages from client\n", discarded_message_count);
  }
}
