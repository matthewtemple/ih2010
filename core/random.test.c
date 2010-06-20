#include "ih/core/random.h"
#include "ih/core/tools.h"
#include "ih/external/external.h"

#define IH_CORE_RANDOM_TEST_ITERATIONS 30000

#define IH_CORE_RANDOM_TEST_UNIFORM_PRECISION 0.01

#define IH_CORE_RANDOM_TEST_POISSON_PRECISION 0.05
#define IH_CORE_RANDOM_TEST_POISSON_MEAN 3

#define IH_CORE_RANDOM_TEST_GAUSSIAN_PRECISION 0.05
#define IH_CORE_RANDOM_TEST_GAUSSIAN_MEAN 5

int main(int argc, char *argv[])
{
  unsigned long ii = 0;
  double value = 0.0;
  double total = 0.0;
  double average = 0.0;

  srand(time(0));

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < IH_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = ih_core_random_01();
    total += value;
  }
  average = (total / IH_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - .5);

  if (value > IH_CORE_RANDOM_TEST_UNIFORM_PRECISION) {
    ih_core_trace_exit("FAILED: Uniform random average: %f", average);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < IH_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = (double) ih_core_random_poisson(IH_CORE_RANDOM_TEST_POISSON_MEAN);
    total += value;
  }
  average = (total / IH_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - IH_CORE_RANDOM_TEST_POISSON_MEAN);

  if (value > IH_CORE_RANDOM_TEST_POISSON_PRECISION) {
    ih_core_trace_exit("FAILED: Poisson random average: %f vs %f", average,
        (double) IH_CORE_RANDOM_TEST_POISSON_MEAN);
  }

  value = 0.0;
  total = 0.0;
  average = 0.0;

  for (ii = 0; ii < IH_CORE_RANDOM_TEST_ITERATIONS; ii++) {
    value = ih_core_random_gaussian(IH_CORE_RANDOM_TEST_GAUSSIAN_MEAN, 2.0);
    total += value;
  }
  average = (total / IH_CORE_RANDOM_TEST_ITERATIONS);
  value = fabs(average - IH_CORE_RANDOM_TEST_GAUSSIAN_MEAN);

  if (value > IH_CORE_RANDOM_TEST_GAUSSIAN_PRECISION) {
    ih_core_trace_exit("FAILED: Gaussian random average: %f vs %f",
        average, (double) IH_CORE_RANDOM_TEST_GAUSSIAN_MEAN);
  }

  return 0;
}
