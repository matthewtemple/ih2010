#include "ih/core/buffer.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

static char *initial_buffer = "happy";
static char *additional_buffer_0 = " day for fun on the beach";
static char *additional_buffer_1 = " with a white paper rabbyt";

int main(int argc, char *argv[])
{
  ih_core_buffer_t *buffer;
  char *string;
  unsigned long string_size;

  buffer = ih_core_buffer_create(initial_buffer, strlen(initial_buffer));
  if (!buffer) {
    ih_core_trace_exit("ih_core_buffer_create");
  }

  if (!ih_core_buffer_append_string(buffer, additional_buffer_0,
          strlen(additional_buffer_0))) {
    ih_core_trace_exit("ih_core_buffer_append_string");
  }
  if (!ih_core_buffer_append_string(buffer, additional_buffer_1,
          strlen(additional_buffer_1))) {
    ih_core_trace_exit("ih_core_buffer_append_string");
  }

  string = ih_core_buffer_get_string(buffer, &string_size);
  printf("string_size:%lu\n", string_size);
  printf(":%s:\n", string);

  ih_core_buffer_destroy(buffer);

  return 0;
}
