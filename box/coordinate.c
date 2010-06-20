#include "ih/box/coordinate.h"

void ih_box_coordinate_copy(ih_box_coordinate_t *destination,
    ih_box_coordinate_t *source)
{
  assert(destination);
  assert(source);

  destination->x = source->x;
  destination->y = source->y;
  destination->z = source->z;
}

void ih_box_coordinate_free(ih_box_coordinate_t *coordinate) {}

void ih_box_coordinate_init_witih_coordinate(ih_box_coordinate_t *coordinate,
    ih_box_coordinate_t *initial_coordinate)
{
  assert(coordinate);

  coordinate->x = initial_coordinate->x;
  coordinate->y = initial_coordinate->y;
  coordinate->z = initial_coordinate->z;
}

void ih_box_coordinate_init_witih_random(ih_box_coordinate_t *coordinate,
    ih_box_coordinate_t *dimension_coordinate)
{
  assert(coordinate);
  assert(dimension_coordinate);

  coordinate->x = random() % dimension_coordinate->x;
  coordinate->y = random() % dimension_coordinate->y;
  coordinate->z = random() % dimension_coordinate->z;
}

void ih_box_coordinate_init_witih_xyz(ih_box_coordinate_t *coordinate,
    unsigned short x, unsigned short y, unsigned short z)
{
  assert(coordinate);

  coordinate->x = x;
  coordinate->y = y;
  coordinate->z = z;
}
