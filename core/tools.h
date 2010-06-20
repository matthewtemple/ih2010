#ifndef ih_core_tools_h
#define ih_core_tools_h

#include "ih/core/bit.h"
#include "ih/core/types.h"
#include "ih/external/external.h"

void ih_core_do_nothing();

#define ih_core_get_bit(value, bit_index) ((value >> bit_index) & 1)

#define ih_core_hash(string) ih_core_hasih_djb2_xor(string)

#define ih_core_max(a, b) ((a > b) ? a : b)

#define ih_core_min(a, b) ((a < b) ? a : b)

#define ih_core_trace(message, ...) fprintf(stderr, message "() in %s:%d\n", \
      ##__VA_ARGS__, __FILE__, __LINE__)

#define ih_core_trace_exit(message, ...) fprintf                         \
  (stderr, message"() in %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__);  \
  exit(22);

extern ih_core_bool_t ih_core_stop_requested;

/*
  TODO: rename ih_core_toss_coin()
*/
unsigned short ih_core_coin_toss();

ih_core_bool_t ih_core_get_current_time_string
(ih_core_time_string_t time_string);

double ih_core_gray_coin_toss();

unsigned long ih_core_hasih_djb2(char *string);

unsigned long ih_core_hasih_djb2_xor(char *string);

unsigned long ih_core_hasih_sdbm(char *string);

ih_core_bool_t ih_core_key_hit();

void ih_core_note_maximum_double(double *maximum, double candidate);

void ih_core_note_maximum_unsigned_long(unsigned long *maximum,
    unsigned long candidate);

double ih_core_percentage(double part, double whole);

void ih_core_request_stop();

void ih_core_seed_random(unsigned long seed);

void ih_core_seed_random_witih_time();

void ih_core_set_bit_in_unsigned_char(unsigned char *c,
    unsigned short bit_index, ih_core_bit_t bit_value);

void ih_core_show_memory(void *location, unsigned short length);

char *ih_core_string_append(char *original, const char *addition);

char *ih_core_string_append_char(char *original, char addition);

char *ih_core_string_append_multiple(char *original, ...);

char *ih_core_string_append_n(char *original, char *addition,
    unsigned long addition_size);

char *ih_core_substring(char *string, unsigned short max_length);

void ih_core_terminal_block();

void ih_core_terminal_nonblock();

/*!
 * determine if time is remaining, or has elapsed.
 *
 * \param start_time pointer to an already-initialized timeval containing the
 *        start time.
 * \param max_wall_time_microseconds the number of microseconds after which
 *        this function will consider that time is not remaining.
 *
 * \return whether or not time is remaining
 */
ih_core_bool_t ih_core_time_is_remaining_microseconds
(struct timeval *start_time, unsigned long max_wall_time_microseconds);

void ih_core_truncate_string(char *string, unsigned short max_length);

unsigned long ih_core_wrap_index(long virtual_index, unsigned long range);

#endif
