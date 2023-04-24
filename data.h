#ifndef HEADER_DATA
#define HEADER_DATA

#include <stdint.h>
#include "fio.h"


typedef char log_record_type;
const log_record_type LOG_RECORD_NORMAL = 1;
const log_record_type LOG_RECORD_DELETED = 2;
const int64_t HEADER_SIZE = 2 * sizeof(int32_t) + sizeof(log_record_type);
struct log_record{
    int32_t key_size;
    int32_t val_size;
    log_record_type type;
    char *key;
    char *val;
};

int64_t get_size(struct log_record* self){
    return self->key_size + self->val_size + HEADER_SIZE;
}

void encode_log_record(struct log_record *record, char **buf, int64_t *size){
    *size = get_size(record);
    *buf = (char*)malloc(*size);

    (*buf)[0] = (record->key_size >> 0) & 0xff;
    (*buf)[1] = (record->key_size >> 8) & 0xff;
    (*buf)[2] = (record->key_size >> 16) & 0xff;
    (*buf)[3] = (record->key_size >> 24) & 0xff;

    (*buf)[4] = (record->val_size >> 0) & 0xff;
    (*buf)[5] = (record->val_size >> 8) & 0xff;
    (*buf)[6] = (record->val_size >> 16) & 0xff;
    (*buf)[7] = (record->val_size >> 24) & 0xff;

    (*buf)[8] = record->type;
    strncpy((*buf) + HEADER_SIZE, record->key, record->key_size);
    strncpy((*buf) + HEADER_SIZE + record->key_size, record->val, record->val_size);
    printf("total size: %ld, headesize: %ld\n", *size, HEADER_SIZE);
}

void decode_log_record(char* buf, struct log_record *record){
    record->key_size = (buf[0] << 0) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    record->val_size = (buf[4] << 0) | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);
    record->type = buf[8];
    record->key = (char*)malloc(record->key_size);
    record->val = (char*)malloc(record->val_size);
    strncpy(record->key, buf + HEADER_SIZE, record->key_size);
    strncpy(record->val, buf + HEADER_SIZE + record->key_size, record->val_size);
}

// log_record_pos descript the position of a data in the disk
struct log_record_pos{
    int file_id; // which file this record is in.
    int64_t offset;  // offset in the file.
};

// data file use for kvdb
struct data_file{
    int file_id;
    int64_t write_offset;
    struct io_manager* io; 
};

struct data_file* open_data_file(){
    return NULL;
}

void data_file_write(struct data_file* self, char *buf){
    return;
}

void data_file_sync(struct data_file* self){
    return;
}

struct log_record* data_file_read(struct data_file* self, int64_t offset){
    return NULL;
}

#endif