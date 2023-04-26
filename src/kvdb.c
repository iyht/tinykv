
#include "../include/kvdb.h"
#include <pthread.h>
struct engine{
    pthread_mutex_t mutex;
    struct indexer *index;
    struct option *opt;
    struct data_file *active_data_file;
    struct data_file **old_data_files;
    int old_data_files_size;
};

static void load_data_files(struct engine *self);
static void load_index_from_data_files(struct engine *self);


int filter(const struct dirent *entry){
    return strstr(entry->d_name, ".data") != NULL;
}

struct engine* engine_open(struct option *opt) {
    struct engine *db = malloc(sizeof(struct engine));
    db->opt = opt;
    db->index = create_skiplist_indexer();
    db->old_data_files = NULL;
    db->old_data_files_size = 0;
    db->active_data_file = NULL;
    pthread_mutex_init(&db->mutex, NULL);

    // check the data files directory exists or not
    struct stat st = {0};
    if(stat(opt->dir_path, &st) == -1) {
        mkdir(opt->dir_path, 0777);
    }


    // load data files
    load_data_files(db);
    
    // load index
    load_index_from_data_files(db);
    return db;
}



static void load_data_files(struct engine *self) {

    // load all data files under self->opt->dir_path
    // sort the loaded data files by its filename from small to large
    struct dirent **namelist;
    int n = scandir(self->opt->dir_path, &namelist, filter, alphasort);
    if(n < 0) return;

    // store all the loaded data files in self->old_data_files
    // set self->active_data_file to the last data file
    // update self->old_data_files_size
    self->old_data_files = malloc((n - 1) * sizeof(struct data_file*));
    self->old_data_files_size = n - 1;
    for(int i = 0; i < n; i++){
        int file_id;
        sscanf(namelist[i]->d_name, "%d.data", &file_id);

        struct data_file* df = open_data_file(self->opt->dir_path, file_id);
        if (i == n - 1){
            self->active_data_file = df;
        } else {
            self->old_data_files[file_id] = df;
        }
    }

}


static void load_index_from_data_files(struct engine *self){
//    printf("load_index_from_data_files called\n");
    for(int i = 0; i < self->old_data_files_size + 1; i++){
        struct data_file *df;
        if(i == self->old_data_files_size){
            df = self->active_data_file;
        } else {
            df = self->old_data_files[i];
        }
        int64_t offset = 0;
        while(true){
            struct log_record *record = read_log_record(df, offset);
            if(!record) break;

            struct log_record_pos* pos = malloc(sizeof(struct log_record_pos));
            pos->file_id = df->file_id;
            pos->offset = offset;

            if(record->type == LOG_RECORD_NORMAL){
                self->index->put(self->index, record->key, (void*)pos);
            } else if(record->type == LOG_RECORD_DELETED){
                self->index->del(self->index, record->key);
            }
            offset += record->total_size;
            free(record);
        }
        if(df == self->active_data_file){
            df->write_offset = offset;
        }
    }


}

void set_active_data_file(struct engine *self){
    int file_id = 0; 
    if(self->active_data_file) file_id = self->active_data_file->file_id + 1;
    self->active_data_file = open_data_file(self->opt->dir_path, 0);
    return;
}

struct log_record_pos* append_log_record(struct engine *self, struct log_record *record) {
    if(!record) return NULL;
    char *buf;
    int64_t buf_size;
    encode_log_record(record, &buf, &buf_size);
    if(!buf || buf_size == 0){
        printf("encode log record failed\n");
        return NULL;
    }

    if(self->active_data_file == NULL) {
        set_active_data_file(self);
    }

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

bool engine_put(struct engine *self, char *key, char *val) {
    pthread_mutex_lock(&self->mutex);
    if(self == NULL || key == NULL || val == NULL) {
        pthread_mutex_unlock(&self->mutex);
        return false;
    }

    // generate log record
    struct log_record record = {.key = key,
                                .val = val, 
                                .type = LOG_RECORD_NORMAL,
                                .key_size = strlen(key),
                                .val_size = strlen(val),
                                .total_size = HEADER_SIZE + strlen(key) + strlen(val) + TIMESTAMP_LEN};

    record.timestamp = malloc(TIMESTAMP_LEN + 1);
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(record.timestamp, 24, "%Y-%m-%d %H:%M:%S.000", tm);
//    printf("engine_put: key=%s, val=%s, timestamp=$s\n", record.key, record.val, record.timestamp);

    // append to the active data file
    struct log_record_pos* pos = append_log_record(self, &record);

    // update the index corresponding log_record_pos
    self->index->put(self->index, key, pos);
    pthread_mutex_unlock(&self->mutex);
    return true;
}

struct log_record* engine_get(struct engine *self, char *key) {
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
        printf("file_id %d is not found\n", pos->file_id);
        pthread_mutex_unlock(&self->mutex);
        return NULL;
    }

    // according to the log record position, read the log record from disk
    struct log_record* record = read_log_record(df, pos->offset);
    if (record && record->type == LOG_RECORD_NORMAL) {
        pthread_mutex_unlock(&self->mutex);
        return record;
    }

    pthread_mutex_unlock(&self->mutex);
    return NULL;
}


bool engine_remove(struct engine* self, char *key){
    pthread_mutex_lock(&self->mutex);
    // check if the key in the index
    struct log_record_pos* pos = self->index->get(self->index, key);
    if(pos == NULL) {
        pthread_mutex_unlock(&self->mutex);
        return false;
    }

    // generate log record
    struct log_record record = {.key = key, 
                                .val = NULL, 
                                .type = LOG_RECORD_DELETED,
                                .key_size = strlen(key),
                                .val_size = 0,
                                .total_size = HEADER_SIZE + strlen(key) + TIMESTAMP_LEN};

    record.timestamp = malloc(TIMESTAMP_LEN + 1);
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(record.timestamp, 24, "%Y-%m-%d %H:%M:%S.000", tm);

    // append to the active data file
    struct log_record_pos* new_pos = append_log_record(self, &record);

    // remove the log position from index
    pthread_mutex_unlock(&self->mutex);
    return self->index->del(self->index, key);
}

char *engine_timestamp(struct engine* self, char *key){
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
}
