#include "ih/core/bitarray.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

static void test_from_char();
static void test_from_double();
static void test_from_long();
static void test_from_short();
static void test_from_string();
static void test_get_functions();
static void test_set_functions();

int main(int argc, char *argv[])
{
  ih_core_bitarray_t *bitarray;

  bitarray = ih_core_bitarray_create(4);
  if (!bitarray) {
    ih_core_trace_exit("ih_core_bitarray_create");
  }

  ih_core_bitarray_unset_all(bitarray);
  ih_core_bitarray_print(bitarray);
  while (ih_core_bitarray_increment(bitarray)) {
    ih_core_bitarray_print(bitarray);
  }

  ih_core_bitarray_destroy(bitarray);

  test_get_functions();
  test_set_functions();
  test_from_char();
  test_from_double();
  test_from_long();
  test_from_short();
  test_from_string();

  return 0;
}

static void test_from_char()
{
  ih_core_bitarray_t *bitarray;
  char *string;

  bitarray = ih_core_bitarray_create_from_char('a');
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);

  bitarray = ih_core_bitarray_create_from_char('b');
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("b = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);

  bitarray = ih_core_bitarray_create_from_char('c');
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("c = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_from_double()
{
  ih_core_bitarray_t *bitarray;
  char *string;

  bitarray = ih_core_bitarray_create_from_double(1.5);
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1.5 = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_from_long()
{
  ih_core_bitarray_t *bitarray;
  char *string;

  bitarray = ih_core_bitarray_create_from_long(1025);
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("1025 = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_from_short()
{
  ih_core_bitarray_t *bitarray;
  char *string;

  bitarray = ih_core_bitarray_create_from_short(5);
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("5 = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_from_string()
{
  ih_core_bitarray_t *bitarray = NULL;
  char *string;

  bitarray = ih_core_bitarray_create_from_string("A", strlen("A"));
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("A = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);

  bitarray = ih_core_bitarray_create_from_string("abc", strlen("abc"));
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("abc = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);

  bitarray = ih_core_bitarray_create_from_string("a@#!2323..~",
      strlen("a@#!2323..~"));
  string = ih_core_bitarray_as_binary_string(bitarray);
  if (string) {
    printf("a@#!2323..~ = %s\n", string);
    free(string);
  } else {
    ih_core_trace_exit("ih_core_bitarray_as_binary_string");
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_get_functions()
{
  double float_value = 0.0;
  unsigned long value = 0;
  ih_core_bitarray_t *bitarray = NULL;

  bitarray = ih_core_bitarray_create(8);
  if (!bitarray) {
    ih_core_trace_exit("ih_core_bitarray_create");
  }
  ih_core_bitarray_set_all(bitarray);

  value = ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 7);
  if (value != 255) {
    ih_core_trace_exit("ih_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 255 got %d)", (int) value);
  }

  value = ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 2, 4);
  if (value != 7)  {
    ih_core_trace_exit("ih_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 7 got %d)", (int) value);
  }

  ih_core_bitarray_set_bit(bitarray, 0, 0);
  ih_core_bitarray_set_bit(bitarray, 2, 0);

  value = ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 3);
  if (value != 10)  {
    ih_core_trace_exit("ih_core_bitarray_get_unsigned_long_from_bits() "
        "failed (expected 10 got %d)", (int) value);
  }
  ih_core_bitarray_destroy(bitarray);

  bitarray = ih_core_bitarray_create(4);
  if (!bitarray) {
    ih_core_trace_exit("ih_core_bitarray_create");
  }
  ih_core_bitarray_set_all(bitarray);

  float_value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (float_value != 1.0) {
    ih_core_trace_exit("ih_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  float_value = ih_core_bitarray_get_double_from_bits(bitarray, 1, 3);
  if (float_value != 1.0) {
    ih_core_trace_exit("ih_core_bitarray_get_doublefrom_bits() "
        "failed (expected 1.0 got %f)", float_value);
  }

  ih_core_bitarray_set_bit(bitarray, 0, 0);
  ih_core_bitarray_set_bit(bitarray, 3, 0);

  float_value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 3);
  if (fabs(float_value - (6.0 / 15.0)) > 0.00001) {
    ih_core_trace_exit("ih_core_bitarray_get_doublefrom_bits() "
        "failed (expected %f got %f)", (6.0 / 15.0), float_value);
  }
  ih_core_bitarray_destroy(bitarray);
}

static void test_set_functions()
{
  double value;
  ih_core_bitarray_t *bitarray;

  bitarray = ih_core_bitarray_create(16);
  ih_core_bitarray_set_unsigned_long(bitarray, 0, 15, 31);
  if (ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 31) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  ih_core_bitarray_set_unsigned_long(bitarray, 0, 15, 0);
  if (ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 0) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_unsigned_long() "
        "failed\n", __FUNCTION__, __LINE__);
  }

  ih_core_bitarray_set_unsigned_long(bitarray, 0, 15, 1);
  if (ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) != 1) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_unsigned_long() "
        "failed", __FUNCTION__, __LINE__);
  }

  ih_core_bitarray_set_unsigned_long(bitarray, 0, 15,
      (unsigned long) (pow(2, 16) - 1));
  if (ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15) !=
      (unsigned long) (pow(2, 16) - 1)) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_unsigned_long() "
        "failed (%lu)", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15));
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 0.0);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.0) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 0.1);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.1) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 0.33);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.33) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15),
        value);
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 0.5);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.5) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 0.6478);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 0.6478) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  ih_core_bitarray_set_double(bitarray, 0, 15, 1.0);
  value = ih_core_bitarray_get_double_from_bits(bitarray, 0, 15);
  if (fabs(value - 1.0) > 0.001) {
    ih_core_trace_exit("[%s():%d] ih_core_bitarray_set_double() "
        "failed: (%lu) %f", __FUNCTION__, __LINE__,
        ih_core_bitarray_get_unsigned_long_from_bits(bitarray, 0, 15), value);
  }

  ih_core_bitarray_destroy(bitarray);
}
