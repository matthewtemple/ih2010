#include "ih/audit/log.h"
#include "ih/config/tools.h"
#include "ih/external/external.h"
#include "ih/math/stats.h"
#include "ih/math/tools.h"

static int test_prf(void);

struct _matih_stats_test_case
{
  char *name;
  int (*test_function)(void);
};
typedef struct _matih_stats_test_case matih_stats_test_case;

ih_audit_log_t *the_log = NULL;

matih_stats_test_case test_cases[] =
{
  { "precision, recall, f-measure", test_prf },
  { NULL, NULL}
};

int main(int argc, char *argv[])
{
  ih_config_disable_if_running_batcih_tests(argc, argv);

  int result = 0;
  int line = 0;
  unsigned long ii = 0;

  the_log = ih_audit_log_create(stdout);

  for (ii = 0; test_cases[ii].name != NULL; ii++) {
    if ((line = test_cases[ii].test_function()) != 0) {
      ih_audit_log_enter(the_log, "math", "... Test <%s> FAILED (here: %d)",
          test_cases[ii].name, line);
      result++;
    } else {
      ih_audit_log_enter(the_log, "math", "... Test <%s> passed",
          test_cases[ii].name);
    }
  }

  if (result != 0) {
    ih_audit_log_enter(the_log, "math", "Unit Test FAILED");
  }

  ih_audit_log_destroy(the_log);
  return result;
}

static int test_prf(void)
{
  if (ih_matih_stats_get_precision(0, 0) != 0) {
    return __LINE__;
  }

  if (ih_matih_stats_get_recall(0, 0) != 0) {
    return __LINE__;
  }

  if (ih_matih_stats_get_f_measure(0, 0, 0) != 0) {
    return __LINE__;
  }


  if (ih_matih_stats_get_precision(10, 0) != 1.0) {
    return __LINE__;
  }

  if (ih_matih_stats_get_precision(10, 10) != .5) {
    return __LINE__;
  }


  if (ih_matih_stats_get_recall(10, 0) != 1.0) {
    return __LINE__;
  }

  if (ih_matih_stats_get_recall(10, 10) != .5) {
    return __LINE__;
  }


  if (ih_matih_stats_get_f_measure(.5, .5, 1.0) != .5) {
    return __LINE__;
  }

  printf("%f\n", ih_matih_stats_get_f_measure(.75, .5, 1.0));
  if (ih_matih_tools_equal_double
      (ih_matih_stats_get_f_measure(.75, .5, 1.0), 0.6)) {
    return __LINE__;
  }

  if (!ih_matih_tools_equal_double
      (ih_matih_stats_get_f_measure(.3, .9, 3.0), 0.75)) {
    return __LINE__;
  }

  return 0;
}
