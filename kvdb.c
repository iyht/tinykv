#include "index.h"
#include "data.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

struct db{
    pthread_mutex_t mutex;
    struct data_file *active_data_file;
    struct indexer *index;
};


struct log_record_pos* append_log_record(struct db *self, struct log_record *record) {
    if(!record) {
        return NULL;
    }

    // check the active log file exists
}

bool db_put(struct db *self, char *key, void *val) {

    if(self == NULL || key == NULL || val == NULL) {
        return false;
    }

    // generate log record
    struct log_record record = {.key = key, 
                                .val = val, 
                                .type = LOG_RECORD_NORMAL};
                            
    // append to the active data file
    struct log_record_pos* pos = append_log_record(self, &record);

    // update the corresponding log_record_pos in the cache
    self->index->put(self->index, key, pos);
    return true;
}

char* db_get(struct db *self, char *key) {
    if(self == NULL || key == NULL) {
        return NULL;
    }

    // fetch the log record position from index
    struct log_record_pos* pos = self->index->get(self->index, key);
    if(pos == NULL) {
        return NULL;
    }

    // select the data file to read
    struct data_file* df = self->active_data_file;

    // according to the log record position, read the log record
    struct log_record* record = data_file_read(df, pos->offset);
    if (record && record->type == LOG_RECORD_NORMAL) {
        return record->val;
    }
    return NULL;
}


