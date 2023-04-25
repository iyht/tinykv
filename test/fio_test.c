#include "../include/fio.h"
#include <assert.h>


void test_write_read(){
    char *filepath = "test.data";
    struct io_manager *io = create_file_io(filepath);
    assert(io != NULL);
    char *buf = "1234bbbbb";
    int size = io->write(io, buf, strlen(buf));
    assert(size == strlen(buf));

    // test read;
    int buf_size = 10000;
    char *read_buf = malloc(buf_size);
    int read_size = io->read(io, read_buf, 0, buf_size);
    assert(strcmp(buf, read_buf) == 0);
    assert(read_size == strlen(buf));
    printf("read size: %d, buf: %s\n", read_size, buf);

    free(read_buf);
    remove(filepath);
    destroy_file_io(io);

}

int main(){
    test_write_read();
    printf("test fio passed\n");
}