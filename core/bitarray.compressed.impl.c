#include "ih/core/bitarray.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

struct ih_core_bitarray_t {
  unsigned long array_size;

  unsigned char *array;
  unsigned long bit_count;
};

struct ih_core_bitarray_array_position_t {
  unsigned long char_index;
  unsigned short char_bit_index;
};
typedef struct ih_core_bitarray_array_position_t
ih_core_bitarray_array_position_t;

static void calculate_array_postion(unsigned long bit_index,
    ih_core_bitarray_array_position_t *array_position);

void calculate_array_postion(unsigned long bit_index,
    ih_core_bitarray_array_position_t *array_position)
{
  assert(array_position);
  array_position->char_index = bit_index / 8;
  array_position->char_bit_index = bit_index % 8;
}

int ih_core_bitarray_compare(void *bitarray_object_a,
    void *bitarray_object_b)
{
  ih_core_trace_exit("TODO: implement");
}

void *ih_core_bitarray_copy(void *bitarray_object)
{
  assert(bitarray_object);
  ih_core_bitarray_t *bitarray;
  ih_core_bitarray_t *bitarray_copy;

  bitarray = bitarray_object;

  bitarray_copy = ih_core_bitarray_create(bitarray->bit_count);
  if (bitarray_copy) {
    memcpy(bitarray_copy->array, bitarray->array, bitarray->array_size);
  } else {
    ih_core_trace("ih_core_bitarray_create");
  }

  return bitarray_copy;
}

ih_core_bitarray_t *ih_core_bitarray_create(unsigned long size)
{
  assert(size > 0);
  ih_core_bitarray_t *bitarray;
  unsigned long array_memory_size;

  bitarray = malloc(sizeof *bitarray);
  if (bitarray) {
    array_memory_size = (size / 8) + 1;
    bitarray->array = malloc(array_memory_size);
    if (bitarray->array) {
      memset(bitarray->array, '\0', array_memory_size);
      bitarray->array_size = array_memory_size;
      bitarray->bit_count = size;
    } else {
      ih_core_trace("malloc");
      free(bitarray);
      bitarray = NULL;
    }
  } else {
    ih_core_trace("malloc");
  }

  return bitarray;
}

void ih_core_bitarray_destroy(void *bitarray_object)
{
  assert(bitarray_object);
  ih_core_bitarray_t *bitarray;

  bitarray = bitarray_object;

  free(bitarray->array);
  free(bitarray);

}

ih_core_bit_t ih_core_bitarray_get_bit_actual(ih_core_bitarray_t *bitarray,
    unsigned long index)
{
  assert(bitarray);
  ih_core_bitarray_array_position_t array_position;
  ih_core_bit_t bit;
  unsigned char *c;

  calculate_array_postion(index, &array_position);
  c = bitarray->array + array_position.char_index;
  bit = get_bit_from_unsigned_char(c, array_position.char_bit_index);

  return bit;
}

unsigned long ih_core_bitarray_get_size(ih_core_bitarray_t *bitarray)
{
  return bitarray->bit_count;
}

void ih_core_bitarray_set_all(ih_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 255, bitarray->array_size);
}

void ih_core_bitarray_set_bit_actual(ih_core_bitarray_t *bitarray,
    unsigned long index, ih_core_bit_t value)
{
  assert(bitarray);
  ih_core_bitarray_array_position_t array_position;
  unsigned char *c;

  calculate_array_postion(index, &array_position);
  c = bitarray->array + array_position.char_index;
  set_bit_in_unsigned_char(c, array_position.char_bit_index, value);
}

unsigned long ih_core_bitarray_get_memory_size_bytes
(ih_core_bitarray_t *bitarray)
{
  assert(bitarray);
  unsigned long size;

  size = sizeof *bitarray;
  size += (bitarray->array_size / 8) + 1;

  return size;
}

void ih_core_bitarray_unset_all(ih_core_bitarray_t *bitarray)
{
  memset(bitarray->array, 0, bitarray->array_size);
}
