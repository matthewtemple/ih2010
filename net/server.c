#include "ih/container/list.h"
#include "ih/container/set.h"
#include "ih/core/constants.h"
#include "ih/core/engine.h"
#include "ih/core/message.h"
#include "ih/core/period.h"
#include "ih/core/short.h"
#include "ih/core/time.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/clientsocket.h"
#include "ih/net/engine_thread.h"
#include "ih/net/enginey.h"
#include "ih/net/exchange.h"
#include "ih/net/post.h"
#include "ih/net/server.h"
#include "ih/net/serversocket.h"

#define DEFAULT_UNRESPONSIVE_CLIENT_TIME_SECONDS 32
#define ENGINE_INITIAL_SLEEP_MICROSECONDS IH_CORE_STANDARD_SLEEP_MICROSECONDS
#define IH_NET_SERVER_SLEEP_MICROSECONDS IH_CORE_STANDARD_SLEEP_MICROSECONDS
#define MAX_THREADS_PER_ENGINE 8
#define PERFORMANCE_PERIOD_SECONDS 4

struct engine_container_t {
  ih_net_engine_id_t engine_id;
  ih_net_enginey_t *enginey;

  void *engine_object;
  ih_core_bool_t stop_requested;
  unsigned long sleep;

  ih_container_list_t *inbox;

  pthread_t threads[MAX_THREADS_PER_ENGINE];
  ih_core_bool_t run_threads_quiesce_requested[MAX_THREADS_PER_ENGINE];
  ih_core_bool_t run_threads_quiesce_completed[MAX_THREADS_PER_ENGINE];
  ih_container_list_t *thread_inboxes[MAX_THREADS_PER_ENGINE];
  pthread_mutex_t thread_inbox_mutexes[MAX_THREADS_PER_ENGINE];

  unsigned short min_run_threads;
  unsigned short max_run_threads;
  unsigned short run_thread_count;

  pthread_t maintain_thread;
  ih_net_maintain_t maintain_schedule;

  ih_net_server_handle_message_f *message_handlers;
  unsigned long message_handlers_size;

  double messages_per_second;
  double messages_per_second_thread[MAX_THREADS_PER_ENGINE];
  ih_core_period_t *performance_period;
  unsigned long messages_per_period;
  ih_core_bool_t reported_idle;
  unsigned long messages_per_period_thread[MAX_THREADS_PER_ENGINE];
};
typedef struct engine_container_t engine_container_t;

struct ih_net_server_t {
  char *name;
  int socket;
  unsigned short min_port;
  unsigned short max_port;

  unsigned short max_threads;
  unsigned short thread_count;

  ih_net_server_stats_t stats;

  pthread_t server_thread;
  pthread_t accept_thread;
  ih_core_bool_t accept_thread_created;

  ih_container_list_t *outbox;
  pthread_mutex_t outbox_mutex;

  ih_container_set_t *client_posts;
  ih_net_exchange_t *client_exchange;
  pthread_mutex_t client_posts_mutex;

  ih_container_list_t *engines;
  engine_container_t *engines_array[IH_NET_SERVER_MAX_ENGINES];

  void *custom_server_object;

  unsigned long unresponsive_client_time_seconds;

  ih_core_messagey_t *messagey;
  ih_net_postey_t *postey;
  ih_net_engine_get_name_f get_engine_name;

  time_t create_time;

  ih_audit_log_t *log;

  ih_config_system_t *config_system;
};

struct running_engine_t {
  ih_net_server_t *server;
  void *engine_object;
  ih_net_engine_id_t engine_id;

  ih_net_enginey_start_f start;
  ih_net_enginey_run_f run;
  ih_net_enginey_stop_f stop;

  unsigned short thread_index;
};
typedef struct running_engine_t running_engine_t;

struct maintaining_engine_t {
  ih_net_server_t *server;
  void *engine_object;
  ih_net_engine_id_t engine_id;

  ih_net_enginey_maintain_f maintain;
};
typedef struct maintaining_engine_t maintaining_engine_t;

static void *accept_thread(void *server_object);

static void close_disconnected_clients(ih_net_server_t *server);

static void close_unresponsive_clients(ih_net_server_t *server);

static engine_container_t *create_engine_container(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, void *custom_server_object,
    ih_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, ih_net_maintain_t maintain_schedule,
    unsigned long message_type_count);

static ih_core_bool_t create_engine_container_message_handlers
(engine_container_t *engine_container, unsigned long message_type_count,
    ih_audit_log_t *log);

static ih_core_bool_t create_engine_container_performance_period
(engine_container_t *engine_container, ih_audit_log_t *log);

static void create_engine_container_rollback
(engine_container_t *engine_container);

static void create_engine_container_stats
(engine_container_t *engine_container);

static ih_core_bool_t create_engine_container_threads
(engine_container_t *engine_container, ih_core_messagey_t *messagey,
    ih_audit_log_t *log);

static maintaining_engine_t *create_maintaining_engine(ih_net_server_t *server,
    void *engine_object, ih_net_engine_id_t engine_id,
    ih_net_enginey_maintain_f maintain);

static ih_core_bool_t create_post_for_new_client(ih_net_server_t *server,
    int client_socket);

static running_engine_t *create_running_engine(ih_net_server_t *server,
    void *engine_object, ih_net_engine_id_t engine_id,
    ih_net_enginey_start_f start, ih_net_enginey_run_f run,
    ih_net_enginey_stop_f stop, unsigned short thread_index);

static ih_core_bool_t create_thread(ih_net_server_t *server,
    pthread_t *pthread_address, thread_f thread_function, void *thread);

static void deliver_messages_to_engine(ih_net_server_t *server,
    engine_container_t *engine_container);

static unsigned long deliver_messages_to_engine_thread(ih_net_server_t *server,
    engine_container_t *engine_container, unsigned short thread_index,
    unsigned long target_number_of_messages_to_deliver);

static void deliver_messages_to_engines(ih_net_server_t *server);

static void destroy_engine_container(void *engine_container_object);

static void destroy_maintaining_engine
(maintaining_engine_t *maintaining_engine);

static void destroy_running_engine(running_engine_t *running_engine);

static unsigned long engine_count(ih_net_server_t *server);

static ih_core_bool_t engine_is_registered(ih_net_server_t *server,
    ih_net_engine_id_t engine_id);

static void engine_sleep(ih_net_server_t *server, ih_net_engine_id_t engine_id);

static void *find_client_post(ih_net_server_t *server,
    int client_socket);

static void handle_signal(int signal);

static ih_core_bool_t ih_net_server_create_client_exchange
(ih_net_server_t *server);

static ih_core_bool_t ih_net_server_create_client_posts(ih_net_server_t *server);

static ih_core_bool_t ih_net_server_create_engines(ih_net_server_t *server);

static ih_core_bool_t ih_net_server_create_outbox(ih_net_server_t *server);

static void ih_net_server_create_rollback(ih_net_server_t *server);

static ih_core_bool_t ih_net_server_register_engine_create
(engine_container_t *engine_container, ih_net_enginey_create_f create,
    ih_net_server_t *server, void *custom_server_object);

static ih_core_bool_t join_thread(ih_net_server_t *server, pthread_t thread);

static void *maintain_engine(void *maintaining_engine_object);

static void manage_threads(ih_net_server_t *server);

static void post_messages_to_clients(ih_net_server_t *server);

static void receive_messages_from_client_post(ih_net_server_t *server,
    void *post_object);

static void receive_messages_from_clients(ih_net_server_t *server);

static void *run_engine(void *running_engine_object);

static ih_core_bool_t serversocket_bind_listen(ih_net_server_t *server);

static void *server_thread(void *server_object);

static void stop(ih_net_server_t *server);

static ih_core_bool_t start(ih_net_server_t *server);

static ih_core_bool_t start_engine(ih_net_server_t *server,
    ih_net_engine_id_t engine_id);

static ih_core_bool_t start_engine_maintain_thread(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, engine_container_t *engine_container);

static ih_core_bool_t start_engine_run_thread(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, engine_container_t *engine_container,
    unsigned short thread_index);

static ih_core_bool_t threads_maxed_out(ih_net_server_t *server);

void *accept_thread(void *server_object)
{
  ih_net_server_t *server;
  struct sockaddr_in client_address;
  socklen_t client_address_size;
  int client_socket;

  server = server_object;

  while (!ih_core_stop_requested) {
    client_socket = ih_net_serversocket_accept
      (server->socket, &client_address, &client_address_size);
    if (client_socket >= 0) {
      if (!create_post_for_new_client(server, client_socket)) {
        ih_audit_log_trace(server->log, "hnet", "ih_net_serversocket_accept");
      }
    }
    usleep(IH_NET_SERVER_SLEEP_MICROSECONDS);
  }

  return NULL;
}

void close_disconnected_clients(ih_net_server_t *server)
{
  assert(ih_container_set_get_size(server->client_posts)
      == ih_net_exchange_get_post_count(server->client_exchange));

  void *client_post_object;
  ih_container_set_t *client_posts;
  int client_socket;
  ih_core_bool_t success;

  client_posts = server->client_posts;

  ih_container_set_iterate_start(client_posts);
  while ((client_post_object = ih_container_set_iterate_next(client_posts))) {
    if (server->postey->socket_closed(client_post_object)) {
      client_socket = server->postey->get_socket(client_post_object);
      ih_container_set_iterate_remove(client_posts);
      success = ih_core_bool_true;
      if (!ih_net_exchange_unregister_post
          (server->client_exchange, client_socket)) {
        ih_audit_log_trace(server->log, "hnet",
            "ih_net_exchange_unregister_post");
        success = ih_core_bool_false;
      }
      ih_net_clientsocket_destroy(client_socket);
    }
  }

  assert(ih_container_set_get_size(server->client_posts)
      == ih_net_exchange_get_post_count(server->client_exchange));
}

void close_unresponsive_clients(ih_net_server_t *server)
{
  assert(ih_container_set_get_size(server->client_posts)
      == ih_net_exchange_get_post_count(server->client_exchange));
  void *client_post_object;
  ih_container_set_t *client_posts;
  int client_socket;
  time_t current_time;
  time_t post_activity_time;

  client_posts = server->client_posts;
  current_time = time(NULL);

  ih_container_set_iterate_start(client_posts);
  while ((client_post_object = ih_container_set_iterate_next(client_posts))) {
    post_activity_time = server->postey->get_last_receive_activity_time
      (client_post_object);
    if (current_time - post_activity_time
        > server->unresponsive_client_time_seconds) {
      ih_container_set_iterate_remove(client_posts);
      client_socket = server->postey->get_socket(client_post_object);
      if (!ih_net_exchange_unregister_post(server->client_exchange,
              client_socket)) {
        ih_audit_log_trace(server->log, "hnet",
            "ih_net_exchange_unregister_post");
      }
      ih_net_clientsocket_destroy(client_socket);
      ih_audit_log_enter(server->log, "hnet",
          "server closed unresponsive client %u", client_socket);
    }
  }
}

engine_container_t *create_engine_container(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, void *custom_server_object,
    ih_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, ih_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  assert(server);
  engine_container_t *engine_container;
  ih_core_bool_t so_far_so_good;

  engine_container = malloc(sizeof *engine_container);
  if (engine_container) {
    engine_container->engine_id = engine_id;
    engine_container->enginey = enginey;
    engine_container->stop_requested = ih_core_bool_false;
    engine_container->sleep = ENGINE_INITIAL_SLEEP_MICROSECONDS;
    engine_container->maintain_schedule = maintain_schedule;
    engine_container->min_run_threads = min_run_threads;
    engine_container->max_run_threads = max_run_threads;
    engine_container->run_thread_count = 0;
    create_engine_container_stats(engine_container);
    so_far_so_good = ih_core_bool_true;
  } else {
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    engine_container->inbox = ih_container_list_create
      (IH_CORE_NO_COMPARE_FUNCTION, IH_CORE_NO_COPY_FUNCTION,
          IH_CORE_NO_DESTROY_FUNCTION);
    if (!engine_container->inbox) {
      ih_audit_log_trace(server->log, "hnet", "ih_container_list_create");
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_threads(engine_container,
        server->messagey,
        server->log);
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_message_handlers
      (engine_container, message_type_count, server->log);
  }

  if (so_far_so_good) {
    so_far_so_good = create_engine_container_performance_period
      (engine_container, server->log);
  }

  if (!so_far_so_good) {
    create_engine_container_rollback(engine_container);
    engine_container = NULL;
  }

  return engine_container;
}

ih_core_bool_t create_engine_container_message_handlers
(engine_container_t *engine_container, unsigned long message_type_count,
    ih_audit_log_t *log)
{
  assert(engine_container);
  assert(log);
  unsigned long each_handler;
  ih_core_bool_t success;

  engine_container->message_handlers_size = message_type_count;
  engine_container->message_handlers
    = malloc((sizeof *engine_container->message_handlers)
        * message_type_count);
  if (engine_container->message_handlers) {
    for (each_handler = 0; each_handler < message_type_count; each_handler++) {
      *(engine_container->message_handlers + each_handler) = NULL;
    }
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(log, "hnet", "malloc");
  }

  return success;
}

ih_core_bool_t create_engine_container_performance_period
(engine_container_t *engine_container, ih_audit_log_t *log)
{
  assert(engine_container);
  assert(log);
  ih_core_bool_t success;

  engine_container->performance_period
    = ih_core_period_create(PERFORMANCE_PERIOD_SECONDS);
  if (engine_container->performance_period) {
    success = ih_core_bool_true;
  } else {
    ih_audit_log_trace(log, "hnet", "new");
    success = ih_core_bool_false;
  }

  return success;
}

void create_engine_container_rollback(engine_container_t *engine_container)
{
  assert(engine_container);
  unsigned short each_thread;

  if (engine_container->engine_object) {
    engine_container->enginey->destroy(engine_container->engine_object);
  }
  if (engine_container->inbox) {
    ih_container_list_destroy(engine_container->inbox);
  }
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    if (engine_container->thread_inboxes[each_thread]) {
      ih_container_list_destroy(engine_container->thread_inboxes[each_thread]);
    }
  }
  if (engine_container->message_handlers) {
    free(engine_container->message_handlers);
  }
  if (engine_container->performance_period) {
    ih_core_period_destroy(engine_container->performance_period);
  }
  free(engine_container);
}

void create_engine_container_stats(engine_container_t *engine_container)
{
  assert(engine_container);
  unsigned short each_thread;

  engine_container->messages_per_second = 0.0;
  engine_container->messages_per_period = 0;
  engine_container->reported_idle = ih_core_bool_false;
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    *(engine_container->messages_per_second_thread + each_thread) = 0.0;
    *(engine_container->messages_per_period_thread + each_thread) = 0;
  }
}

ih_core_bool_t create_engine_container_threads
(engine_container_t *engine_container, ih_core_messagey_t *messagey,
    ih_audit_log_t *log)
{
  assert(engine_container);
  assert(messagey);
  assert(log);
  unsigned short each_thread;
  pthread_mutex_t *each_mutex;
  ih_core_bool_t success;

  success = ih_core_bool_true;

  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {

    *(engine_container->run_threads_quiesce_requested
        + each_thread) = ih_core_bool_false;
    *(engine_container->run_threads_quiesce_completed
        + each_thread) = ih_core_bool_false;

    *(engine_container->thread_inboxes + each_thread)
      = ih_container_list_create(IH_CORE_NO_COMPARE_FUNCTION,
          IH_CORE_NO_COPY_FUNCTION, messagey->destroy);
    if (!*(engine_container->thread_inboxes + each_thread)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(log, "hnet", "ih_container_list_create");
      break;
    }

    each_mutex = &engine_container->thread_inbox_mutexes[each_thread];
    if (0 != pthread_mutex_init(each_mutex, NULL)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(log, "hnet", "pthread_mutex_init");
      break;
    }
  }

  return success;
}

maintaining_engine_t *create_maintaining_engine(ih_net_server_t *server,
    void *engine_object, ih_net_engine_id_t engine_id,
    ih_net_enginey_maintain_f maintain)
{
  maintaining_engine_t *maintaining_engine;

  maintaining_engine = malloc(sizeof *maintaining_engine);
  if (maintaining_engine) {
    maintaining_engine->server = server;
    maintaining_engine->engine_object = engine_object;
    maintaining_engine->engine_id = engine_id;
    maintaining_engine->maintain = maintain;
  } else {
    ih_audit_log_trace(server->log, "hnet", "malloc");
  }

  return maintaining_engine;
}

ih_core_bool_t create_post_for_new_client(ih_net_server_t *server,
    int client_socket)
{
  assert(server);
  ih_core_bool_t success;
  void *post_object;

  post_object = server->postey->create(client_socket);

  if (post_object) {

    pthread_mutex_lock(&server->client_posts_mutex);
    {
      if (ih_container_set_add(server->client_posts, post_object)) {
        success = ih_core_bool_true;
        if (!ih_net_exchange_register_post
            (server->client_exchange, post_object)) {
          ih_audit_log_trace(server->log, "hnet",
              "ih_net_exchange_register_post");
          success = ih_core_bool_false;
        }
      } else {
        ih_audit_log_trace(server->log, "hnet", "ih_container_set_add");
        success = ih_core_bool_false;
      }
    }
    pthread_mutex_unlock(&server->client_posts_mutex);

    if (!success) {
      server->postey->destroy(post_object);
    }

  } else {
    ih_audit_log_trace(server->log, "hnet", "post_create");
    success = ih_core_bool_false;
  }

  return success;
}

running_engine_t *create_running_engine(ih_net_server_t *server,
    void *engine_object, ih_net_engine_id_t engine_id,
    ih_net_enginey_start_f start, ih_net_enginey_run_f run,
    ih_net_enginey_stop_f stop, unsigned short thread_index)
{
  running_engine_t *running_engine;

  running_engine = malloc(sizeof *running_engine);
  if (running_engine) {
    running_engine->server = server;
    running_engine->engine_object = engine_object;
    running_engine->engine_id = engine_id;
    running_engine->start = start;
    running_engine->run = run;
    running_engine->stop = stop;
    running_engine->thread_index = thread_index;
  }

  return running_engine;
}

ih_core_bool_t create_thread(ih_net_server_t *server, pthread_t *pthread_address,
    thread_f thread_function, void *thread)
{
  ih_core_bool_t success;

  success = ih_core_bool_false;

  if (!threads_maxed_out(server)) {
    if (0 == pthread_create(pthread_address, NULL, thread_function, thread)) {
      success = ih_core_bool_true;
      server->thread_count++;
    } else {
      ih_audit_log_trace(server->log, "hnet", "pthread_create");
    }
  } else {
    ih_audit_log_enter(server->log, "hnet", "threads are maxed out");
  }

  return success;
}

/*
  TODO: compartmentalize the reporting part of this function
*/
void deliver_messages_to_engine(ih_net_server_t *server,
    engine_container_t *engine_container)
{
  assert(server);
  assert(engine_container);
  unsigned short each_thread;
  unsigned long inbox_size;
  unsigned long target_number_of_messages_to_deliver;
  unsigned long actual_messages_delivered;
  unsigned long actual_messages_delivered_thread;
  double messages_per_second_this_period;
  double messages_per_second_this_period_on_this_thread;
  char *string;
  char little_string[5 + 1];

  actual_messages_delivered = 0;

  if (ih_core_period_once(engine_container->performance_period)) {

    messages_per_second_this_period = engine_container->messages_per_period
      / ((double) PERFORMANCE_PERIOD_SECONDS);

    if (messages_per_second_this_period > 0.0) {
      engine_container->reported_idle = ih_core_bool_false;

      string = NULL;
      for (each_thread = 0; each_thread < engine_container->run_thread_count;
           each_thread++) {
        messages_per_second_this_period_on_this_thread
          = engine_container->messages_per_period_thread
          [each_thread] / ((double) PERFORMANCE_PERIOD_SECONDS);
        snprintf(little_string, 5 + 1, " %0.1f ",
            messages_per_second_this_period_on_this_thread);
        string = ih_core_string_append(string, little_string);
        if (!string) {
          ih_audit_log_trace(server->log, "hnet", "ih_core_string_append");
        }
      }
      ih_audit_log_enter(server->log, "hnet", "%0.1f mps (%s) %s engine",
          messages_per_second_this_period, string,
          ih_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name));
      free(string);

    } else if (!engine_container->reported_idle) {
      ih_audit_log_enter(server->log, "hnet", "%s engine is idle",
          ih_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name));
      engine_container->reported_idle = ih_core_bool_true;

    }

    engine_container->messages_per_period = 0;
    for (each_thread = 0; each_thread < engine_container->run_thread_count;
         each_thread++) {
      *(engine_container->messages_per_period_thread + each_thread) = 0;
    }
  }

  if (engine_container->run_thread_count > 0) {
    inbox_size = ih_container_list_get_size(engine_container->inbox);
    target_number_of_messages_to_deliver
      = inbox_size / engine_container->run_thread_count;
    if (0 == target_number_of_messages_to_deliver) {
      target_number_of_messages_to_deliver = 1;
    }

    for (each_thread = 0; each_thread < engine_container->run_thread_count;
         each_thread++) {
      actual_messages_delivered_thread = deliver_messages_to_engine_thread
        (server, engine_container, each_thread,
            target_number_of_messages_to_deliver);
      actual_messages_delivered += actual_messages_delivered_thread;
      engine_container->messages_per_period_thread[each_thread]
        += actual_messages_delivered_thread;
    }
    engine_container->messages_per_period += actual_messages_delivered;
  }
}

unsigned long deliver_messages_to_engine_thread(ih_net_server_t *server,
    engine_container_t *engine_container, unsigned short thread_index,
    unsigned long target_number_of_messages_to_deliver)
{
  assert(server);
  assert(engine_container);
  unsigned long messages_delivered;
  unsigned long each_message;
  ih_container_list_t *thread_inbox;
  pthread_mutex_t *thread_inbox_mutex;
  void *message;

  messages_delivered = 0;
  thread_inbox_mutex = &engine_container->thread_inbox_mutexes[thread_index];
  thread_inbox = engine_container->thread_inboxes[thread_index];

  if (0 == pthread_mutex_trylock(thread_inbox_mutex)) {
    for (each_message = 0; each_message < target_number_of_messages_to_deliver;
         each_message++) {
      if ((message = ih_container_list_find_first(engine_container->inbox))) {
        if (ih_container_list_add_last(thread_inbox, message)) {
          ih_container_list_remove_first(engine_container->inbox);
          messages_delivered++;
        } else {
          ih_audit_log_trace(server->log, "hnet", "list_add_last");
        }
      }
    }
    pthread_mutex_unlock(thread_inbox_mutex);
  }

  return messages_delivered;
}

void deliver_messages_to_engines(ih_net_server_t *server)
{
  assert(server);
  engine_container_t *engine_container;

  ih_container_list_iterate_start(server->engines);
  while ((engine_container = ih_container_list_iterate_next(server->engines))) {
    deliver_messages_to_engine(server, engine_container);
  }
}

void destroy_engine_container(void *engine_container_object)
{
  assert(engine_container_object);
  engine_container_t *engine_container;
  unsigned short each_thread;

  engine_container = engine_container_object;

  engine_container->enginey->destroy(engine_container->engine_object);
  for (each_thread = 0; each_thread < MAX_THREADS_PER_ENGINE; each_thread++) {
    ih_container_list_destroy(engine_container->thread_inboxes[each_thread]);
  }
  ih_container_list_destroy(engine_container->inbox);
  free(engine_container->message_handlers);
  ih_core_period_destroy(engine_container->performance_period);

  free(engine_container);
}

void destroy_maintaining_engine(maintaining_engine_t *maintaining_engine)
{
  free(maintaining_engine);
}

void destroy_running_engine(running_engine_t *running_engine)
{
  free(running_engine);
}

unsigned long engine_count(ih_net_server_t *server)
{
  return ih_container_list_get_size(server->engines);
}

ih_core_bool_t engine_is_registered(ih_net_server_t *server,
    ih_net_engine_id_t engine_id)
{
  ih_core_bool_t is_registered;

  if (server->engines_array[engine_id]) {
    is_registered = ih_core_bool_true;
  } else {
    is_registered = ih_core_bool_false;
  }

  return is_registered;
}

void engine_sleep(ih_net_server_t *server, ih_net_engine_id_t engine_id)
{
  usleep(server->engines_array[engine_id]->sleep);
}

void *find_client_post(ih_net_server_t *server, int client_socket)
{
  void *client_post_object;
  void *decoy_post_object;

  decoy_post_object = server->postey->create_decoy(client_socket);
  if (decoy_post_object) {
    client_post_object
      = ih_container_set_find(server->client_posts, decoy_post_object);
    server->postey->destroy_decoy(decoy_post_object);
  } else {
    ih_audit_log_trace(server->log, "hnet", "postey->create_decoy");
    client_post_object = NULL;
  }

  return client_post_object;
}

void handle_signal(int signal)
{
  switch (signal) {
    case SIGUSR1:
      break;
    case SIGUSR2:
      break;
    default:
      ih_core_stop_requested = ih_core_bool_true;
      break;
  }
}

ih_net_server_t *ih_net_server_create(const char *name, unsigned short min_port,
    unsigned short max_port, unsigned short max_threads,
    ih_core_messagey_t *messagey, ih_net_postey_t *postey,
    ih_net_engine_get_name_f get_engine_name,
    ih_config_system_t *config_system, ih_audit_log_t *log)
{
  assert(name);
  assert(messagey);
  assert(postey);
  assert(log);
  ih_net_server_t *server;
  ih_core_bool_t so_far_so_good;

  server = malloc(sizeof *server);
  if (server) {
    server->min_port = min_port;
    server->max_port = max_port;
    server->max_threads = max_threads;
    server->thread_count = 0;
    server->unresponsive_client_time_seconds
      = DEFAULT_UNRESPONSIVE_CLIENT_TIME_SECONDS;
    ih_net_server_stats_init(&server->stats);
    server->custom_server_object = NULL;
    server->messagey = messagey;
    server->postey = postey;
    server->get_engine_name = get_engine_name;
    server->create_time = time(NULL);
    server->log = log;
    server->config_system = config_system;
    server->accept_thread_created = ih_core_bool_false;
    so_far_so_good = ih_core_bool_true;
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_audit_log_trace(server->log, "hnet", "malloc");
  }

  if (so_far_so_good) {
    server->name = strdup(name);
    if (!server->name) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_server_create_outbox(server);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_server_create_client_posts(server);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_server_create_client_exchange(server);
  }

  if (so_far_so_good) {
    so_far_so_good = ih_net_server_create_engines(server);
  }

  if (!so_far_so_good) {
    ih_net_server_create_rollback(server);
    server = NULL;
  }

  return server;
}

ih_core_bool_t ih_net_server_create_client_exchange(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t success;

  server->client_exchange = ih_net_exchange_create(server->postey);
  if (server->client_exchange) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(server->log, "hnet", "ih_net_exchange_create");
  }

  return success;
}

ih_core_bool_t ih_net_server_create_client_posts(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t success;

  server->client_posts = ih_container_set_create(server->postey->compare,
      IH_CORE_NO_COPY_FUNCTION, server->postey->destroy);
  if (server->client_posts) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(server->log, "hnet", "set_create");
  }

  if (success) {
    if (0 == pthread_mutex_init(&server->client_posts_mutex, NULL)) {
      success = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
      ih_audit_log_trace(server->log, "hnet", "pthread_mutex_init");
      ih_container_set_destroy(server->client_posts);
    }
  }

  return success;
}

ih_core_bool_t ih_net_server_create_engines(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t success;
  unsigned long each_engine_id;

  server->engines = ih_container_list_create(IH_CORE_NO_COMPARE_FUNCTION,
      IH_CORE_NO_COPY_FUNCTION, destroy_engine_container);
  if (server->engines) {
    for (each_engine_id = 0; each_engine_id < IH_NET_SERVER_MAX_ENGINES;
         each_engine_id++) {
      server->engines_array[each_engine_id] = NULL;
    }
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(server->log, "hnet", "ih_container_list_create");
  }

  return success;
}

ih_core_bool_t ih_net_server_create_outbox(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t success;

  server->outbox = ih_container_list_create(IH_CORE_NO_COMPARE_FUNCTION,
      IH_CORE_NO_COPY_FUNCTION, IH_CORE_NO_DESTROY_FUNCTION);
  if (server->outbox) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    ih_audit_log_trace(server->log, "hnet", "list_create");
  }

  if (success) {
    if (0 == pthread_mutex_init(&server->outbox_mutex, NULL)) {
      success = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
      ih_audit_log_trace(server->log, "hnet", "pthread_mutex_init");
      ih_container_list_destroy(server->outbox);
    }
  }

  return success;
}

void ih_net_server_create_rollback(ih_net_server_t *server)
{
  if (server) {
    if (server->name) {
      free(server->name);
    }
    if (server->outbox) {
      ih_container_list_destroy(server->outbox);
    }
    if (server->client_posts) {
      ih_container_set_destroy(server->client_posts);
    }
    if (server->client_exchange) {
      ih_net_exchange_destroy(server->client_exchange);
    }
    if (server->engines) {
      ih_container_list_destroy(server->engines);
    }
    free(server);
  }
}

void ih_net_server_destroy(ih_net_server_t *server)
{
  assert(server);
  void *message_object;

  join_thread(server, server->server_thread);

  ih_container_list_iterate_start(server->outbox);
  while ((message_object = ih_container_list_iterate_next(server->outbox))) {
    server->messagey->destroy(message_object);
  }
  ih_container_list_destroy(server->outbox);

  ih_container_set_destroy(server->client_posts);
  ih_net_exchange_destroy(server->client_exchange);
  ih_container_list_destroy(server->engines);
  free(server->name);
  free(server);
}

ih_core_bool_t ih_net_server_engine_run_thread_quiesce_requested
(ih_net_server_t *server, ih_net_engine_id_t engine_id,
    unsigned short thread_index)
{
  return server->engines_array[engine_id]
    ->run_threads_quiesce_requested[thread_index];
}

ih_net_server_handle_message_f ih_net_server_get_handler_for_message
(ih_net_server_t *server, void *message_object)
{
  assert(server);
  assert(message_object);
  engine_container_t *engine_container;
  ih_net_engine_id_t engine_id;
  unsigned long message_type;
  ih_net_server_handle_message_f handler;

  engine_id = server->messagey->get_engine_id(message_object);
  engine_container = server->engines_array[engine_id];
  message_type = server->messagey->get_type(message_object);

  if (message_type < engine_container->message_handlers_size) {
    handler = *(engine_container->message_handlers + message_type);
  } else {
    ih_audit_log_enter(server->log, "hnet",
        "server has no handler for %s engine, message %lu",
        ih_net_engine_get_name(engine_container->engine_id,
            server->get_engine_name),
        message_type);
    handler = NULL;
  }

  return handler;
}

ih_config_system_t *ih_net_server_get_config_system(ih_net_server_t *server)
{
  return server->config_system;
}

void ih_net_server_get_stats(ih_net_server_t *server,
    ih_net_server_stats_t *server_stats)
{
  assert(server_stats);

  server_stats->engine_count = engine_count(server);
  server_stats->server_send_message_failure_count
    = server->stats.server_send_message_failure_count;
  server_stats->engine_cant_handle_message_now_count
    = server->stats.engine_cant_handle_message_now_count;
  server_stats->engine_handled_message_count
    = server->stats.engine_handled_message_count;
  server_stats->engine_cant_handle_message_count
    = server->stats.engine_cant_handle_message_count;
  server_stats->discarded_message_for_nonexistent_client_count
    = server->stats.discarded_message_for_nonexistent_client_count;
  server_stats->discarded_message_for_unregistered_engine_count
    = server->stats.discarded_message_for_unregistered_engine_count;
  server_stats->discarded_message_engine_inbox_add_failed_count
    = server->stats.discarded_message_engine_inbox_add_failed_count;
  server_stats->uptime_seconds = time(NULL) - server->create_time;
  server_stats->connected_client_count = ih_container_set_get_size
    (server->client_posts);
  server_stats->max_threads = server->max_threads;
  server_stats->thread_count = server->thread_count;
}

void ih_net_server_print_stats(ih_net_server_t *server)
{
  ih_net_server_stats_t stats;
  ih_core_time_t *time;
  char *time_string;
  ih_core_days_hours_minutes_seconds_t days_hours_minutes_seconds;

  ih_net_server_get_stats(server, &stats);

  time = ih_core_time_create(stats.uptime_seconds);
  if (time) {
    ih_core_time_get_days_hours_minutes_seconds
      (time, &days_hours_minutes_seconds);
    time_string = create_string_from_days_hours_minutes_seconds
      (&days_hours_minutes_seconds);
    ih_core_time_destroy(time);
  } else {
    time_string = NULL;
  }

  if (time_string) {
    ih_audit_log_enter(server->log, "hnet", "%s server: %lu engines, up %s, "
        "handled %lu messages", server->name, stats.engine_count, time_string,
        stats.engine_handled_message_count);
  }

  if (stats.server_send_message_failure_count > 0) {
    ih_audit_log_enter(server->log, "hnet", "%s server_send_message() "
        "failures: %lu", server->name,
        stats.server_send_message_failure_count);
  }

  if (stats.discarded_message_for_unregistered_engine_count > 0) {
    ih_audit_log_enter(server->log, "hnet", "%s messages received for "
        "unregistered engines: %lu", server->name,
        stats.discarded_message_for_unregistered_engine_count);
  }

  if (stats.engine_cant_handle_message_count > 0) {
    ih_audit_log_enter(server->log, "hnet", "%s instances of engine can't "
        "handle message: %lu", server->name,
        stats.engine_cant_handle_message_count);
  }

  if (stats.engine_cant_handle_message_now_count > 0) {
    ih_audit_log_enter(server->log, "hnet", "%s instances of engine can't "
        "handle message now: %lu", server->name,
        stats.engine_cant_handle_message_now_count);
  }

  if (time_string) {
    free(time_string);
  }
}

void ih_net_server_process_messages(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, unsigned short thread_index)
{
  assert(server);
  ih_container_list_t *inbox;
  pthread_mutex_t *inbox_mutex;
  ih_net_message_status_t message_status;
  void *message_object;
  ih_net_server_handle_message_f handler;
  unsigned long message_type;
  engine_container_t *engine_container;

  engine_container = server->engines_array[engine_id];
  inbox = engine_container->thread_inboxes[thread_index];
  inbox_mutex = &engine_container->thread_inbox_mutexes[thread_index];

  pthread_mutex_lock(inbox_mutex);
  ih_container_list_iterate_start(inbox);
  while ((message_object = ih_container_list_iterate_next(inbox))) {
    handler = engine_container->enginey->get_handler_for_message
      (engine_container->engine_object, message_object);
    if (handler) {
      message_status
        = (*handler)(engine_container->engine_object, message_object);
      switch (message_status) {
        case IH_NET_MESSAGE_STATUS_HANDLED:
          server->stats.engine_handled_message_count++;
          ih_container_list_iterate_remove(inbox);
          break;
        case IH_NET_MESSAGE_STATUS_CANT_HANDLE:
          server->stats.engine_cant_handle_message_count++;
          ih_container_list_iterate_remove(inbox);
          break;
        case IH_NET_MESSAGE_STATUS_CANT_HANDLE_NOW:
          server->stats.engine_cant_handle_message_now_count++;
          break;
        default:
          break;
      }
    } else {
      message_type = server->messagey->get_type(message_object);
      ih_audit_log_enter(server->log, "hnet",
          "server has no handler for %s engine's message %lu",
          ih_net_engine_get_name(engine_container->engine_id,
              server->get_engine_name),
          message_type);
      ih_container_list_iterate_remove(inbox);
    }
  }
  pthread_mutex_unlock(inbox_mutex);
}

ih_core_bool_t ih_net_server_register_engine(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, void *custom_server_object,
    ih_net_enginey_t *enginey, unsigned short min_run_threads,
    unsigned short max_run_threads, ih_net_maintain_t maintain_schedule,
    unsigned long message_type_count)
{
  assert(min_run_threads > 0);
  assert(max_run_threads >= min_run_threads);
  assert(max_run_threads <= MAX_THREADS_PER_ENGINE);
  assert(!server->engines_array[engine_id]);
  engine_container_t *engine_container;
  ih_core_bool_t success;

  if (engine_is_registered(server, engine_id)) {
    success = ih_core_bool_false;
  } else {
    success = ih_core_bool_true;
  }

  if (success) {
    engine_container = create_engine_container(server, engine_id,
        custom_server_object, enginey, min_run_threads, max_run_threads,
        maintain_schedule, message_type_count);
    if (!engine_container) {
      success = ih_core_bool_false;
    }
  }

  if (success) {
    server->engines_array[engine_id] = engine_container;
    if (!ih_container_list_add_last(server->engines, engine_container)) {
      ih_audit_log_trace(server->log, "hnet", "ih_container_list_add_last");
      success = ih_core_bool_false;
    }
  }

  if (success) {
    success = ih_net_server_register_engine_create(engine_container,
        enginey->create, server, custom_server_object);
  }

  return success;
}

ih_core_bool_t ih_net_server_register_engine_create
(engine_container_t *engine_container, ih_net_enginey_create_f create,
    ih_net_server_t *server, void *custom_server_object)
{
  assert(engine_container);
  ih_core_bool_t success;

  engine_container->engine_object = create(server, custom_server_object);
  if (engine_container->engine_object) {
    success = ih_core_bool_true;
  } else {
    ih_audit_log_trace(server->log, "hnet", "create_engine_function");
    success = ih_core_bool_false;
  }

  return success;
}

void ih_net_server_register_message_handler(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, unsigned long message_type,
    ih_net_server_handle_message_f message_handler)
{
  assert(server);
  assert(server->engines_array[engine_id]);
  assert(message_type
      < server->engines_array[engine_id]->message_handlers_size);
  engine_container_t *engine_container;

  engine_container = server->engines_array[engine_id];
  *(engine_container->message_handlers + message_type) = message_handler;
}

ih_core_bool_t ih_net_server_send_message(ih_net_server_t *server,
    void *message_object)
{
  ih_core_bool_t success;

  pthread_mutex_lock(&server->outbox_mutex);
  if (ih_container_list_add_last(server->outbox, message_object)) {
    success = ih_core_bool_true;
  } else {
    success = ih_core_bool_false;
    server->stats.server_send_message_failure_count++;
  }
  pthread_mutex_unlock(&server->outbox_mutex);

  return success;
}

void ih_net_server_set_unresponsive_client_time_seconds(ih_net_server_t *server,
    unsigned long unresponsive_client_time_seconds)
{
  server->unresponsive_client_time_seconds = unresponsive_client_time_seconds;
}

ih_core_bool_t ih_net_server_start(ih_net_server_t *server)
{
  ih_core_bool_t success;

  ih_core_stop_requested = ih_core_bool_false;

  if (start(server)) {
    if (create_thread
        (server, &server->server_thread, server_thread, (void *) server)) {
      success = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
    }
  } else {
    success = ih_core_bool_false;
  }
  if (!success) {
    stop(server);
  }

  return success;
}

ih_container_list_t *ih_net_server_take_unprocessed_messages
(ih_net_server_t *server)
{
  return NULL;
}

ih_container_list_t *ih_net_server_take_unsent_messages(ih_net_server_t *server)
{
  return NULL;
}

ih_core_bool_t join_thread(ih_net_server_t *server, pthread_t thread)
{
  ih_core_bool_t success;

  if (0 == pthread_join(thread, NULL)) {
    success = ih_core_bool_true;
    server->thread_count--;
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

void *maintain_engine(void *maintaining_engine_object)
{
  maintaining_engine_t *maintaining_engine;
  void *engine_object;
  ih_net_engine_id_t engine_id;
  ih_net_server_t *server;
  engine_container_t *engine_container;
  ih_core_period_t *maintenance_period;

  maintaining_engine = maintaining_engine_object;
  engine_object = maintaining_engine->engine_object;
  engine_id = maintaining_engine->engine_id;
  server = maintaining_engine->server;
  engine_container = server->engines_array[engine_id];

  maintenance_period
    = ih_core_period_create(engine_container->maintain_schedule);
  if (maintenance_period) {
    while (!engine_container->stop_requested) {
      if (ih_core_period_once(maintenance_period)) {
        maintaining_engine->maintain(engine_object);
      }
      engine_sleep(server, engine_id);
    }
    ih_core_period_destroy(maintenance_period);
  } else {
    ih_audit_log_trace(server->log, "hnet", "new");
  }

  destroy_maintaining_engine(maintaining_engine);

  return NULL;
}

void manage_threads(ih_net_server_t *server)
{
}

void post_messages_to_clients(ih_net_server_t *server)
{
  assert(server);
  void *message_object;
  void *client_post_object;
  int client_socket;

  pthread_mutex_lock(&server->outbox_mutex);
  ih_container_list_iterate_start(server->outbox);
  while ((message_object = ih_container_list_iterate_next(server->outbox))) {
    client_socket = server->messagey->get_client_socket(message_object);
    client_post_object = find_client_post(server, client_socket);
    if (client_post_object) {
      if (server->postey->send_message(client_post_object, message_object)) {
        ih_container_list_iterate_remove(server->outbox);
      } else {
        ih_audit_log_trace(server->log, "hnet", "postey->send_message");
      }
    } else {
      ih_container_list_iterate_remove(server->outbox);
      server->stats.discarded_message_for_nonexistent_client_count++;
      server->messagey->destroy(message_object);
      ih_audit_log_enter(server->log, "hnet", "server "
          "post_messages_to_clients() discarded "
          "message for nonexistent client %i", client_socket);
    }
  }
  pthread_mutex_unlock(&server->outbox_mutex);
}

void receive_messages_from_client_post(ih_net_server_t *server,
    void *post_object)
{
  assert(server);
  assert(post_object);
  void *message_object;
  ih_net_engine_id_t engine_id;
  engine_container_t *engine_container;
  ih_container_list_t *engine_inbox;

  while ((message_object = server->postey->receive_message(post_object))) {
    engine_id = server->messagey->get_engine_id(message_object);
    if (engine_is_registered(server, engine_id)) {
      engine_container = server->engines_array[engine_id];
      engine_inbox = engine_container->inbox;
      if (!ih_container_list_add_last(engine_inbox, message_object)) {
        ih_audit_log_trace(server->log, "hnet", "ih_container_list_add_last");
        server->stats.discarded_message_engine_inbox_add_failed_count++;
        server->messagey->destroy(message_object);
      }
    } else {
      ih_audit_log_enter(server->log, "hnet",
          "server discarded message type %lu for %s "
          "engine because the engine is not registered",
          server->messagey->get_type(message_object),
          ih_net_engine_get_name(engine_id, server->get_engine_name));
      server->stats.discarded_message_for_unregistered_engine_count++;
      server->messagey->destroy(message_object);
    }
  }
}

void receive_messages_from_clients(ih_net_server_t *server)
{
  void *client_post_object;
  ih_container_set_t *client_posts;

  client_posts = server->client_posts;
  ih_container_set_iterate_start(client_posts);
  while ((client_post_object = ih_container_set_iterate_next(client_posts))) {
    receive_messages_from_client_post(server, client_post_object);
  }
}

void *run_engine(void *running_engine_object)
{
  running_engine_t *running_engine;
  void *engine_object;
  ih_net_engine_id_t engine_id;
  ih_net_server_t *server;
  engine_container_t *engine_container;
  ih_net_engine_thread_t *engine_thread;

  running_engine = running_engine_object;
  engine_object = running_engine->engine_object;
  engine_id = running_engine->engine_id;
  server = running_engine->server;
  engine_container = server->engines_array[engine_id];
  engine_thread = ih_net_engine_create_thread
    (engine_object, running_engine->thread_index);

  running_engine->start(engine_thread);
  while (!engine_container->stop_requested) {
    running_engine->run(engine_thread);
    engine_sleep(server, engine_id);
  }
  running_engine->stop(engine_thread);

  ih_net_engine_destroy_thread(engine_thread);
  destroy_running_engine(running_engine);

  return NULL;
}

ih_core_bool_t serversocket_bind_listen(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t success;
  unsigned short port;

  success = ih_core_bool_false;

  for (port = server->min_port;
       (port <= server->max_port) && (!success); port++) {
    server->socket = ih_net_serversocket_create(port);
    if (server->socket >= 0) {
      success = ih_core_bool_true;
      ih_audit_log_enter(server->log, "hnet", "listening on port %i", port);
    } else {
      ih_audit_log_enter(server->log, "hnet", "ih_net_serversocket_create");
    }
  }

  return success;
}

void *server_thread(void *server_object)
{
  ih_net_server_t *server;

  server = server_object;

  while (!ih_core_stop_requested) {

    pthread_mutex_lock(&server->client_posts_mutex);
    {
      ih_net_exchange_send_and_receive_messages(server->client_exchange);
      post_messages_to_clients(server);
      receive_messages_from_clients(server);
      close_disconnected_clients(server);
      if (server->unresponsive_client_time_seconds) {
        close_unresponsive_clients(server);
      }
    }
    pthread_mutex_unlock(&server->client_posts_mutex);

    deliver_messages_to_engines(server);

    manage_threads(server);

    usleep(IH_NET_SERVER_SLEEP_MICROSECONDS);

  }

  stop(server);

  return NULL;
}

ih_core_bool_t start(ih_net_server_t *server)
{
  engine_container_t *engine_container;
  ih_net_engine_id_t engine_id;
  ih_core_bool_t success;

  success = ih_core_bool_true;

  signal(SIGINT, handle_signal);
  signal(SIGUSR1, handle_signal);
  signal(SIGUSR2, handle_signal);
  signal(SIGPIPE, SIG_IGN);

  ih_container_list_iterate_start(server->engines);
  while ((engine_container = ih_container_list_iterate_next(server->engines))) {
    engine_id = engine_container->engine_id;
    if (!start_engine(server, engine_id)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(server->log, "hnet", "start_engine");
      break;
    }
  }

  if (success) {
    if (!serversocket_bind_listen(server)) {
      success = ih_core_bool_false;
      ih_audit_log_trace(server->log, "hnet", "serversocket_bind_listen");
    }
  }

  if (success) {
    if (create_thread(server, &server->accept_thread, accept_thread,
            (void *) server)) {
      server->accept_thread_created = ih_core_bool_true;
    } else {
      success = ih_core_bool_false;
      ih_audit_log_trace(server->log, "hnet", "create_thread");
    }
  }

  return success;
}

ih_core_bool_t start_engine(ih_net_server_t *server, ih_net_engine_id_t engine_id)
{
  ih_core_bool_t success;
  unsigned short each_thread;
  engine_container_t *engine_container;

  success = ih_core_bool_true;
  engine_container = server->engines_array[engine_id];

  for (each_thread = 0; each_thread < engine_container->min_run_threads;
       each_thread++) {
    success = start_engine_run_thread(server, engine_id, engine_container,
        each_thread);
    if (success) {
      engine_container->run_thread_count++;
    } else {
      break;
    }
  }
  if (success) {
    success = start_engine_maintain_thread(server, engine_id,
        engine_container);
  }

  return success;
}

ih_core_bool_t start_engine_maintain_thread(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, engine_container_t *engine_container)
{
  ih_core_bool_t success;
  pthread_t *pthread_address;
  maintaining_engine_t *maintaining_engine;

  pthread_address = &engine_container->maintain_thread;
  maintaining_engine = create_maintaining_engine(server,
      engine_container->engine_object, engine_id,
      engine_container->enginey->maintain);
  if (maintaining_engine) {
    success = create_thread(server, pthread_address, maintain_engine,
        maintaining_engine);
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t start_engine_run_thread(ih_net_server_t *server,
    ih_net_engine_id_t engine_id, engine_container_t *engine_container,
    unsigned short thread_index)
{
  ih_core_bool_t success;
  running_engine_t *running_engine;
  pthread_t *pthread_address;

  pthread_address = &engine_container->threads[thread_index];
  running_engine = create_running_engine(server,
      engine_container->engine_object, engine_id,
      engine_container->enginey->start, engine_container->enginey->run,
      engine_container->enginey->stop, thread_index);
  if (running_engine) {
    success
      = create_thread(server, pthread_address, run_engine, running_engine);
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

void stop(ih_net_server_t *server)
{
  unsigned short each_thread;
  unsigned short run_thread_count;
  engine_container_t *engine_container;

  if (server->accept_thread_created) {
    join_thread(server, server->accept_thread);
  }

  ih_net_serversocket_destroy(server->socket);

  ih_container_list_iterate_start(server->engines);
  while ((engine_container = ih_container_list_iterate_next(server->engines))) {
    engine_container->stop_requested = ih_core_bool_true;
  }

  ih_container_list_iterate_start(server->engines);
  while ((engine_container = ih_container_list_iterate_next(server->engines))) {
    if (engine_container->enginey->maintain) {
      join_thread(server, engine_container->maintain_thread);
    }
    run_thread_count = engine_container->run_thread_count;
    for (each_thread = 0; each_thread < run_thread_count; each_thread++) {
      join_thread(server, engine_container->threads[each_thread]);
    }
  }
}

ih_core_bool_t threads_maxed_out(ih_net_server_t *server)
{
  assert(server);
  ih_core_bool_t maxed_out;

  if (server->thread_count < server->max_threads) {
    maxed_out = ih_core_bool_false;
  } else {
    maxed_out = ih_core_bool_true;
  }

  return maxed_out;
}
