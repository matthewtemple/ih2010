#ifndef ih_core_content_h
#define ih_core_content_h

#define IH_CORE_CONTENT_TYPE_COUNT 5
enum ih_core_content_t {
  IH_CORE_CONTENT_UNKNOWN = 0,
  IH_CORE_CONTENT_MIME_IMAGE_GIF = 1,
  IH_CORE_CONTENT_MIME_TEXT_CSS = 2,
  IH_CORE_CONTENT_MIME_TEXT_HTML = 3,
  IH_CORE_CONTENT_MIME_TEXT_JAVASCRIPT = 4,
};
typedef enum ih_core_content_t ih_core_content_t;

char *ih_core_content_get_name(ih_core_content_t content_type);

#endif
