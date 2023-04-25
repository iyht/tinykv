#ifndef HEADER_INDEX
#define HEADER_INDEX
#include "../include/log_record.h"
#include "stdbool.h"
#include "pthread.h"
#include "../include/skiplist.h"
#include <stdlib.h>
#include <stdio.h>

struct indexer {
    bool (*put)(void *self, char *key, struct log_record_pos *pos);
    bool (*del)(void *self, char *key);
    struct log_record_pos* (*get)(void *self, char *key);
};


struct skiplist_indexer {
    struct indexer op;
    struct skiplist *skl;
    pthread_mutex_t mutex;
};

bool skiplist_indexer_put(void *self, char *key, struct log_record_pos *pos);
bool skiplist_indexer_del(void *self, char *key);
struct log_record_pos* skiplist_indexer_get(void *self, char *key);
struct indexer* create_skiplist_indexer();
void destroy_skiplist_indexer(struct indexer *indexer);



#endif

