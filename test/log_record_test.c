#include "../src/log_record.h"
#include "../src/data_file.h"
#include "string.h"
#include <assert.h>
#include <time.h>

int main(){
    char *key = "how are you doing my bro";
    char *val = "I'm pretty good thanks for asking";
    char *timestamp = malloc(TIMESTAMP_LEN);
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(timestamp, 24, "%Y-%m-%d %H:%M:%S.000", tm);

    struct log_record write_record = {.key_size = strlen(key), 
                                .val_size = strlen(val), 
                                .type = LOG_RECORD_NORMAL,
                                .timestamp = timestamp,
                                .key = key,
                                .val = val};

    char* buf;
    int64_t size;
    encode_log_record(&write_record, &buf, &size);
    assert(buf != NULL);
    assert(size != 0);

    struct data_file* df = open_data_file(".", 900099);
    assert(df != NULL);
    assert(df->write_offset == 0);
    assert(df->file_id = 9);

    data_file_write(df, buf, size);
    assert(df->write_offset == size);


    struct log_record *read_record = read_log_record(df, 0);
    assert(read_record != NULL);
    assert(read_record->key_size == write_record.key_size);
    assert(read_record->val_size == write_record.val_size);
    assert(read_record->type == write_record.type);
    assert(strncmp(read_record->key, write_record.key, write_record.key_size) == 0);
    assert(strncmp(read_record->val, write_record.val, write_record.val_size) == 0);
    printf("read %s\n", read_record->timestamp);
    assert(strncmp(read_record->timestamp, write_record.timestamp, TIMESTAMP_LEN) == 0);
    data_file_delete(df);

    printf("test log_record passed!\n");
}