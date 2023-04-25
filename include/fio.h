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


int file_io_write(void *self, char *buf, int64_t size){
    struct file_io *fio = (struct file_io *)self;
    // printf("file_io_write: size=%d\n",strlen(buf));
    return write(fio->fd, buf, size);
}

int file_io_read(void *self, char *buf, int offset, int64_t size){
    struct file_io *fio = (struct file_io *)self;
    lseek(fio->fd, offset, SEEK_SET);
    return read(fio->fd, buf, size);
}

int file_io_sync(void *self){
    struct file_io *fio = (struct file_io*)self;
    return fsync(fio->fd);
}

int file_io_close(void *self){
    struct file_io *fio = (struct file_io*)self;
    return close(fio->fd);
}
struct io_manager *create_file_io(char *filepath){
    struct file_io *fio = (struct file_io*)malloc(sizeof(struct file_io));
    fio->op.read = file_io_read;
    fio->op.write = file_io_write;
    fio->op.sync = file_io_sync;
    fio->op.close = file_io_close;
    fio->fd = open(filepath, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (fio->fd < 0){
        perror("open");
        return NULL;
    }
    return (struct io_manager*)fio;
}

void destroy_file_io(struct io_manager *io){
    struct file_io *fio = (struct file_io *)io;
    fio->op.close(fio);
    free(fio);
}

#endif



// int main(){
//     // test write
//     char *filepath = "test.data";
//     struct io_manager *io = create_file_io(filepath);
//     int size = io->write(io, "1234bbbbb");
//     printf("size: %d written\n", size);

//     // test read;
//     int buf_size = 10000;
//     char *buf = malloc(buf_size + 1);
//     buf[buf_size] = '\0';
//     int read_size = io->read(io, buf, 0, buf_size);
//     printf("read size: %d, buf: %s\n", read_size, buf);

//     free(buf);
//     remove(filepath);
//     destroy_file_io(io);


// }