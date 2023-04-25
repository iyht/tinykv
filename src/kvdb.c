#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/index.h"
#include "../include/data_file.h"
#include "../include/log_record.h"
#include "../include/option.h"

struct db{
    pthread_mutex_t mutex;
    struct indexer *index;
    struct option *opt;
    struct data_file *active_data_file;
    struct data_file **old_data_files;
    int old_data_files_size;
};

static void set_active_data_file(struct db *self){
    int file_id = 0; 
    if(self->active_data_file) file_id = self->active_data_file->file_id + 1;
    self->active_data_file = open_data_file(self->opt->dir_path, 0);
    return;
}

struct log_record_pos* append_log_record(struct db *self, struct log_record *record) {
    if(!record) return NULL;
    char *buf;
    int64_t buf_size;
    encode_log_record(record, &buf, &buf_size);
    if(!buf || buf_size == 0) return NULL;

    if(self->active_data_file->write_offset + buf_size > self->opt->data_file_size) {
        data_file_sync(self->active_data_file);
        self->old_data_files_size++;
        self->old_data_files = realloc(self->old_data_files, self->old_data_files_size * sizeof(struct data_file*));
        self->old_data_files[self->active_data_file->file_id] = self->active_data_file;
        set_active_data_file(self);
    }

    struct log_record_pos *pos = malloc(sizeof(struct log_record_pos));
    pos->file_id = self->active_data_file->file_id;
    pos->offset = self->active_data_file->write_offset;

    // write and persist the data file
    data_file_write(self->active_data_file, buf, buf_size);
    data_file_sync(self->active_data_file);
    return pos;
}

bool put(struct db *self, char *key, void *val) {
    pthread_mutex_lock(&self->mutex);
    if(self == NULL || key == NULL || val == NULL) {
        pthread_mutex_unlock(&self->mutex);
        return false;
    }

    // generate log record
    struct log_record record = {.key = key, 
                                .val = val, 
                                .type = LOG_RECORD_NORMAL};
                            
    // append to the active data file
    struct log_record_pos* pos = append_log_record(self, &record);

    // update the index corresponding log_record_pos
    self->index->put(self->index, key, pos);
    pthread_mutex_unlock(&self->mutex);
    return true;
}

char* get(struct db *self, char *key) {
    pthread_mutex_lock(&self->mutex);
    if(self == NULL || key == NULL) {
        pthread_mutex_unlock(&self->mutex);
        return NULL;
    }

    // fetch the log record position from index
    struct log_record_pos* pos = self->index->get(self->index, key);
    if(pos == NULL) {
        pthread_mutex_unlock(&self->mutex);
        return NULL;
    }

    // select the data file to read
    struct data_file *df;
    if(pos->file_id == self->active_data_file->file_id) df = self->active_data_file;
    else if (pos->file_id < self->old_data_files_size) {
        df = self->old_data_files[pos->file_id];
    } else {
        pthread_mutex_unlock(&self->mutex);
        printf("file_id %d is not found\n", pos->file_id);
        return NULL;
    }

    // according to the log record position, read the log record from disk
    struct log_record* record = data_file_read(df, pos->offset);
    if (record && record->type == LOG_RECORD_NORMAL) {
        pthread_mutex_unlock(&self->mutex);
        return record->val;
    }

    pthread_mutex_unlock(&self->mutex);
    return NULL;
}


