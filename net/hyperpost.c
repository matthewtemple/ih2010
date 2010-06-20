#include "ih/container/list.h"
#include "ih/core/nameobject.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/net/hypermessage.h"
#include "ih/net/hyperpost.h"
#include "ih/net/socket.h"

#define IN_BUFFER_SIZE 4096
#define STATUS_STRING_OK "200 OK"
#define STATUS_STRING_NOT_FOUND "404 Not Found"

struct ih_net_hyperpost_t {
  int socket;

  ih_container_list_t *inbox;
  ih_container_list_t *outbox;

  char *out_buffer;
  unsigned long out_buffer_size;
  ih_core_bool_t currently_sending_out_buffer;
  unsigned long out_buffer_send_position;

  char in_buffer[IN_BUFFER_SIZE];
  ih_core_bool_t in_buffer_have_status_line;
  ih_core_bool_t in_buffer_have_headers;
  ih_core_bool_t in_buffer_expecting_body;
  ih_core_bool_t in_buffer_have_body;
  ih_core_bool_t in_buffer_have_complete_message;
  unsigned long in_buffer_receive_position;
  unsigned long in_buffer_parse_position;
  ih_net_hypermethod_t in_hypermethod;
  char *in_resource_path;
  ih_net_hyperversion_t in_hyperversion;
  ih_container_set_t *in_hyperheaders;
  char *in_body;

  time_t last_receive_activity_time;
  ih_core_bool_t socket_closed;

  ih_net_post_stats_t stats;
};

static char *get_header_lines(ih_net_hypermessage_t *hypermessage,
    unsigned short *header_lines_size);

static char *get_outgoing_status_line(ih_net_hypermessage_t *hypermessage,
    unsigned short *status_line_size);

static const char *get_outgoing_status_line_status_string
(ih_net_hyperstatus_t hyperstatus, unsigned short *status_string_size);

static void ih_net_hyperpost_create_rollback(ih_net_hyperpost_t *hyperpost);

static ih_net_hypermethod_t parse_hypermethod(char *hypermethod_string);

static ih_net_hyperversion_t parse_hyperversion(char *hyperversion_string);

static void parse_incoming_message(ih_net_hyperpost_t *hyperpost);

static void parse_incoming_message_status_line(ih_net_hyperpost_t *hyperpost);

static void parse_incoming_message_headers(ih_net_hyperpost_t *hyperpost);

static void parse_incoming_message_body(ih_net_hyperpost_t *hyperpost);

static ih_core_bool_t put_message_into_out_buffer(ih_net_hyperpost_t *hyperpost,
    ih_net_hypermessage_t *hypermessage);

static ih_core_bool_t put_received_message_in_inbox
(ih_net_hyperpost_t *hyperpost);

static ih_core_bool_t receive_messages(ih_net_hyperpost_t *hyperpost);

static void reset_for_next_receive(ih_net_hyperpost_t *hyperpost);

static void reset_for_next_send(ih_net_hyperpost_t *hyperpost);

static void send_messages_get_new_message(ih_net_hyperpost_t *hyperpost);

static ih_core_bool_t send_messages_send_current_message
(ih_net_hyperpost_t *hyperpost);

char *get_header_lines(ih_net_hypermessage_t *hypermessage,
    unsigned short *header_lines_size)
{
  assert(hypermessage);
  assert(header_lines_size);
  ih_container_set_t *hyperheaders;
  char *header_lines;
  ih_core_nameobject_t *header;
  char *header_name;
  char *header_value;
  unsigned short line_size;
  unsigned short header_lines_position;
  unsigned short header_name_size;
  unsigned short header_value_size;

  header_lines = NULL;
  header_lines_position = 0;
  *header_lines_size = 0;
  hyperheaders = ih_net_hypermessage_get_hyperheaders(hypermessage);

  ih_container_set_iterate_start(hyperheaders);

  while ((header = ih_container_set_iterate_next(hyperheaders))) {

    header_name = ih_core_nameobject_get_name(header);
    header_value = ih_core_nameobject_get_object(header);
    header_name_size = strlen(header_name);
    header_value_size = strlen(header_value);
    line_size = header_name_size + 2 + header_value_size;

    *header_lines_size += line_size + 2;
    header_lines = realloc(header_lines, *header_lines_size);
    if (!header_lines) {
      ih_core_trace("realloc");
      break;
    }

    memcpy(header_lines + header_lines_position, header_name,
        header_name_size);
    header_lines_position += header_name_size;

    memcpy(header_lines + header_lines_position, ": ", 2);
    header_lines_position += 2;

    memcpy(header_lines + header_lines_position, header_value,
        header_value_size);
    header_lines_position += header_value_size;

    memcpy(header_lines + header_lines_position, "\r\n", 2);
    header_lines_position += 2;

  }

  return header_lines;
}

char *get_outgoing_status_line(ih_net_hypermessage_t *hypermessage,
    unsigned short *status_line_size)
{
  assert(hypermessage);
  assert(status_line_size);
  char *status_line;
  const char *hyperversion_name;
  ih_net_hyperversion_t hyperversion;
  unsigned short hyperversion_name_size;
  const char *status_string;
  unsigned short status_string_size;
  ih_net_hyperstatus_t hyperstatus;

  hyperstatus = ih_net_hypermessage_get_hyperstatus(hypermessage);
  hyperversion = ih_net_hypermessage_get_hyperversion(hypermessage);
  hyperversion_name = ih_net_hyperversion_get_name(hyperversion);
  hyperversion_name_size = strlen(hyperversion_name);

  status_string = get_outgoing_status_line_status_string(hyperstatus,
      &status_string_size);

  *status_line_size = hyperversion_name_size + 1 + status_string_size + 2;
  status_line = malloc(*status_line_size);
  if (status_line) {
    memcpy(status_line, hyperversion_name, hyperversion_name_size);
    *(status_line + hyperversion_name_size) = ' ';
    memcpy(status_line + hyperversion_name_size + 1, status_string,
        status_string_size);
    memcpy(status_line + hyperversion_name_size + 1 + status_string_size,
        "\r\n", 2);
  } else {
    ih_core_trace("malloc");
  }

  return status_line;
}

const char *get_outgoing_status_line_status_string
(ih_net_hyperstatus_t hyperstatus, unsigned short *status_string_size)
{
  assert(status_string_size);
  const char *status_string;

  switch (hyperstatus) {

    case IH_NET_HYPERSTATUS_OK:
      status_string = STATUS_STRING_OK;
      *status_string_size = strlen(STATUS_STRING_OK);
      break;

    case IH_NET_HYPERSTATUS_NOT_FOUND:
      status_string = STATUS_STRING_NOT_FOUND;
      *status_string_size = strlen(STATUS_STRING_NOT_FOUND);
      break;

    default:
    case IH_NET_HYPERSTATUS_UNKNOWN:
      ih_core_trace("invalid hyperstatus");
      status_string = "";
      *status_string_size = 0;
      break;

  }

  assert(status_string);
  return status_string;
}

int ih_net_hyperpost_compare(void *hyperpost_object_a,
    void *hyperpost_object_b)
{
  assert(hyperpost_object_a);
  assert(hyperpost_object_b);
  ih_net_hyperpost_t *hyperpost_a;
  ih_net_hyperpost_t *hyperpost_b;
  int compare;

  hyperpost_a = hyperpost_object_a;
  hyperpost_b = hyperpost_object_b;

  if (hyperpost_a->socket < hyperpost_b->socket) {
    compare = -1;
  } else if (hyperpost_a->socket > hyperpost_b->socket) {
    compare = 1;
  } else {
    compare = 0;
  }

  return compare;
}

void *ih_net_hyperpost_create(int socket)
{
  ih_net_hyperpost_t *hyperpost;
  ih_core_bool_t so_far_so_good;

  hyperpost = malloc(sizeof *hyperpost);
  if (hyperpost) {
    hyperpost->socket = socket;
    hyperpost->last_receive_activity_time = time(NULL);
    hyperpost->socket_closed = ih_core_bool_false;

    hyperpost->in_buffer_have_status_line = ih_core_bool_false;
    hyperpost->in_buffer_have_headers = ih_core_bool_false;
    hyperpost->in_buffer_have_body = ih_core_bool_false;
    hyperpost->in_buffer_have_complete_message = ih_core_bool_false;
    hyperpost->in_buffer_receive_position = 0;
    hyperpost->in_buffer_parse_position = 0;
    hyperpost->in_resource_path = NULL;
    hyperpost->in_body = NULL;

    hyperpost->out_buffer = NULL;
    hyperpost->out_buffer_size = 0;
    hyperpost->currently_sending_out_buffer = ih_core_bool_false;
    hyperpost->out_buffer_send_position = 0;

    ih_net_post_stats_init(&hyperpost->stats);

    so_far_so_good = ih_core_bool_true;
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    hyperpost->in_hyperheaders = ih_container_set_create
      (ih_core_nameobject_compare, ih_core_nameobject_copy,
          ih_core_nameobject_destroy);
    if (!hyperpost->in_hyperheaders) {
      ih_core_trace("ih_container_set_create");
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    hyperpost->inbox = ih_container_list_create(IH_CORE_NO_COMPARE_FUNCTION,
        IH_CORE_NO_COPY_FUNCTION, IH_CORE_NO_DESTROY_FUNCTION);
    if (!hyperpost->inbox) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    hyperpost->outbox = ih_container_list_create(IH_CORE_NO_COMPARE_FUNCTION,
        IH_CORE_NO_COPY_FUNCTION, ih_net_hypermessage_destroy);
    if (!hyperpost->outbox) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (!so_far_so_good && hyperpost) {
    ih_net_hyperpost_create_rollback(hyperpost);
    hyperpost = NULL;
  }

  return hyperpost;
}

void *ih_net_hyperpost_create_decoy(int socket)
{
  ih_net_hyperpost_t *hyperpost;

  hyperpost = malloc(sizeof *hyperpost);
  if (hyperpost) {
    hyperpost->socket = socket;
  } else {
    ih_core_trace("malloc() failed\n");
  }

  return hyperpost;
}

void ih_net_hyperpost_create_rollback(ih_net_hyperpost_t *hyperpost)
{
  assert(hyperpost);

  if (hyperpost->inbox) {
    ih_container_list_destroy(hyperpost->inbox);
  }
  if (hyperpost->outbox) {
    ih_container_list_destroy(hyperpost->outbox);
  }
  free(hyperpost);
}

void ih_net_hyperpost_destroy(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;
  ih_net_hypermessage_t *hypermessage;

  hyperpost = hyperpost_object;

  ih_container_list_iterate_start(hyperpost->inbox);
  while ((hypermessage = ih_container_list_iterate_next(hyperpost->inbox))) {
    ih_net_hypermessage_destroy(hypermessage);
  }
  ih_container_list_destroy(hyperpost->inbox);

  ih_container_list_destroy(hyperpost->outbox);

  ih_container_set_destroy(hyperpost->in_hyperheaders);

  if (hyperpost->in_resource_path) {
    free(hyperpost->in_resource_path);
  }

  free(hyperpost);
}

void ih_net_hyperpost_destroy_decoy(void *hyperpost_object)
{
  free(hyperpost_object);
}

time_t ih_net_hyperpost_get_last_receive_activity_time
(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  return hyperpost->last_receive_activity_time;
}

int ih_net_hyperpost_get_socket(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  return hyperpost->socket;
}

void ih_net_hyperpost_get_stats(void *hyperpost_object,
    ih_net_post_stats_t *post_stats)
{
  assert(hyperpost_object);
  assert(post_stats);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  post_stats->messages_sent = hyperpost->stats.messages_sent;
  post_stats->messages_received = hyperpost->stats.messages_received;
  post_stats->messages_in_inbox = hyperpost->stats.messages_in_inbox;
  post_stats->messages_in_outbox = hyperpost->stats.messages_in_outbox;
  post_stats->most_messages_ever_in_inbox
    = hyperpost->stats.most_messages_ever_in_inbox;
  post_stats->most_messages_ever_in_outbox
    = hyperpost->stats.most_messages_ever_in_outbox;
  post_stats->send_message_failures = hyperpost->stats.send_message_failures;
  post_stats->receive_message_failures
    = hyperpost->stats.receive_message_failures;
  post_stats->messages_not_sent_due_to_socket_send_failures
    = hyperpost->stats.messages_not_sent_due_to_socket_send_failures;
}

ih_core_bool_t ih_net_hyperpost_is_socket_closed
(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  return hyperpost->socket_closed;
}

void *ih_net_hyperpost_receive_message
(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;
  ih_net_hypermessage_t *hypermessage;

  hyperpost = hyperpost_object;

  hypermessage = ih_container_list_find_first(hyperpost->inbox);
  if (hypermessage) {
    ih_container_list_remove_first(hyperpost->inbox);
  }

  return hypermessage;
}

void ih_net_hyperpost_receive_messages(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  hyperpost->last_receive_activity_time = time(NULL);

  if (receive_messages(hyperpost)) {
    reset_for_next_receive(hyperpost);
  }
}

ih_core_bool_t ih_net_hyperpost_send_message(void *hyperpost_object,
    void *hypermessage_object)
{
  assert(hyperpost_object);
  assert(hypermessage_object);
  ih_core_bool_t success;
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  success = ih_container_list_add_last(hyperpost->outbox, hypermessage_object);

  return success;
}

void ih_net_hyperpost_send_messages(void *hyperpost_object)
{
  assert(hyperpost_object);
  ih_net_hyperpost_t *hyperpost;

  hyperpost = hyperpost_object;

  if (!hyperpost->currently_sending_out_buffer) {
    send_messages_get_new_message(hyperpost);
  }
  if (hyperpost->currently_sending_out_buffer) {
    if (send_messages_send_current_message(hyperpost)) {
      reset_for_next_send(hyperpost);
    }
  }
}

ih_net_hypermethod_t parse_hypermethod(char *resource_patih_string)
{
  assert(resource_patih_string);
  ih_net_hypermethod_t hypermethod;

  if (0 == strcmp(resource_patih_string, "GET")) {
    hypermethod = IH_NET_HYPERMETHOD_GET;

  } else if (0 == strcmp(resource_patih_string, "HEAD")) {
    hypermethod = IH_NET_HYPERMETHOD_HEAD;

  } else if (0 == strcmp(resource_patih_string, "POST")) {
    hypermethod = IH_NET_HYPERMETHOD_POST;

  } else {
    hypermethod = IH_NET_HYPERMETHOD_UNKNOWN;

  }

  return hypermethod;
}

ih_net_hyperversion_t parse_hyperversion(char *hyperversion_string)
{
  assert(hyperversion_string);
  ih_net_hyperversion_t hyperversion;

  if (0 == strcmp(hyperversion_string, "HTTP/1.1")) {
    hyperversion = IH_NET_HYPERVERSION_1_1;

  } else if (0 == strcmp(hyperversion_string, "HTTP/1.0")) {
    hyperversion = IH_NET_HYPERVERSION_1_0;

  } else {
    hyperversion = IH_NET_HYPERVERSION_UNKNOWN;

  }

  return hyperversion;
}

void parse_incoming_message(ih_net_hyperpost_t *hyperpost)
{
  assert(hyperpost);
  assert(!hyperpost->in_buffer_have_complete_message);

  if (!hyperpost->in_buffer_have_status_line) {
    parse_incoming_message_status_line(hyperpost);
  }

  if (hyperpost->in_buffer_have_status_line
      && !hyperpost->in_buffer_have_headers) {
    parse_incoming_message_headers(hyperpost);
  }

  if (hyperpost->in_buffer_have_status_line
      && hyperpost->in_buffer_have_headers
      && hyperpost->in_buffer_expecting_body
      && !hyperpost->in_buffer_have_body) {
    parse_incoming_message_body(hyperpost);
  }
}

void parse_incoming_message_status_line(ih_net_hyperpost_t *hyperpost)
{
  assert(hyperpost);
  char *first_newline;
  char *remaining_message;
  unsigned short line_size;
  char *line;
  char *hypermethod_string;
  char *resource_patih_string;
  char *hyperversion_string;
  char *strtok_r_char;

  strtok_r_char = NULL;

  remaining_message = hyperpost->in_buffer
    + hyperpost->in_buffer_parse_position;

  first_newline = strstr(remaining_message, "\r\n");
  if (first_newline) {
    line_size = first_newline - hyperpost->in_buffer;
    line = malloc(line_size + 1);
    if (line) {
      memcpy(line, hyperpost->in_buffer, line_size);
      *(line + line_size) = '\0';

      hypermethod_string = strtok_r(line, " ", &strtok_r_char);
      resource_patih_string = strtok_r(NULL, " ", &strtok_r_char);
      hyperversion_string = strtok_r(NULL, " ", &strtok_r_char);

      if (hypermethod_string && resource_patih_string && hyperversion_string) {
        hyperpost->in_hypermethod = parse_hypermethod(hypermethod_string);
        hyperpost->in_resource_path = strdup(resource_patih_string);
        hyperpost->in_hyperversion = parse_hyperversion(hyperversion_string);
        hyperpost->in_buffer_have_status_line = ih_core_bool_true;
        hyperpost->in_buffer_parse_position += line_size + 2;
      } else {
        printf("hyperpost received malformed http status line from "
            "client %i\n", hyperpost->socket);
      }

      free(line);
    } else {
      ih_core_trace("malloc");
    }
  }
}

void parse_incoming_message_headers(ih_net_hyperpost_t *hyperpost)
{
  char *double_newline_char;
  char *start_char;
  char *line;
  char *name;
  char *value;
  char *line_context;
  char *nameobject_context;
  ih_core_nameobject_t *nameobject;

  start_char = hyperpost->in_buffer + hyperpost->in_buffer_parse_position;
  double_newline_char = strstr(start_char, "\r\n\r\n");
  if (double_newline_char) {
    hyperpost->in_buffer_expecting_body = ih_core_bool_false;
    line = strtok_r(start_char, "\r\n", &line_context);
    while (line) {
      name = strtok_r(line, ": ", &nameobject_context);
      value = strtok_r(NULL, ": ", &nameobject_context);
      if (name && value) {
        if ((0 == strcmp(name, "Content-Length"))
            || (0 == strcmp(name, "Transfer-Encoding"))) {
          hyperpost->in_buffer_expecting_body = ih_core_bool_true;
        }
        nameobject = ih_core_nameobject_create(name, value, ih_core_string_copy,
            ih_core_string_destroy, ih_core_string_get_as_string);
        if (nameobject) {
          if (!ih_container_set_add(hyperpost->in_hyperheaders, nameobject)) {
            ih_core_nameobject_destroy(nameobject);
          }
        }
      }
      line = strtok_r(NULL, "\r\n", &line_context);
    }

    if (!hyperpost->in_buffer_expecting_body) {
      hyperpost->in_buffer_have_complete_message = ih_core_bool_true;
    }

    hyperpost->in_buffer_have_headers = ih_core_bool_true;
    hyperpost->in_buffer_parse_position
      += (double_newline_char - start_char) + 2;
  }
}

void parse_incoming_message_body(ih_net_hyperpost_t *hyperpost)
{
  /*
    TODO: if that works, set hyperpost->in_buffer_have_complete_message
  */
}

ih_core_bool_t put_message_into_out_buffer(ih_net_hyperpost_t *hyperpost,
    ih_net_hypermessage_t *hypermessage)
{
  assert(hyperpost);
  assert(hypermessage);
  ih_core_bool_t success;
  unsigned long message_body_size;
  char *message_body;
  char *status_line;
  unsigned short status_line_size;
  char *headers;
  unsigned short headers_size;
  unsigned long message_size;
  unsigned long buffer_write_position;

  status_line = get_outgoing_status_line(hypermessage, &status_line_size);
  if (status_line) {
    headers = get_header_lines(hypermessage, &headers_size);

    message_body = ih_net_hypermessage_get_body(hypermessage);
    message_body_size = ih_net_hypermessage_get_body_size(hypermessage);

    message_size = status_line_size + headers_size + 2 + message_body_size;

    hyperpost->out_buffer = malloc(message_size);
    if (hyperpost->out_buffer) {
      success = ih_core_bool_true;
      buffer_write_position = 0;

      memcpy(hyperpost->out_buffer + buffer_write_position, status_line,
          status_line_size);
      buffer_write_position += status_line_size;

      memcpy(hyperpost->out_buffer + buffer_write_position, headers,
          headers_size);
      buffer_write_position += headers_size;
      memcpy(hyperpost->out_buffer + buffer_write_position, "\r\n", 2);
      buffer_write_position += 2;

      memcpy(hyperpost->out_buffer + buffer_write_position, message_body,
          message_body_size);

      hyperpost->out_buffer_size = message_size;
      hyperpost->out_buffer_send_position = 0;
    } else {
      ih_core_trace("malloc");
      success = ih_core_bool_false;
    }

    free(status_line);
    if (headers) {
      free(headers);
    }
  } else {
    ih_core_trace("get_outgoing_status_line");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t put_received_message_in_inbox(ih_net_hyperpost_t *hyperpost)
{
  ih_core_bool_t success;
  ih_net_hypermessage_t *hypermessage;

  hypermessage = ih_net_hypermessage_create(hyperpost->socket,
      hyperpost->in_hypermethod, IH_NET_HYPERSTATUS_UNKNOWN,
      hyperpost->in_resource_path, hyperpost->in_hyperversion,
      hyperpost->in_hyperheaders);
  if (hypermessage) {
    if (ih_container_list_add_last(hyperpost->inbox, hypermessage)) {
      success = ih_core_bool_true;
    } else {
      ih_core_trace("ih_container_list_add_last");
      success = ih_core_bool_false;
    }
  } else {
    ih_core_trace("ih_net_hypermessage_create");
    success = ih_core_bool_false;
  }

  return success;
}

ih_core_bool_t receive_messages(ih_net_hyperpost_t *hyperpost)
{
  assert(hyperpost);
  ih_core_bool_t received_complete_message;
  int actual_bytes_read;
  int max_bytes_to_read;

  received_complete_message = ih_core_bool_false;

  max_bytes_to_read = IN_BUFFER_SIZE - hyperpost->in_buffer_receive_position;
  actual_bytes_read = ih_net_socket_receive(hyperpost->socket,
      hyperpost->in_buffer + hyperpost->in_buffer_receive_position,
      max_bytes_to_read);
  if (actual_bytes_read > 0) {
    hyperpost->in_buffer_receive_position += actual_bytes_read;
    parse_incoming_message(hyperpost);
    if (hyperpost->in_buffer_have_complete_message) {
      received_complete_message = ih_core_bool_true;
      if (!put_received_message_in_inbox(hyperpost)) {
        ih_core_trace("put_received_message_in_inbox");
      }
    }
  } else {
    hyperpost->socket_closed = ih_core_bool_true;
    received_complete_message = ih_core_bool_true;
  }

  return received_complete_message;
}

void reset_for_next_receive(ih_net_hyperpost_t *hyperpost)
{
  hyperpost->in_buffer_have_status_line = ih_core_bool_false;
  hyperpost->in_buffer_have_headers = ih_core_bool_false;
  hyperpost->in_buffer_have_body = ih_core_bool_false;
  hyperpost->in_buffer_have_complete_message = ih_core_bool_false;
  hyperpost->in_buffer_receive_position = 0;
  hyperpost->in_buffer_parse_position = 0;
  if (hyperpost->in_resource_path) {
    free(hyperpost->in_resource_path);
    hyperpost->in_resource_path = NULL;
  }
  ih_container_set_clear(hyperpost->in_hyperheaders);
  if (hyperpost->in_body) {
    free(hyperpost->in_body);
    hyperpost->in_body = NULL;
  }
}

void reset_for_next_send(ih_net_hyperpost_t *hyperpost)
{
  free(hyperpost->out_buffer);
  hyperpost->out_buffer = NULL;
  hyperpost->out_buffer_size = 0;
  hyperpost->currently_sending_out_buffer = ih_core_bool_false;
  hyperpost->out_buffer_send_position = 0;
}

void send_messages_get_new_message(ih_net_hyperpost_t *hyperpost)
{
  ih_net_hypermessage_t *hypermessage;

  hypermessage = ih_container_list_find_first(hyperpost->outbox);
  if (hypermessage) {
    if (put_message_into_out_buffer(hyperpost, hypermessage)) {
      ih_container_list_remove_first(hyperpost->outbox);
      hyperpost->currently_sending_out_buffer = ih_core_bool_true;
    } else {
      ih_core_trace("put_message_into_out_buffer");
    }
  }
}

ih_core_bool_t send_messages_send_current_message(ih_net_hyperpost_t *hyperpost)
{
  assert(hyperpost);
  ih_core_bool_t sent_complete_message;
  int actual_bytes_sent;
  int bytes_remaining_to_send;

  bytes_remaining_to_send = hyperpost->out_buffer_size
    - hyperpost->out_buffer_send_position;
  if (bytes_remaining_to_send > 0) {
    actual_bytes_sent = ih_net_socket_send(hyperpost->socket,
        hyperpost->out_buffer, bytes_remaining_to_send);
    if (actual_bytes_sent > 0) {
      hyperpost->out_buffer_send_position += actual_bytes_sent;
      if (hyperpost->out_buffer_send_position == hyperpost->out_buffer_size) {
        sent_complete_message = ih_core_bool_true;
      } else {
        sent_complete_message = ih_core_bool_false;
      }
    } else {
      hyperpost->socket_closed = ih_core_bool_true;
      sent_complete_message = ih_core_bool_false;
    }
  } else {
    sent_complete_message = ih_core_bool_true;
  }

  return sent_complete_message;
}
