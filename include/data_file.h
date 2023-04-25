#ifndef HEADER_DATA_FILE
#define HEADER_DATA_FILE

#include <stdlib.h>
#include <stdio.h>
#include "fio.h"
#include "log_record.h"

// data file use for kvdb
struct data_file{
    char *file_path;
    int file_id;
    int64_t write_offset;
    struct io_manager* io; 
};

struct data_file* open_data_file(char *dir_path, int file_id){
    char *file_path = (char*)malloc(256);
    sprintf(file_path, "%s%09d", dir_path, file_id);

    struct data_file *df = (struct data_file*)malloc(sizeof(struct data_file));
    df->file_id = file_id;
    df->write_offset = 0;
    df->io = create_file_io(file_path);
    df->file_path = file_path;
    return df;
}

char *read_n_bytes(struct data_file* self, int64_t offset, int64_t n){
    char *buf = (char*)malloc(n);
    self->io->read(self->io, buf, offset, n);
    return buf;
}

struct log_record* data_file_read(struct data_file* self, int64_t offset){
    // read header 
    char *header_buf = read_n_bytes(self, offset, HEADER_SIZE);
    struct log_record_header header = decode_log_record_header(header_buf);

    // read key and value
    int64_t kv_size = header.key_size + header.val_size + HEADER_SIZE;
    char *kv_buf = read_n_bytes(self, offset + HEADER_SIZE, kv_size);

    // construct log record
    struct log_record* record = (struct log_record*)malloc(sizeof(struct log_record));
    record->key_size = header.key_size;
    record->val_size = header.val_size;
    record->type = header.type;
    record->key = kv_buf;
    record->val = kv_buf + header.key_size;
    free(header_buf);
    return record;
}

void data_file_write(struct data_file* self, char *buf, int64_t size){
    // append to the end of the data file
    self->io->write(self->io, buf, size);
    self->write_offset += size;
}

void data_file_sync(struct data_file* self){
    self->io->sync(self->io);
}

void data_file_delete(struct data_file* self){
    self->io->close(self->io);
    remove(self->file_path);
    free(self->file_path);
    free(self);
}

#endif