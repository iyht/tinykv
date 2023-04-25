#include "../include/skiplist.h"


void node_free(struct list_node* node);

struct list_node* create_node(char *k, void *v){
    struct list_node* node= malloc(sizeof(struct list_node));
    node->key = malloc(KEY_SIZE);
    strcpy(node->key, k);
    node->val = v;
    node->next = malloc(sizeof(struct list_node*) * LEVEL);
    for(int i =0 ; i < LEVEL; i++){
        node->next[i] = NULL;
    }
    return node;
}

struct skiplist* create_skiplist() {
    struct skiplist *ret = malloc(sizeof(struct skiplist));
    char *key = malloc(sizeof(char) * KEY_SIZE);
    char *val = malloc(sizeof(char) * 1);
    ret->head = create_node(key, val);
    return ret;
}





static void find(struct skiplist* self, char *target, struct list_node **pre){
    struct list_node *cur = self->head;
    for(int i = LEVEL - 1; i >= 0; i--){
        while(cur->next[i] != NULL && strcmp(cur->next[i]->key, target) < 0){
            cur = cur->next[i];
        }
        pre[i] = cur;
    }
}

struct list_node* search(struct skiplist* self, char* key) {
    struct list_node **pre = malloc(sizeof(struct list_node*) * LEVEL);

    find(self, key, pre);
    struct list_node *cur = pre[0]->next[0];
    if(cur && strcmp(cur->key, key) == 0) {
        return cur;
    }
    return NULL;
}

void insert(struct skiplist* self, char* key, void *val) {
    struct list_node **pre = malloc(sizeof(struct list_node*) * LEVEL);
    find(self, key, pre);

    struct list_node *new_node = create_node(key, val);
    for(int i = 0; i < LEVEL; i++){
        new_node->next[i] = pre[i]->next[i];
        pre[i]->next[i] = new_node;
        if(rand() % 2 == 0){
            break;
        }
    }
    free(pre);
}

bool erase(struct skiplist* self, char* key) {
    struct list_node **pre = malloc(sizeof(struct list_node*) * LEVEL);

    find(self, key, pre);
    struct list_node *cur = pre[0]->next[0];
    if(!cur || strcmp(cur->key, key) != 0) return false;
    for(int i = 0; i < LEVEL && pre[i]->next[i] == cur; i ++){
        pre[i]->next[i] = cur->next[i];
    }
    node_free(cur);
    free(pre);
    return true;
}

void node_free(struct list_node* node){
    free(node->key);
    free(node->val);
    free(node->next);
    free(node);
}

void skiplist_free(struct skiplist* self) {
    struct list_node *pre = self->head, *cur = pre->next[0];
    while(pre){
        node_free(pre);
        pre = cur;
        if(!cur) break;
        cur = cur->next[0];
    }
}

void print_list_node(struct list_node* node){
    if(node) printf("key: %s, val: %s\n", node->key, (char*)node->val);
    else printf("node is null\n");
}
