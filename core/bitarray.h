#ifndef ih_core_bitarray_h
#define ih_core_bitarray_h

#include "ih/core/bit.h"
#include "ih/core/bool.h"
#include "ih/core/message.h"
#include "ih/core/object.h"
#include "ih/core/string.h"

#define IH_CORE_BITARRAY_BITS_IN_CHAR 8
#define IH_CORE_BITARRAY_BITS_IN_LONG 32
#define IH_CORE_BITARRAY_BITS_IN_SHORT 16
#define IH_CORE_BITARRAY_BITS_IN_UNSIGNED_CHAR 8
#define IH_CORE_BITARRAY_BITS_IN_UNSIGNED_LONG 32
#define IH_CORE_BITARRAY_BITS_IN_UNSIGNED_SHORT 16

struct ih_core_bitarray_t;
typedef struct ih_core_bitarray_t ih_core_bitarray_t;

ih_core_bool_t ih_core_bitarray_add_to_message(void *bitarray_object,
    ih_core_message_t *message);

char *ih_core_bitarray_as_binary_string(void *bitarray_object);

int ih_core_bitarray_compare(void *bitarray_object_a, void *bitarray_object_b);

void *ih_core_bitarray_copy(void *bitarray_object);

ih_core_bitarray_t *ih_core_bitarray_create(unsigned long size);

ih_core_bitarray_t *ih_core_bitarray_create_from_char(char value);

ih_core_bitarray_t *ih_core_bitarray_create_from_char_bits(char value,
    unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_double(double value);

ih_core_bitarray_t *ih_core_bitarray_create_from_double_bits(double value,
    unsigned short integral_bits, unsigned short fractional_bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_long(long value);

ih_core_bitarray_t *ih_core_bitarray_create_from_long_bits(long value,
    unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_message
(ih_core_message_t *message);

ih_core_bitarray_t *ih_core_bitarray_create_from_short(short value);

ih_core_bitarray_t *ih_core_bitarray_create_from_short_bits(short value,
    unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_string(char *string,
    unsigned long string_length);

ih_core_bitarray_t *ih_core_bitarray_create_from_string_bits(char *string,
    unsigned long string_length, unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_char
(unsigned char value);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_char_bits
(unsigned char value, unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_long
(unsigned long value);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_long_bits
(unsigned long value, unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_short
(unsigned short value);

ih_core_bitarray_t *ih_core_bitarray_create_from_unsigned_short_bits
(unsigned short value, unsigned short bits);

ih_core_bitarray_t *ih_core_bitarray_create_random(unsigned long size);

void ih_core_bitarray_destroy(void *bitarray_object);

char *ih_core_bitarray_get_as_string(void *bitarray_object);

unsigned long ih_core_bitarray_get_actual_index(ih_core_bitarray_t *bitarray,
    unsigned long virtual_index);

ih_core_bit_t ih_core_bitarray_get_bit(ih_core_bitarray_t *bitarray,
    unsigned long virtual_index);

ih_core_bool_t ih_core_bitarray_get_bool(ih_core_bitarray_t *bitarray,
    unsigned long index);

char ih_core_bitarray_get_char(ih_core_bitarray_t *bitarray,
    unsigned long index);

char ih_core_bitarray_get_char_bits(ih_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

double ih_core_bitarray_get_double(ih_core_bitarray_t *bitarray,
    unsigned long index);

double ih_core_bitarray_get_double_bits(ih_core_bitarray_t *bitarray,
    unsigned long index, unsigned short integral_bits,
    unsigned short fractional_bits);

/*
  TODO: deprecate in favor of ih_core_bitarray_get_double()
*/
double ih_core_bitarray_get_double_from_bits(ih_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index);

long ih_core_bitarray_get_long(ih_core_bitarray_t *bitarray,
    unsigned long index);

long ih_core_bitarray_get_long_bits(ih_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long ih_core_bitarray_get_memory_size_bytes
(ih_core_bitarray_t *bitarray);

short ih_core_bitarray_get_short(ih_core_bitarray_t *bitarray,
    unsigned long index);

short ih_core_bitarray_get_short_bits(ih_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned long ih_core_bitarray_get_size(ih_core_bitarray_t *bitarray);

char *ih_core_bitarray_get_string(ih_core_bitarray_t *bitarray,
    unsigned long index, unsigned short bits);

unsigned char ih_core_bitarray_get_unsigned_char(ih_core_bitarray_t *bitarray,
    unsigned long index);

unsigned char ih_core_bitarray_get_unsigned_char_bits
(ih_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

unsigned long ih_core_bitarray_get_unsigned_long(ih_core_bitarray_t *bitarray,
    unsigned long index);

unsigned long ih_core_bitarray_get_unsigned_long_bits
(ih_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

/*
  TODO: deprecate in favor of ih_core_bitarray_get_unsigned_long()
*/
unsigned long ih_core_bitarray_get_unsigned_long_from_bits
(ih_core_bitarray_t *bitarray, unsigned long start_index,
    unsigned long end_index);

unsigned short ih_core_bitarray_get_unsigned_short
(ih_core_bitarray_t *bitarray, unsigned long index);

unsigned short ih_core_bitarray_get_unsigned_short_bits
(ih_core_bitarray_t *bitarray, unsigned long index, unsigned short bits);

ih_core_bool_t ih_core_bitarray_increment(ih_core_bitarray_t *bitarray);

void ih_core_bitarray_print(ih_core_bitarray_t *bitarray);

void ih_core_bitarray_randomize(ih_core_bitarray_t *bitarray);

void ih_core_bitarray_set_all(ih_core_bitarray_t *bitarray);

void ih_core_bitarray_set_bit(ih_core_bitarray_t *bitarray,
    unsigned long index, ih_core_bit_t value);

void ih_core_bitarray_set_bits_from_bitarray(ih_core_bitarray_t *destination,
    unsigned long destination_index, ih_core_bitarray_t *source,
    unsigned long source_index, unsigned long length);

void ih_core_bitarray_flip_bit(ih_core_bitarray_t *bitarray,
    unsigned long index);

void ih_core_bitarray_set_bit(ih_core_bitarray_t *bitarray,
    unsigned long virtual_index, ih_core_bit_t value);

void ih_core_bitarray_set_double(ih_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, double value);

void ih_core_bitarray_set_unsigned_long(ih_core_bitarray_t *bitarray,
    unsigned long start_index, unsigned long end_index, unsigned long value);

void ih_core_bitarray_unset_all(ih_core_bitarray_t *bitarray);

#endif
