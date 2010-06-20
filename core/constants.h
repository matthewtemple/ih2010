#ifndef ih_core_constants_h
#define ih_core_constants_h

#define IH_CORE_SECONDS_PER_MINUTE 60
#define IH_CORE_SECONDS_PER_HOUR 3600
#define IH_CORE_SECONDS_PER_DAY 86400

#define IH_CORE_BYTES_IN_A_KILOBYTE 1024
#define IH_CORE_BYTES_IN_A_MEGABYTE 1048576
#define IH_CORE_BYTES_IN_A_GIGABYTE 1073741824

#define IH_CORE_MAX_COLOR 2147483647

#define IH_CORE_STANDARD_SLEEP_MICROSECONDS 1024

#define IH_CORE_WHITESPACE_DELIMITERS " \t\n\r"

#define IH_CORE_DEMO_SOLUTION_COUNT 1

/*!
 * the number of bits in a class.  classes are the results of classifications.
 */
#define IH_CORE_CLASS_BIT_COUNT 32

/*!
 * the number of bits in a solution.  solutions are what searches find; they
 * are also what is used to morph.
 */
#define IH_CORE_SOLUTION_BIT_COUNT 512
#define IH_CORE_SOLUTION_BIT_COUNT_2N 9

#endif
