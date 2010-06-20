#ifndef ih_box_coordinate_h
#define ih_box_coordinate_h

#include "ih/external/external.h"

struct ih_box_coordinate_t {
  unsigned short x;
  unsigned short y;
  unsigned short z;
};
typedef struct ih_box_coordinate_t ih_box_coordinate_t;

void ih_box_coordinate_copy(ih_box_coordinate_t *destination,
    ih_box_coordinate_t *source);

void ih_box_coordinate_free(ih_box_coordinate_t *coordinate);

void ih_box_coordinate_init_witih_coordinate(ih_box_coordinate_t *coordinate,
    ih_box_coordinate_t *initial_coordinate);

void ih_box_coordinate_init_witih_random(ih_box_coordinate_t *coordinate,
    ih_box_coordinate_t *dimension_coordinate);

void ih_box_coordinate_init_witih_xyz(ih_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z);

#endif
