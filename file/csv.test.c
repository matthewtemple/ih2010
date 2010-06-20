#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/file/csv.h"

int main(int argc, char *argv[])
{
  ih_file_csv_t *csv;
  unsigned long object_count;
  unsigned long field_count;
  unsigned long each_object;
  unsigned long each_field;
  char *value;

  csv = ih_file_csv_create_extended("file/csv_test.csv", 1, 2);
  if (!csv) {
    ih_core_trace_exit("ih_file_csv_create");
  }

  object_count = ih_file_csv_get_object_count(csv);
  field_count = ih_file_csv_get_field_count(csv);

  for (each_field = 0; each_field < field_count; each_field++) {
    for (each_object = 0; each_object < object_count; each_object++) {
      value = ih_file_csv_get_value_by_index_as_string(csv, each_object,
          each_field);
      printf("%s,", value);
    }
    printf("\n");
  }

  ih_file_csv_destroy(csv);

  return 0;
}
