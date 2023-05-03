#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../include/engine.h"
#include <assert.h>


int main(){

    char dir_path[] = "./db";
    struct option opt = {.dir_path = dir_path, .data_file_size = 1024 * 1024 * 1024};
    struct engine *db = engine_open(&opt);
    char *key1 = "abc", *val1 = "123";  
    char *key2 = "def", *val2 = "456";
    char *get_val = NULL;

    if(engine_get(db, key1) != NULL){
        engine_remove(db, key1);
    }

    if(engine_get(db, key2) != NULL){
        engine_remove(db, key2);
    }
    

    assert(engine_get(db, key1) == NULL);
    assert(engine_get(db, key2) == NULL);

    engine_put(db, key2, val2);
    get_val = engine_get(db, key2)->val;
    assert(strcmp(get_val, val2) == 0);

    engine_put(db, key1, val1);
    get_val = engine_get(db, key1)->val;
    assert(strcmp(get_val, val1) == 0);

    engine_remove(db, key1);
    assert(engine_get(db, key1) == NULL);
    
    struct engine *db2 = engine_open(&opt);
    assert(engine_get(db2, key1) == NULL);

    get_val = engine_get(db2, key2)->val;
    assert(strcmp(get_val, val2) == 0);

}

