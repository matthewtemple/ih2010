#ifndef ih_file_csv_h
#define ih_file_csv_h

#include "ih/container/array.h"
#include "ih/core/bit.h"
#include "ih/core/object.h"

struct ih_file_csv_t;
typedef struct ih_file_csv_t ih_file_csv_t;

ih_file_csv_t *ih_file_csv_create(char *filename);

ih_file_csv_t *ih_file_csv_create_extended(char *filename,
    unsigned long start_object, unsigned long end_object);

void ih_file_csv_destroy(void *csv_object);

ih_container_array_t *ih_file_csv_get_field_by_name_as_array
(ih_file_csv_t *csv, char *field_name);

ih_container_array_t *ih_file_csv_get_field_by_index_as_array
(ih_file_csv_t *csv, unsigned long field_index);

unsigned long ih_file_csv_get_field_count(ih_file_csv_t *csv);

ih_core_bool_t ih_file_csv_get_field_index(ih_file_csv_t *csv,
    char *field_name, unsigned long *field_index);

char *ih_file_csv_get_field_name(ih_file_csv_t *csv,
    unsigned long field_index);

unsigned long ih_file_csv_get_object_count(ih_file_csv_t *csv);

ih_core_bit_t ih_file_csv_get_value_by_index_as_bit(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

ih_core_bool_t ih_file_csv_get_value_by_index_as_bool(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char ih_file_csv_get_value_by_index_as_char(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

double ih_file_csv_get_value_by_index_as_double(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

long ih_file_csv_get_value_by_index_as_long(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

short ih_file_csv_get_value_by_index_as_short(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

char *ih_file_csv_get_value_by_index_as_string(ih_file_csv_t *csv,
    unsigned long object_index, unsigned long field_index);

unsigned char ih_file_csv_get_value_by_index_as_unsigned_char
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned long ih_file_csv_get_value_by_index_as_unsigned_long
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

unsigned short ih_file_csv_get_value_by_index_as_unsigned_short
(ih_file_csv_t *csv, unsigned long object_index, unsigned long field_index);

char ih_file_csv_get_value_by_name_as_char(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

double ih_file_csv_get_value_by_name_as_double(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

long ih_file_csv_get_value_by_name_as_long(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

short ih_file_csv_get_value_by_name_as_short(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

char *ih_file_csv_get_value_by_name_as_string(ih_file_csv_t *csv,
    unsigned long object_index, char *field_name);

unsigned char ih_file_csv_get_value_by_name_as_unsigned_char
(ih_file_csv_t *csv, unsigned long object_index, char *field_name);

unsigned long ih_file_csv_get_value_by_name_as_unsigned_long
(ih_file_csv_t *csv, unsigned long object_index, char *field_name);

unsigned short ih_file_csv_get_value_by_name_as_unsigned_short
(ih_file_csv_t *csv, unsigned long object_index, char *field_name);

#endif
