#include <stdint.h>

struct log_record_pos{
    int file_id; // which file this record is in.
    int64_t offset;  // offset in the file.
};