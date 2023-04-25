#include "../include/index.h"
#include <assert.h>

void test_get(){
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
    assert(ret != NULL);
    assert(ret->file_id == 0);
    assert(ret->offset == 0);
    assert(ret->offset == 0);

    ret = indexer->get(indexer, "null");
    assert(ret == NULL);

    indexer->put(indexer, "1", pos1);
    ret = indexer->get(indexer, "1");
    assert(ret != NULL);
    assert(ret->file_id == 1);
    assert(ret->offset == 1);
    assert(ret->offset == 1);

    indexer->put(indexer, "2", pos2);
    ret = indexer->get(indexer, "2");
    assert(ret != NULL);
    assert(ret->file_id == 2);
    assert(ret->offset == 2);
    assert(ret->offset == 2);

    destroy_skiplist_indexer(indexer);
}

int main(){
    test_get();
    printf("test_get passed\n");

}