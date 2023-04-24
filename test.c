#include "data.h"
#include "fio.h"
#include "string.h"
int main(){
    char *key = "how are you doing my bro";
    char *val = "I'm pretty good thanks for asking";
    struct log_record record = {.key_size = strlen(key), 
                                .val_size = strlen(val), 
                                .type = LOG_RECORD_NORMAL,
                                .key = key,
                                .val = val};

    char* buf;
    int64_t size;
    encode_log_record(&record, &buf, &size);

    struct io_manager *io = create_file_io("test.txt");
    io->write(io, buf, size);
    io->sync(io);

    char 
    io->close(io);
    // remove("test.txt");



}