#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct Node{
    char *key;
    char *val;
    struct Node **next;
};

typedef struct {
    struct Node* head;
} Skiplist;

const static int LEVEL = 8;
const static int KEY_SIZE = 128;
const static int VAL_SIZE = 256;

void nodeFree(struct Node* node);

struct Node* CreateNode(char *k, char *v){
    struct Node* node= malloc(sizeof(struct Node));
    node->key = k;
    node->val = v;
    node->next = malloc(sizeof(struct Node*) * LEVEL);
    for(int i =0 ; i < LEVEL; i++){
        node->next[i] = NULL;
    }
    return node;
}

Skiplist* CreateSkiplist() {
    Skiplist *ret = malloc(sizeof(Skiplist));
    char *key = malloc(sizeof(char) * KEY_SIZE);
    char *val = malloc(sizeof(char) * 1);
    ret->head = CreateNode(key, val);
    return ret;
}





void find(Skiplist* obj, char *target, struct Node **pre){
    struct Node *cur = obj->head;
    for(int i = LEVEL - 1; i >= 0; i--){
        while(cur->next[i] != NULL && strcmp(cur->next[i]->key, target) < 0){
            cur = cur->next[i];
        }
        pre[i] = cur;
    }
}

bool Search(Skiplist* obj, int target) {
    char *target_str = malloc(sizeof(char) * KEY_SIZE);
    sprintf(target_str, "%d", target);
    struct Node **pre = malloc(sizeof(struct Node*) * LEVEL);

    find(obj, target_str, pre);
    struct Node *cur = pre[0]->next[0];
    return cur && strcmp(cur->key, target_str) == 0;
}

void Insert(Skiplist* obj, int num) {
    char *num_str = malloc(sizeof(char) * KEY_SIZE);
    sprintf(num_str, "%d", num);
    struct Node **pre = malloc(sizeof(struct Node*) * LEVEL);
    find(obj, num_str, pre);
    char *val = malloc(sizeof(char) * VAL_SIZE);
    sprintf(val, "%d", num);

    struct Node *new_node = CreateNode(num_str, val);
    for(int i = 0; i < LEVEL; i++){
        new_node->next[i] = pre[i]->next[i];
        pre[i]->next[i] = new_node;
        if(rand() % 2 == 0){
            break;
        }
    }
    free(pre);
}

bool skiplistErase(Skiplist* obj, int num) {
    char *num_str = malloc(sizeof(char) * KEY_SIZE);
    sprintf(num_str, "%d", num);
    struct Node **pre = malloc(sizeof(struct Node*) * LEVEL);

    find(obj, num_str, pre);
    struct Node *cur = pre[0]->next[0];
    if(!cur || strcmp(cur->key, num_str) != 0) return false;
    for(int i = 0; i < LEVEL && pre[i]->next[i] == cur; i ++){
        pre[i]->next[i] = cur->next[i];
    }
    nodeFree(cur);
    free(pre);
    return true;
}

void nodeFree(struct Node* node){
    free(node->key);
    free(node->val);
    free(node->next);
    free(node);
}

void skiplistFree(Skiplist* obj) {
    struct Node *pre = obj->head, *cur = pre->next[0];
    while(pre){
        nodeFree(pre);
        pre = cur;
        if(!cur) break;
        cur = cur->next[0];
    }
}

/**
 * Your Skiplist struct will be instantiated and called as such:
 * Skiplist* obj = CreateSkiplist();
 * bool param_1 = Search(obj, target);
 
 * Insert(obj, num);
 
 * bool param_3 = skiplistErase(obj, num);
 
 * skiplistFree(obj);
*/


int main(){
    Skiplist *obj = CreateSkiplist();
    bool param_1, param_2;
    param_1 = Search(obj, 1);
    Insert(obj, 1);
    Insert(obj, 2);
    param_2 = Search(obj, 2);

    printf("%d %d\n", param_1, param_2);
    return 0;
}