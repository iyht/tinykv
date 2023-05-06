#include <stdio.h>
#include "../include/engine.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    char dir_path[] = "./db";
    struct option opt = {.dir_path = dir_path, .data_file_size = 1024 * 1024 * 1024};
    struct engine *engine = engine_open(&opt);

    if (strcmp(argv[1], "set") == 0 && argc == 4) {
        engine_put(engine, argv[2], argv[3]);
    } else if (strcmp(argv[1], "get") == 0 && argc == 3) {
        struct log_record* record = engine_get(engine, argv[2]);
        if(!record) return 0;
        printf("%s\n", record->val);
    } else if (strcmp(argv[1], "del") == 0 && argc == 3) {
        engine_remove(engine, argv[2]);
    }else if (strcmp(argv[1], "ts") == 0 && argc == 3) {
        struct log_record* record = engine_get(engine, argv[2]);
        if(!record) {
            printf("No key=%s found in the database!\n", argv[2]);
            return 0;
        }
        printf("%s\n", record->timestamp);
    } else {
        printf("Invalid command\n");
        return 1;
    }


    return 0;
}
