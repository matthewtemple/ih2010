#include "ih/external/external.h"
#include "ih/math/tools.h"

double ih_matih_tools_calculate_distance(double x1, double y1, double x2,
    double y2)
{
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

ih_core_bool_t ih_matih_tools_equal_double(double a, double b)
{
  ih_core_bool_t equal;

  if (abs(a - b) < 0.0000001) {
    equal = ih_core_bool_true;
  } else {
    equal = ih_core_bool_false;
  }

  return equal;
}
