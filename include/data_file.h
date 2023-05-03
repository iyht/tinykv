#ifndef HEADER_DATA_FILE
#define HEADER_DATA_FILE

#include <stdlib.h>
#include <stdio.h>
#include "fio.h"
#include "log_record.h"

// data file used for storing data
struct data_file{
    char *file_path;
    int file_id;
    int64_t write_offset;
    struct io_manager* io; 
};

struct data_file* open_data_file(char *dir_path, int file_id);
struct log_record* read_log_record(struct data_file* self, int64_t offset);
int data_file_write(struct data_file* self, char *buf, int64_t size);
void data_file_sync(struct data_file* self);
void data_file_delete(struct data_file* self);
void data_file_close(struct data_file *self);

#endif