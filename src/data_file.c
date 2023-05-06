#include "../include/data_file.h"
#include <stdio.h>
#include <stdlib.h>

struct data_file* open_data_file(char *dir_path, int file_id){
    char *file_path = (char*)malloc(256);
    sprintf(file_path, "%s/%d.data", dir_path, file_id);

    struct data_file *df = (struct data_file*)malloc(sizeof(struct data_file));
    df->file_id = file_id;
    df->write_offset = 0;
    df->io = create_file_io(file_path);
    df->file_path = file_path;
    return df;
}


static char *read_n_bytes(struct data_file* self, int64_t offset, int64_t n){
    char *buf = (char*)malloc(n);
    int err = self->io->read(self->io, buf, offset, n);
    if(err <= 0) {
//        printf("error or EOF\n");
        return NULL;
    }
    return buf;
}


struct log_record* read_log_record(struct data_file* self, int64_t offset){
    // read header 
    char *header_buf = read_n_bytes(self, offset, HEADER_SIZE);
    if(!header_buf){
//        printf("cannot load header\n");
        return NULL;
    }
    struct log_record_header header = decode_log_record_header(header_buf);

    // read key and value
    int64_t data_size = header.key_size + header.val_size + TIMESTAMP_LEN;
    char *data = read_n_bytes(self, offset + HEADER_SIZE, data_size);
    if(!header_buf) {
        printf("cannot load kv\n");
        return NULL;
    }

    // construct log record
    struct log_record* record = (struct log_record*)malloc(sizeof(struct log_record));
    record->key_size = header.key_size;
    record->val_size = header.val_size;
    record->type = header.type;
    record->key = malloc(header.key_size + 1);
    record->val = malloc(header.val_size + 1);
    record->timestamp = malloc(TIMESTAMP_LEN + 1);
    memcpy(record->key, data, header.key_size);
    memcpy(record->val, data + header.key_size, header.val_size);
    memcpy(record->timestamp, data + header.key_size + header.val_size, TIMESTAMP_LEN);
    record->key[header.key_size] = '\0';
    record->val[header.val_size] = '\0';
    record->timestamp[TIMESTAMP_LEN] = '\0';
    record->total_size = data_size + HEADER_SIZE;
    free(header_buf);
    return record;
}

int data_file_write(struct data_file* self, char *buf, int64_t size){
    // append to the end of the data file
    self->io->write(self->io, buf, size);
    self->write_offset += size;
    return 0;
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

void data_file_close(struct data_file *self){
    self->io->close(self->io);
}
