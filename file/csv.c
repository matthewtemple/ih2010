#include "ih/container/array.h"
#include "ih/container/list.h"
#include "ih/container/map.h"
#include "ih/core/objects.h"
#include "ih/core/string.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/file/basic.h"
#include "ih/file/csv.h"

struct ih_file_csv_t {
  unsigned long object_count;
  unsigned long field_count;

  ih_container_map_t *name_to_index;
  ih_container_array_t *index_to_name;
  ih_container_array_t *objects;

  ih_core_objects_t ih_core_objects;
};

static ih_container_array_t *create_index_to_name_array(ih_file_csv_t *csv,
    ih_file_basic_t *file);

static ih_container_map_t *create_name_to_index_map(ih_file_csv_t *csv,
    ih_file_basic_t *file);

static ih_container_array_t *create_objects_array(ih_file_csv_t *csv,
    ih_file_basic_t *file, unsigned long start_object,
    unsigned long end_object);

static char *get_value_by_index_as_string(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

static char *get_value_by_name_as_string(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

ih_file_csv_t *ih_file_csv_create(char *filename)
{
  return ih_file_csv_create_extended(filename, 0, 0);
}

ih_file_csv_t *ih_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object)
{
  assert(filename);
  ih_file_csv_t *csv;
  ih_core_bool_t so_far_so_good;
  ih_file_basic_t *file;

  file = NULL;

  csv = malloc(sizeof *csv);
  if (csv) {
    csv->name_to_index = NULL;
    csv->index_to_name = NULL;
    csv->objects = NULL;
    ih_core_objects_init(&csv->ih_core_objects);
    file = ih_file_basic_create(filename, IH_FILE_MODE_OPEN_FOR_READ);
    if (file) {
      so_far_so_good = ih_core_bool_true;
    } else {
      ih_core_trace("ih_file_basic_create");
      so_far_so_good = ih_core_bool_false;
    }
  } else {
    so_far_so_good = ih_core_bool_false;
    ih_core_trace("malloc");
  }

  if (so_far_so_good) {
    csv->name_to_index = create_name_to_index_map(csv, file);
    if (!csv->name_to_index) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("create_name_to_index_map");
    }
  }

  if (so_far_so_good) {
    csv->index_to_name = create_index_to_name_array(csv, file);
    if (!csv->index_to_name) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("create_index_to_name_array");
    }
  }

  if (so_far_so_good) {
    csv->objects = create_objects_array(csv, file, start_object, end_object);
    if (!csv->objects) {
      so_far_so_good = ih_core_bool_false;
      ih_core_trace("create_objects_array");
    }
  }

  if (!so_far_so_good && csv) {
    if (csv->name_to_index) {
      ih_container_map_destroy(csv->name_to_index);
    }
    if (csv->index_to_name) {
      ih_container_array_destroy(csv->index_to_name);
    }
    if (csv->objects) {
      ih_container_array_destroy(csv->objects);
    }
  }

  if (file) {
    ih_file_basic_destroy(file);
  }

  return csv;
}

void ih_file_csv_destroy(void *csv_object)
{
  assert(csv_object);
  ih_file_csv_t *csv;

  csv = csv_object;

  ih_container_map_destroy(csv->name_to_index);
  ih_container_array_destroy(csv->index_to_name);
  ih_container_array_destroy(csv->objects);
  ih_core_objects_free(&csv->ih_core_objects);
  free(csv);
}

ih_container_array_t *ih_file_csv_get_field_by_index_as_array
(ih_file_csv_t *csv, unsigned long field_index)
{
  assert(csv);
  ih_container_array_t *array;
  unsigned long each_object;
  char *value;
  char *value_copy;

  array = ih_container_array_create(csv->object_count, ih_core_string_compare,
      ih_core_string_copy, ih_core_string_destroy);
  if (array) {
    for (each_object = 0; each_object < csv->object_count; each_object++) {
      value = get_value_by_index_as_string(csv, each_object, field_index);
      value_copy = strdup(value);
      if (value_copy) {
        ih_container_array_add(array, each_object, value_copy);
      } else {
        ih_core_trace("strdup");
        ih_container_array_destroy(array);
        array = NULL;
        break;
      }
    }
  } else {
    ih_core_trace("ih_container_array_create");
  }

  return array;
}

ih_container_array_t *ih_file_csv_get_field_by_name_as_array
(ih_file_csv_t *csv, char *field_name)
{
  assert(csv);
  assert(field_name);
  ih_container_array_t *array;
  unsigned long each_object;
  char *value;
  char *value_copy;

  array = ih_container_array_create(csv->object_count, ih_core_string_compare,
      ih_core_string_copy, ih_core_string_destroy);
  if (array) {
    for (each_object = 0; each_object < csv->object_count; each_object++) {
      value = get_value_by_name_as_string(csv, each_object, field_name);
      value_copy = strdup(value);
      if (value_copy) {
        ih_container_array_add(array, each_object, value_copy);
      } else {
        ih_core_trace("strdup");
        ih_container_array_destroy(array);
        array = NULL;
        break;
      }
    }
  } else {
    ih_core_trace("ih_container_array_create");
  }

  return array;
}

unsigned long ih_file_csv_get_field_count(ih_file_csv_t *csv)
{
  return csv->field_count;
}

ih_core_bool_t ih_file_csv_get_field_index(ih_file_csv_t *csv,
    char *field_name, unsigned long *field_index)
{
  assert(csv);
  unsigned long *field_index_object;
  ih_core_bool_t success;

  field_index_object = ih_container_map_find(csv->name_to_index, field_name);
  if (field_index_object) {
    success = ih_core_bool_true;
    *field_index = *((unsigned long *) field_index_object);
  } else {
    success = ih_core_bool_false;
  }

  return success;
}

char *ih_file_csv_get_field_name(ih_file_csv_t *csv, unsigned long field_index)
{
  return ih_container_array_find(csv->index_to_name, field_index);
}

unsigned long ih_file_csv_get_object_count(ih_file_csv_t *csv)
{
  return csv->object_count;
}

ih_core_bit_t ih_file_csv_get_value_by_index_as_bit(ih_file_csv_t *csv,
    unsigned long object_index,
    unsigned long field_index)
{
  assert(csv);
  char *value_string;
  char value_bit;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    if ('1' == *value_string) {
      value_bit = 1;
    } else {
      value_bit = 0;
    }
  } else {
    value_bit = 0;
  }

  return value_bit;
}

ih_core_bool_t ih_file_csv_get_value_by_index_as_bool(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  ih_core_bool_t value_bool;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    if ('1' == *value_string) {
      value_bool = ih_core_bool_true;
    } else {
      value_bool = ih_core_bool_false;
    }
  } else {
    value_bool = ih_core_bool_false;
  }

  return value_bool;
}

char ih_file_csv_get_value_by_index_as_char(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  char value_char;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  if (strlen(value_string) > 0) {
    value_char = *value_string;
  } else {
    value_char = '\0';
  }

  return value_char;
}

double ih_file_csv_get_value_by_index_as_double(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  double value_double;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_double = atof(value_string);

  return value_double;
}

long ih_file_csv_get_value_by_index_as_long(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  long value_long;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_long = atol(value_string);

  return value_long;
}

short ih_file_csv_get_value_by_index_as_short(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  short value_short;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_short = atoi(value_string);

  return value_short;
}

char *ih_file_csv_get_value_by_index_as_string(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  return get_value_by_index_as_string(csv, object_index, field_index);
}

unsigned char ih_file_csv_get_value_by_index_as_unsigned_char
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned char value_unsigned_char;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_char = atoi(value_string);

  return value_unsigned_char;
}

unsigned long ih_file_csv_get_value_by_index_as_unsigned_long
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned long value_unsigned_long;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_long = atol(value_string);

  return value_unsigned_long;
}

unsigned short ih_file_csv_get_value_by_index_as_unsigned_short
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value_string;
  unsigned short value_unsigned_short;

  value_string = get_value_by_index_as_string(csv, object_index, field_index);
  value_unsigned_short = atoi(value_string);

  return value_unsigned_short;
}

char ih_file_csv_get_value_by_name_as_char(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  char value_char;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  if (strlen(value_string) > 0) {
    value_char = *value_string;
  } else {
    value_char = '\0';
  }

  return value_char;
}

double ih_file_csv_get_value_by_name_as_double(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  double value_double;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_double = atof(value_string);

  return value_double;
}

long ih_file_csv_get_value_by_name_as_long(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  long value_long;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_long = atol(value_string);

  return value_long;
}

short ih_file_csv_get_value_by_name_as_short(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_short;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_short = atoi(value_string);

  return value_short;
}

char *ih_file_csv_get_value_by_name_as_string(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  return get_value_by_name_as_string(csv, object_index, field_name);
}

unsigned long ih_file_csv_get_value_by_name_as_unsigned_long
(ih_file_csv_t *csv, unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_unsigned_long;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_unsigned_long = atol(value_string);

  return value_unsigned_long;
}

unsigned short ih_file_csv_get_value_by_name_as_unsigned_short
(ih_file_csv_t *csv, unsigned long object_index, char *field_name)
{
  assert(csv);
  assert(field_name);
  char *value_string;
  short value_unsigned_short;

  value_string = get_value_by_name_as_string(csv, object_index, field_name);
  value_unsigned_short = atoi(value_string);

  return value_unsigned_short;
}

ih_container_array_t *create_index_to_name_array(ih_file_csv_t *csv,
    ih_file_basic_t *file)
{
  assert(csv);
  assert(file);
  ih_container_array_t *index_to_name;
  char *first_line;
  ih_container_list_t *line_list;
  ih_container_list_t *field_names;
  char *field_name;
  unsigned long field_index;

  index_to_name = ih_container_array_create(csv->field_count,
      ih_core_string_compare, ih_core_string_copy, ih_core_string_destroy);
  if (index_to_name) {
    line_list = ih_file_basic_get_as_line_list(file);
    if (line_list) {
      first_line = ih_container_list_find_first(line_list);
      if (first_line) {
        field_names = ih_container_list_create_strings_from_string
          (first_line, ",");
        if (field_names) {
          field_index = 0;
          ih_container_list_iterate_start(field_names);
          while ((field_name = ih_container_list_iterate_next(field_names))) {
            ih_container_array_add(index_to_name, field_index, field_name);
            field_index++;
          }
          ih_container_list_dont_destroys(field_names);
          ih_container_list_destroy(field_names);
        } else {
          ih_core_trace("ih_container_list_create_strings_from_string");
        }
      } else {
        ih_core_trace_exit("");
      }
      ih_container_list_destroy(line_list);
    } else {
      ih_core_trace("ih_file_basic_get_as_line_list");
    }
  } else {
    ih_core_trace("ih_container_array_add");
  }

  return index_to_name;
}

ih_container_map_t *create_name_to_index_map(ih_file_csv_t *csv,
    ih_file_basic_t *file)
{
  assert(csv);
  assert(file);
  ih_container_list_t *line_list;
  ih_container_map_t *map;
  ih_container_list_t *field_names;
  char *first_line;
  char *name_object;
  unsigned long index;
  unsigned long *index_object;
  char *name;

  map = NULL;

  line_list = ih_file_basic_get_as_line_list(file);
  if (line_list) {
    first_line = ih_container_list_find_first(line_list);
    if (first_line) {
      field_names = ih_container_list_create_strings_from_string
        (first_line, ",");
      if (field_names) {
        csv->field_count = ih_container_list_get_size(field_names);
        map = ih_container_map_create(&csv->ih_core_objects.string_objectey,
            &csv->ih_core_objects.unsigned_long_objectey,
            IH_CONTAINER_MAP_DESTROY);
        if (map) {
          index = 0;
          ih_container_list_iterate_start(field_names);
          while ((name = ih_container_list_iterate_next(field_names))) {
            name_object = ih_core_string_copy(name);
            if (name_object) {
              index_object = malloc(sizeof *index_object);
              if (index_object) {
                *index_object = index;
                if (!ih_container_map_add(map, name_object, index_object)) {
                  ih_core_trace("ih_container_map_add");
                }
              } else {
                ih_core_trace("malloc");
              }
            } else {
              ih_core_trace("ih_core_string_copy");
            }
            index++;
          }
        } else {
          ih_core_trace("ih_container_map_create");
        }
        ih_container_list_destroy(field_names);
      } else {
        ih_core_trace("ih_container_list_create_strings_from_string");
      }
    } else {
      ih_core_trace("ih_container_list_find_first");
    }
    ih_container_list_destroy(line_list);
  } else {
    ih_core_trace("ih_file_basic_get_as_line_list");
  }

  return map;
}

ih_container_array_t *create_objects_array(ih_file_csv_t *csv,
    ih_file_basic_t *file, unsigned long start_object,
    unsigned long end_object)
{
  assert(csv);
  assert(file);
  ih_container_array_t *objects_array;
  ih_container_list_t *lines;
  ih_container_array_t *values;
  char *line;
  unsigned long total_object_index;
  unsigned long relative_object_index;
  unsigned long total_object_count;

  objects_array = NULL;

  lines = ih_file_basic_get_as_line_list(file);
  if (lines) {

    total_object_count = ih_container_list_get_size(lines) - 1;
    if ((0 == start_object) && (0 == end_object)) {
      end_object = total_object_count - 1;
    }
    csv->object_count = (end_object - start_object) + 1;

    objects_array = ih_container_array_create(csv->object_count,
        ih_container_array_compare, ih_container_array_copy,
        ih_container_array_destroy);
    if (objects_array) {
      ih_container_list_iterate_start(lines);
      ih_container_list_iterate_next(lines);
      total_object_index = 0;
      relative_object_index = 0;
      while ((line = ih_container_list_iterate_next(lines))) {
        if ((total_object_index >= start_object)
            && (total_object_index <= end_object)) {
          values = ih_container_array_create_strings_from_string(line, ",");
          if (values) {
            ih_container_array_add(objects_array, relative_object_index,
                values);
          } else {
            ih_core_trace("ih_container_array_create_strings_from_string");
            ih_container_array_destroy(values);
          }
          relative_object_index++;
        }
        total_object_index++;
      }
    } else {
      ih_core_trace("ih_container_array_create");
    }
    ih_container_list_destroy(lines);
  } else {
    ih_core_trace("ih_file_basic_get_as_line_list");
  }

  return objects_array;
}

char *get_value_by_index_as_string(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index)
{
  assert(csv);
  char *value;
  ih_container_array_t *object;

  object = ih_container_array_find(csv->objects, object_index);
  value = ih_container_array_find(object, field_index);

  return value;
}

char *get_value_by_name_as_string(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name)
{
  assert(csv);
  unsigned long field_index;
  unsigned long *field_index_object;
  char *value;

  field_index_object = ih_container_map_find(csv->name_to_index, field_name);
  if (field_index_object) {
    field_index = *((unsigned long *) field_index_object);
    value = get_value_by_index_as_string(csv, object_index, field_index);
  } else {
    value = NULL;
    ih_core_trace("ih_container_map_find");
  }

  return value;
}
