#ifndef ih_file_mode_h
#define ih_file_mode_h

#include "ih/external/external.h"

enum ih_file_mode_t {
  IH_FILE_MODE_OPEN_FOR_READ = 0,
  IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_WRITE = 1,
  IH_FILE_MODE_OPEN_OR_CREATE_FOR_WRITE_AT_END = 2,
  IH_FILE_MODE_OPEN_FOR_READ_AND_WRITE = 3,
  IH_FILE_MODE_TRUNCATE_OR_CREATE_FOR_READ_AND_WRITE = 4,
  IH_FILE_MODE_OPEN_OR_CREATE_FOR_READ_AND_WRITE_AT_END = 5
};
typedef enum ih_file_mode_t ih_file_mode_t;

#endif
