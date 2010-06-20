#include "ih/config/file.h"
#include "ih/container/set.h"
#include "ih/core/nameobject.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/file/basic.h"

struct ih_config_file_t {
  /* TODO: reimplement with ih_container_map_t, like config_options? */
  ih_container_set_t *strings;
  ih_container_set_t *string_lists;
};

static ih_core_bool_t find_as_string(ih_config_file_t *file, char *name,
    char **value);

static void ih_config_file_create_rollback(ih_config_file_t *conf);

static ih_core_bool_t line_is_a_comment(char *line);

static void parse_list_value(ih_config_file_t *file, char *name, char *value);

static void parse_string_value(ih_config_file_t *file, char *name, char *value);

static void read_file(ih_config_file_t *file, char *filename);

static ih_core_bool_t value_is_a_list(char *name);

ih_core_bool_t find_as_string(ih_config_file_t *file, char *name,
    char **value)
{
  assert(file);
  assert(name);
  assert(value);
  ih_core_nameobject_t *nameobject_decoy;
  ih_core_nameobject_t *nameobject;
  ih_core_bool_t found_it;

  *value = NULL;

  nameobject_decoy = ih_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = ih_container_set_find(file->strings, nameobject_decoy);
    ih_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      found_it = ih_core_bool_true;
      *value = ih_core_nameobject_get_object(nameobject);
    } else {
      found_it = ih_core_bool_false;
    }
  } else {
    found_it = ih_core_bool_false;
    ih_core_trace("ih_core_nameobject_create_decoy");
  }

  return found_it;
}

ih_config_file_t *ih_config_file_create(char *filename)
{
  ih_config_file_t *file;
  ih_core_bool_t so_far_so_good;

  file = malloc(sizeof *file);
  if (file) {
    so_far_so_good = ih_core_bool_true;
  } else {
    so_far_so_good = ih_core_bool_false;
  }

  if (so_far_so_good) {
    file->strings = ih_container_set_create(ih_core_nameobject_compare,
        ih_core_nameobject_copy, ih_core_nameobject_destroy);
    if (!file->strings) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    file->string_lists = ih_container_set_create(ih_core_nameobject_compare,
        ih_core_nameobject_copy, ih_core_nameobject_destroy);
    if (!file->string_lists) {
      so_far_so_good = ih_core_bool_false;
    }
  }

  if (so_far_so_good) {
    if (ih_file_basic_exists(filename)) {
      read_file(file, filename);
    }
  }

  if (!so_far_so_good && file) {
    ih_config_file_create_rollback(file);
    file = NULL;
  }

  return file;
}

void ih_config_file_create_rollback(ih_config_file_t *file)
{
  assert(file);

  if (file->strings) {
    ih_container_set_destroy(file->strings);
  }
  if (file->string_lists) {
    ih_container_set_destroy(file->string_lists);
  }
  free(file);

}

void ih_config_file_destroy(ih_config_file_t *file)
{
  assert(file);
  assert(file->strings);
  assert(file->string_lists);

  ih_container_set_destroy(file->strings);
  ih_container_set_destroy(file->string_lists);
  free(file);

}

ih_core_bool_t ih_config_file_find(ih_config_file_t *file, char *name)
{
  assert(file);
  assert(name);
  ih_core_nameobject_t *nameobject_decoy;
  ih_core_bool_t found_it;

  nameobject_decoy = ih_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    if (ih_container_set_find(file->strings, nameobject_decoy)) {
      found_it = ih_core_bool_true;
    } else {
      found_it = ih_core_bool_false;
    }
    ih_core_nameobject_destroy_decoy(nameobject_decoy);
  } else {
    found_it = ih_core_bool_false;
    ih_core_trace("ih_core_nameobject_create_decoy");
  }

  return found_it;
}

ih_core_bool_t ih_config_file_find_as_double(ih_config_file_t *file, char *name,
    double *value, double default_value)
{
  assert(file);
  assert(name);
  assert(value);
  ih_core_bool_t found;
  char *value_string;

  if (find_as_string(file, name, &value_string)) {
    found = ih_core_bool_true;
    *value = atof(value_string);
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_file_find_as_string(ih_config_file_t *file, char *name,
    char **value, char *default_value)
{
  assert(file);
  assert(name);
  assert(value);
  assert(default_value);
  ih_core_bool_t found_it;

  if (find_as_string(file, name, value)) {
    found_it = ih_core_bool_true;
  } else {
    found_it = ih_core_bool_false;
    *value = default_value;
  }

  return found_it;
}

ih_core_bool_t ih_config_file_find_as_unsigned_long(ih_config_file_t *file,
    char *name, unsigned long *value, unsigned long default_value)
{
  assert(file);
  assert(name);
  assert(value);
  char *value_string;
  ih_core_bool_t found;

  if (find_as_string(file, name, &value_string)) {
    found = ih_core_bool_true;
    *value = atol(value_string);
  } else {
    found = ih_core_bool_false;
    *value = default_value;
  }

  return found;
}

ih_core_bool_t ih_config_file_find_as_unsigned_short(ih_config_file_t *file,
    char *name, unsigned short *value, unsigned short default_value)
{
  assert(file);
  assert(name);
  assert(value);
  unsigned long value_unsigned_long;
  ih_core_bool_t success;

  success = ih_config_file_find_as_unsigned_long
    (file, name, &value_unsigned_long, default_value);
  if (success) {
    *value = (unsigned short) value_unsigned_long;
  }

  return success;
}

ih_core_bool_t ih_config_file_find_list_as_strings(ih_config_file_t *file,
    char *name, ih_container_list_t **list)
{
  assert(file);
  assert(name);
  ih_core_nameobject_t *nameobject_decoy;
  ih_core_nameobject_t *nameobject;
  ih_core_bool_t success;

  *list = NULL;

  nameobject_decoy = ih_core_nameobject_create_decoy(name);
  if (nameobject_decoy) {
    nameobject = ih_container_set_find(file->string_lists, nameobject_decoy);
    ih_core_nameobject_destroy_decoy(nameobject_decoy);
    if (nameobject) {
      success = ih_core_bool_true;
      *list = ih_core_nameobject_get_object(nameobject);
    } else {
      success = ih_core_bool_false;
    }
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_core_nameobject_create_decoy");
  }

  return success;
}

ih_core_bool_t line_is_a_comment(char *line)
{
  assert(line);
  ih_core_bool_t is_a_comment;

  if (strlen(line) > 0) {
    if ('#' == *line) {
      is_a_comment = ih_core_bool_true;
    } else {
      is_a_comment = ih_core_bool_false;
    }
  } else {
    is_a_comment = ih_core_bool_true;
  }

  return is_a_comment;
}

void parse_list_value(ih_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  ih_core_nameobject_t *nameobject;
  ih_container_list_t *list;
  char *string;
  char *string_copy;
  char *strtok_context;

  list = ih_container_list_create(ih_core_string_compare, ih_core_string_copy,
      ih_core_string_destroy);
  if (list) {
    string = strtok_r(value, ",", &strtok_context);
    while (string) {
      string_copy = strdup(string);
      if (string_copy) {
        if (!ih_container_list_add_last(list, string_copy)) {
          ih_core_trace("ih_container_list_add_last");
        }
      } else {
        ih_core_trace("strdup");
      }
      string = strtok_r(NULL, ",", &strtok_context);
    }
    nameobject = ih_core_nameobject_create(name, list, IH_CORE_NO_COPY_FUNCTION,
        ih_container_list_destroy, ih_container_list_get_as_string);
    if (nameobject) {
      if (!ih_container_set_add(file->string_lists, nameobject)) {
        ih_core_trace("ih_container_set_add");
        ih_core_nameobject_destroy(nameobject);
      }
    } else {
      ih_core_trace("ih_core_nameobject_create");
    }
  } else {
    ih_core_trace("ih_container_list_create");
  }

}

void parse_string_value(ih_config_file_t *file, char *name, char *value)
{
  assert(file);
  assert(name);
  assert(value);
  ih_core_nameobject_t *nameobject;

  nameobject = ih_core_nameobject_create(name, value, ih_core_string_copy,
      ih_core_string_destroy, ih_core_string_get_as_string);
  if (nameobject) {
    if (!ih_container_set_add(file->strings, nameobject)) {
      ih_core_trace("ih_container_set_add");
    }
  } else {
    ih_core_trace("ih_core_nameobject_create");
  }

}

void read_file(ih_config_file_t *file, char *filename)
{
  assert(file);
  assert(filename);
  ih_file_basic_t *file_basic;
  ih_container_list_t *lines;
  char *line;
  char *name;
  char *value;
  char *strtok_context;

  file_basic = ih_file_basic_create(filename, IH_FILE_MODE_OPEN_FOR_READ);
  if (file_basic) {
    lines = ih_file_basic_get_as_line_list(file_basic);
    if (lines) {
      ih_container_list_iterate_start(lines);
      while ((line = ih_container_list_iterate_next(lines))) {
        if (!line_is_a_comment(line)) {
          name = strtok_r(line, ":", &strtok_context);
          if (name) {
            value = strtok_r(NULL, "\n", &strtok_context);
            if (!value) {
              value = "";
            }
            if (value_is_a_list(name)) {
              parse_list_value(file, name, value);
            } else {
              parse_string_value(file, name, value);
            }
          }
        }
      }
      ih_container_list_destroy(lines);
    }
    ih_file_basic_destroy(file_basic);
  } else {
    ih_core_trace("ih_core_file_create");
  }
}

ih_core_bool_t value_is_a_list(char *name)
{
  assert(name);
  ih_core_bool_t is_a_list;
  unsigned short name_size;
  unsigned short start_compare_position;

  name_size = strlen(name);

  if (name_size >= 6) {
    start_compare_position = name_size - 6;
    if (0 == strcmp("[list]", name + start_compare_position)) {
      is_a_list = ih_core_bool_true;
    } else {
      is_a_list = ih_core_bool_false;
    }
  } else {
    is_a_list = ih_core_bool_false;
  }

  return is_a_list;
}
