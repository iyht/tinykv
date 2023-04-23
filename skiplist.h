
const static int LEVEL = 8;
const static int KEY_SIZE = 128;
const static int VAL_SIZE = 256;

struct list_node{
    char *key;
    void *val;
    struct list_node **next;
};

struct skiplist {
    struct list_node* head;
};


struct skiplist* create_skiplist();
struct list_node* search(struct skiplist* self, char* key);
void insert(struct skiplist* self, char* key, void* val);
bool erase(struct skiplist* self, char* key);
void skiplist_free(struct skiplist* self);
void print_list_node(struct list_node* node);
void node_free(struct list_node*node);
void skiplist_free(struct skiplist* self);