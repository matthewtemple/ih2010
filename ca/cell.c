#include "ih/ca/cell.h"
#include "ih/external/external.h"

ih_ca_t ih_ca_copy(ih_ca_t cell)
{
  ih_ca_t ih_ca_copy;

  ih_ca_copy.value = cell.value;
  ih_ca_copy.rule = cell.rule;

  return ih_ca_copy;
}

void ih_ca_init(ih_ca_t *cell, unsigned long value, unsigned long rule)
{
  assert(cell);

  cell->value = value;
  cell->rule = rule;
}
