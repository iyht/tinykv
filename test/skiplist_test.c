#include "../src/skiplist.h"
#include <assert.h>


void test_insert(){
    struct skiplist *self = create_skiplist();
    struct list_node* search_res = NULL;
    search_res = search(self, "1");
    assert(search_res  == NULL);

    char *key1, *key2, *val1, *val2;
    key1 = malloc(sizeof(char) * KEY_SIZE);
    key2 = malloc(sizeof(char) * KEY_SIZE);
    val1 = malloc(sizeof(char) * VAL_SIZE);
    val2 = malloc(sizeof(char) * VAL_SIZE);
    strcpy(key1, "1");
    strcpy(key2, "2");
    strcpy(val1, "1val");
    strcpy(val2, "2val");

    insert(self, key1, val1);
    insert(self, key2, val2);
    search_res = search(self, "2");
    assert(search_res != NULL);
    assert(strcmp(search_res->val, "2val") == 0);
    assert(strcmp(search_res->key, "2") == 0);

    search_res = search(self, "1");
    assert(strcmp(search_res->val, "1val") == 0);
    assert(strcmp(search_res->key, "1") == 0);
    skiplist_free(self);

    printf("test_insert passed\n");
}

void test_erase(){
    struct skiplist *self = create_skiplist();
    struct list_node* search_res = NULL;
    char *key1, *key2, *val1, *val2;
    key1 = malloc(sizeof(char) * KEY_SIZE);
    key2 = malloc(sizeof(char) * KEY_SIZE);
    val1 = malloc(sizeof(char) * VAL_SIZE);
    val2 = malloc(sizeof(char) * VAL_SIZE);
    strcpy(key1, "1");
    strcpy(key2, "2");
    strcpy(val1, "1val");
    strcpy(val2, "2val");

    insert(self, key1, val1);
    search_res = search(self, "1");
    assert(search_res != NULL);

    erase(self, "1");
    search_res = search(self, "1");
    assert(search_res == NULL);

    printf("test_erase passed\n");
}

void test_insert_replace(){
    struct skiplist *self = create_skiplist();
    struct list_node* search_res = NULL;
    char *key1, *val1, *val2;
    key1 = malloc(sizeof(char) * KEY_SIZE);
    val1 = malloc(sizeof(char) * VAL_SIZE);
    val2 = malloc(sizeof(char) * VAL_SIZE);
    strcpy(key1, "1");
    strcpy(val1, "1val");
    strcpy(val2, "2val");

    insert(self, key1, val1);
    search_res = search(self, "1");
    assert(search_res != NULL);
    assert(strcmp(search_res->val, "1val") == 0);
    assert(strcmp(search_res->key, "1") == 0);

    insert(self, key1, val2);
    search_res = search(self, "1");
    assert(search_res != NULL);
    assert(strcmp(search_res->val, "2val") == 0);
    assert(strcmp(search_res->key, "1") == 0);

    erase(self, "1");
    search_res = search(self, "1");
    assert(search_res == NULL);
    
    skiplist_free(self);
    printf("test_insert_replace passed\n");
}

int main(){
    test_insert();
    test_erase();
    test_insert_replace();
    printf("test skiplist passed\n");
}
