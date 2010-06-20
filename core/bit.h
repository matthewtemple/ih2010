#ifndef ih_core_bit_h
#define ih_core_bit_h

#include "ih/core/object.h"

typedef unsigned char ih_core_bit_t;

int ih_core_bit_compare(void *bit_object_a, void *bit_object_b);

void *ih_core_bit_copy(void *bit_object);

void ih_core_bit_destroy(void *bit_object);

#endif
