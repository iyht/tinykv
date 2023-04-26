#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
// #include "../include/data_file.h"
#include "../include/kvdb.h"
#include <assert.h>


int main(){

    char dir_path[] = "./db";
    struct option opt = {.dir_path = dir_path, .data_file_size = 1024 * 1024 * 1024};
    struct db *db = db_open(&opt);
    char *key1 = "abc", *val1 = "123";  
    char *key2 = "def", *val2 = "456";
    char *get_val = NULL;

    get_val = get(db, key1);
    if(get_val != NULL){
        db_remove(db, key1);
    }

    get_val = get(db, key2);
    if(get_val != NULL){
        db_remove(db, key2);
    }
    

    get_val = get(db, key1);
    assert(get_val == NULL);
    
    get_val = get(db, key2);
    assert(get_val == NULL);

    put(db, key2, val2);
    get_val = get(db, key2);
    assert(strcmp(get_val, val2) == 0);

    put(db, key1, val1);
    get_val = get(db, key1);
    assert(strcmp(get_val, val1) == 0);

    db_remove(db, key1);
    get_val = get(db, key1);
    assert(get_val == NULL);
    
    struct db *db2 = db_open(&opt);
    get_val = get(db2, key1);
    assert(get_val == NULL);

    get_val = get(db2, key2);
    assert(strcmp(get_val, val2) == 0);

}

