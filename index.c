#include "data.h"
#include "stdbool.h"
#include "pthread.h"
#include "skiplist.h"
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

bool skiplist_indexer_put(void *self, char *key, struct log_record_pos *pos) {
    struct skiplist_indexer *indexer = self;
    pthread_mutex_lock(&indexer->mutex);
    insert(indexer->skl, key, pos);
    pthread_mutex_unlock(&indexer->mutex);
    return true;
}

bool skiplist_indexer_del(void *self, char *key) {
    struct skiplist_indexer *indexer = self;
    pthread_mutex_lock(&indexer->mutex);
    bool ret = erase(indexer->skl, key);
    pthread_mutex_unlock(&indexer->mutex);
    return ret;
}

struct log_record_pos* skiplist_indexer_get(void *self, char *key) {
    struct skiplist_indexer *indexer = self;
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
    struct skiplist_indexer *indexer = malloc(sizeof(struct skiplist_indexer));
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

int main(){
    struct indexer *indexer = create_skiplist_indexer();
    struct log_record_pos *pos0 = malloc(sizeof(struct log_record_pos));
    pos0->file_id = 0;
    pos0->offset = 0;

    struct log_record_pos *pos1 = malloc(sizeof(struct log_record_pos));
    pos1->file_id = 1;
    pos1->offset = 1;

    struct log_record_pos *pos2 = malloc(sizeof(struct log_record_pos));
    pos2->file_id = 2;
    pos2->offset = 2;


    indexer->put(indexer, "0", pos0);
    struct log_record_pos *ret = indexer->get(indexer, "0");
    printf("%d %lld\n", ret->file_id, ret->offset);

    ret = indexer->get(indexer, "null");
    if(ret == NULL) printf("null\n");
    else printf("%d %lld\n", ret->file_id, ret->offset);

    indexer->put(indexer, "1", pos1);
    ret = indexer->get(indexer, "1");
    printf("%d %lld\n", ret->file_id, ret->offset);

    indexer->put(indexer, "2", pos2);
    ret = indexer->get(indexer, "2");
    printf("%d %lld\n", ret->file_id, ret->offset);

    destroy_skiplist_indexer(indexer);
    return 0;
}