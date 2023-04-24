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

    struct log_record record = {.key = key, 
                                .val = val, 
                                .type = LOG_RECORD_NORMAL};
                            
    struct log_record_pos* pos = append_log_record(self, &record);
    self->index->put(self->index, key, pos);
    return true;
}

char* db_get(struct db *self, char *key) {
    if(self == NULL || key == NULL) {
        return NULL;
    }

    struct log_record_pos* pos = self->index->get(self->index, key);
    if(pos == NULL) {
        return NULL;
    }

    struct data_file* df = self->active_data_file;

    struct log_record* record = data_file_read(df, pos->offset);
    if (record && record->type == LOG_RECORD_NORMAL) {
        return record->val;
    }
    return NULL;
}


