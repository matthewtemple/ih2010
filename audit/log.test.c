#include "ih/audit/log.h"
#include "ih/external/external.h"
#include "ih/file/basic.h"

int main(int argc, char *argv[])
{
  ih_audit_log_t *log;
  ih_file_basic_t *disk_file_a;
  ih_file_basic_t *disk_file_b;
  FILE *disk_file_a_fp;
  FILE *disk_file_b_fp;

  disk_file_a = ih_file_basic_create("audit/a.log",
      IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_a) {
    ih_core_trace_exit("ih_file_basic_create");
  }
  disk_file_a_fp = ih_file_basic_get_file(disk_file_a);

  log = ih_audit_log_create(disk_file_a_fp);
  if (!log) {
    ih_core_trace_exit("ih_audit_log_create");
  }

  ih_audit_log_enter(log, "haud", "::int=%d::double=%f::string=%s::", 11111,
      2.2222, "three");

  disk_file_b = ih_file_basic_create("audit/b.log",
      IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE);
  if (!disk_file_b) {
    ih_core_trace_exit("ih_audit_file_create");
  }
  disk_file_b_fp = ih_file_basic_get_file(disk_file_b);

  if (!ih_audit_log_add_file(log, disk_file_b_fp)) {
    ih_core_trace_exit("ih_audit_log_add_file");
  }

  ih_audit_log_enter(log, "haud", "this should be in both %s.log and %s.log",
      "a", "b");

  if (!ih_audit_log_add_file(log, stdout)) {
    ih_core_trace_exit("ih_audit_log_add_file");
  }

  ih_audit_log_enter(log, "haud", "word");

  ih_audit_log_enter(log, "haud", "The quick brown fox jumps over the lazy dog."
      "  The quick brown fox jumps over the lazy dog.");

  ih_audit_log_destroy(log);
  ih_file_basic_destroy(disk_file_a);
  ih_file_basic_destroy(disk_file_b);

  return 0;
}
