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



struct engine* engine_open(struct option *opt);
bool engine_put(struct engine *self, char *key, char *val);
struct log_record* engine_get(struct engine *self, char *key);
bool engine_remove(struct engine* self, char *key);


#endif