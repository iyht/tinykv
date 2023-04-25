#ifndef HEADER_DATA
#define HEADER_DATA

#include <stdint.h>
#include "fio.h"


typedef char log_record_type;
extern const log_record_type LOG_RECORD_NORMAL;
extern const log_record_type LOG_RECORD_DELETED;
extern const int64_t HEADER_SIZE;

struct log_record_header{
    int32_t key_size;
    int32_t val_size;
    log_record_type type;
};

struct log_record{
    int32_t key_size;
    int32_t val_size;
    log_record_type type;
    char *key;
    char *val;
    int64_t total_size;
};

// log_record_pos descript the position of a data in the disk
struct log_record_pos{
    int file_id; // which file this record is in.
    int64_t offset;  // offset in the file.
};


void encode_log_record(struct log_record *record, char **buf, int64_t *size);
struct log_record_header decode_log_record_header(char* buf);





// TODO free log_record


#endif