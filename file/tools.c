#include "ih/core/tools.h"
#include "ih/external/external.h"
#include "ih/file/basic.h"
#include "ih/file/tools.h"

ih_core_bool_t ih_file_count_lines_in_file(char *filename,
    unsigned long *line_count)
{
  assert(filename);
  assert(line_count);
  ih_core_bool_t success;
  ih_file_basic_t *file;

  file = ih_file_basic_create(filename, IH_FILE_MODE_OPEN_FOR_READ);
  if (file) {
    success = ih_file_basic_get_line_count(file, line_count);
    ih_file_basic_destroy(file);
  } else {
    success = ih_core_bool_false;
    ih_core_trace("ih_file_basic_create");
  }

  return success;
}
