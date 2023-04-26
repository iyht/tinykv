#ifndef HEADER_KVDB
#define HEADER_KVDB


#include <stdbool.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include "../include/index.h"
#include "../include/data_file.h"
#include "../include/log_record.h"
#include "../include/option.h"



struct db* db_open(struct option *opt);
bool put(struct db *self, char *key, char *val);
char* get(struct db *self, char *key);
bool db_remove(struct db* self, char *key);


#endif