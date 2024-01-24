#ifndef HEADER_FIO
#define HEADER_FIO
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct io_manager{
    int (*read)(void *self, char *buf, int offset, int64_t size);
    int (*write)(void *self, char *buf, int64_t size);
    int (*sync)(void *self);
    int (*close)(void *self);
};

struct file_io{
    struct io_manager op;
    int fd;
};

int file_io_write(void *self, char *buf, int64_t size);
int file_io_read(void *self, char *buf, int offset, int64_t size);
int file_io_sync(void *self);
int file_io_close(void *self);
struct io_manager *create_file_io(char *filepath);
void destroy_file_io(struct io_manager *io);


#endif



