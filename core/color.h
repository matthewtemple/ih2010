#ifndef ih_core_color_h
#define ih_core_color_h

struct ih_core_color_t {
  unsigned long red;
  unsigned long blue;
  unsigned long green;
};
typedef struct ih_core_color_t ih_core_color_t;

void ih_core_color_init(ih_core_color_t *color, unsigned long red,
    unsigned long blue, unsigned long green);

#endif
