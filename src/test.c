#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
// #include "../include/data_file.h"
#include "../include/kvdb.h"



int main(){
    // struct dirent** entries;
    // int num_entries, i;
    char dir_path[] = "./db";
    // num_entries = scandir(dir_path, &entries, filter, alphasort);
    // if (num_entries < 0) {
    //     printf("Failed to open directory '%s'\n", dir_path);
    //     return -1;
    // }
    // for(int i = 0; i < num_entries; i++){
    //     printf("%s\n", entries[i]->d_name);
    //     int id;
    //     sscanf(entries[i]->d_name, "%d.data", &id);
    //     printf("%d\n", id);

    //     char file_path[512];
    //     sprintf(file_path, "%s/%s", dir_path, entries[i]->d_name);
    //     printf("full %s\n", file_path);
    //     open_data_file(dir_path, id);

    // }

    struct option opt = {.dir_path = dir_path, .data_file_size = 1024 * 1024 * 1024};
    struct db *db = db_open(&opt);
    // put(db, "abc", "123");
    char *val = get(db, "abc");
    printf("%s\n", val);
    put(db, "abc", "j7779799");
    val = get(db, "abc");
    printf("%s\n", val);
}

