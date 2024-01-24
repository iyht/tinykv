#include "index.h"


bool skiplist_indexer_put(void *self, char *key, struct log_record_pos *pos) {
    struct skiplist_indexer *indexer = (struct skiplist_indexer*)self;
    pthread_mutex_lock(&indexer->mutex);
    insert(indexer->skl, key, pos);
    pthread_mutex_unlock(&indexer->mutex);
    return true;
}

bool skiplist_indexer_del(void *self, char *key) {
    struct skiplist_indexer *indexer = (struct skiplist_indexer*)self;
    pthread_mutex_lock(&indexer->mutex);
    bool ret = erase(indexer->skl, key);
    pthread_mutex_unlock(&indexer->mutex);
    return ret;
}

struct log_record_pos* skiplist_indexer_get(void *self, char *key) {
    struct skiplist_indexer *indexer = (struct skiplist_indexer*)self;
    pthread_mutex_lock(&indexer->mutex);
    struct list_node* node = search(indexer->skl, key);
    if(node == NULL) {
        pthread_mutex_unlock(&indexer->mutex);
        return NULL;
    }
    struct log_record_pos *ret = (struct log_record_pos *)(node->val);
    pthread_mutex_unlock(&indexer->mutex);
    return ret;
}

struct indexer* create_skiplist_indexer() {
    struct skiplist_indexer *indexer = (struct skiplist_indexer*)malloc(sizeof(struct skiplist_indexer));
    indexer->op.put = skiplist_indexer_put;
    indexer->op.del = skiplist_indexer_del;
    indexer->op.get = skiplist_indexer_get;
    indexer->skl = create_skiplist();
    pthread_mutex_init(&indexer->mutex, NULL);
    return (struct indexer *)indexer;
}

void destroy_skiplist_indexer(struct indexer *indexer) {
    struct skiplist_indexer *skiplist_indexer = (struct skiplist_indexer *)indexer;
    pthread_mutex_destroy(&skiplist_indexer->mutex);
    skiplist_free(skiplist_indexer->skl);
    free(skiplist_indexer);
}