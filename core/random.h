#ifndef ih_core_random_h
#define ih_core_random_h

double ih_core_random_01();

double ih_core_random_gaussian(double mean, double stddev);

unsigned long ih_core_random_poisson(double mean);

unsigned long ih_core_random_unsigned_long(unsigned long range);

#endif
