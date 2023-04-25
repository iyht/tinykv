#include "../include/fio.h"

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