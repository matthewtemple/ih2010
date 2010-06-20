#ifndef ih_ca_cell_h
#define ih_ca_cell_h

#define IH_CA_NO_RULE 0

struct ih_ca_t {
  unsigned long value;
  unsigned long rule;
};
typedef struct ih_ca_t ih_ca_t;

ih_ca_t ih_ca_copy(ih_ca_t cell);

void ih_ca_init(ih_ca_t *cell, unsigned long value, unsigned long rule);

#endif
