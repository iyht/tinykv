#include "../include/log_record.h"

const log_record_type LOG_RECORD_NORMAL = 1;
const log_record_type LOG_RECORD_DELETED = 2;
const int64_t HEADER_SIZE = 2 * sizeof(int32_t) + sizeof(log_record_type);
const int64_t TIMESTAMP_LEN = 23;



int64_t get_size(struct log_record* self){
    return self->key_size + self->val_size + HEADER_SIZE + TIMESTAMP_LEN;
}

void encode_log_record(struct log_record *record, char **buf, int64_t *size){
    if(!record || record->key_size == 0){
        printf("encode log record failed\n");
    }
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
    strncpy((*buf) + HEADER_SIZE + record->key_size + record->val_size, record->timestamp, TIMESTAMP_LEN);
    printf("encode_log_record %s\n", record->timestamp);
    // printf("total size: %ld, headesize: %ld\n", *size, HEADER_SIZE);
}


struct log_record_header decode_log_record_header(char* buf){
    struct log_record_header header;
    header.key_size = (buf[0] << 0) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    header.val_size = (buf[4] << 0) | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);
    header.type = buf[8];
    return header;
}