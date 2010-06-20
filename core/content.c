#include "ih/core/content.h"
#include "ih/external/external.h"

static char *content_names[IH_CORE_CONTENT_TYPE_COUNT] = {
  "void",
  "image/gif",
  "text/css",
  "text/html",
  "text/javascript"
};

char *ih_core_content_get_name(ih_core_content_t content_type)
{
  char *content_name;

  if (content_type < IH_CORE_CONTENT_TYPE_COUNT) {
    content_name = content_names[content_type];
  } else {
    content_name = "[unknown]";
  }

  return content_name;
}
