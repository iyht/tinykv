CC = gcc
FLAG = -g -fprofile-arcs -ftest-coverage -O0

# Define directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
DB_DIR = db
$(shell mkdir -p $(DB_DIR))

all: engine

log_record.o: $(SRC_DIR)/log_record.c
	$(CC) $(FLAG) -c -o $@ $<

skiplist.o: $(SRC_DIR)/skiplist.c
	$(CC) $(FLAG) -c -o $@ $<

fio.o: $(SRC_DIR)/fio.c
	$(CC) $(FLAG) -c -o $@ $<

index.o: $(SRC_DIR)/index.c
	$(CC) $(FLAG) -c -o $@ $<

data_file.o: $(SRC_DIR)/data_file.c
	$(CC) $(FLAG) -c -o $@ $<

engine.o: $(SRC_DIR)/engine.c
	$(CC) $(FLAG) -c -o $@ $<

# test for skiplist
skiplist_test: $(TEST_DIR)/skiplist_test.c skiplist.o
	$(CC) $(FLAG) -o $@ $^


# test for index
index_test: $(TEST_DIR)/index_test.c index.o skiplist.o
	$(CC) $(FLAG) -o $@ $^

# test fio
fio_test: $(TEST_DIR)/fio_test.c fio.o
	$(CC) $(FLAG) -o $@ $^

# test log_record
log_record_test: $(TEST_DIR)/log_record_test.c log_record.o fio.o data_file.o
	$(CC) $(FLAG) -o $@ $^

# test engine
engine_test: $(TEST_DIR)/engine_test.c log_record.o skiplist.o fio.o index.o data_file.o engine.o
	$(CC) $(FLAG) -o $@ $^

all_test: skiplist_test index_test fio_test log_record_test engine_test
	./skiplist_test
	./index_test
	./fio_test
	./log_record_test
	./engine_test
	gcov -pbr --object-directory ./ ./src/*


engine: $(SRC_DIR)/client.c log_record.o skiplist.o fio.o index.o data_file.o engine.o
	$(CC) $(FLAG) -o $@ $^

clean:
	rm -f engine
	rm -f a.out
	rm -f *.gcno
	rm -f *.gcda
	rm -f *.o
	rm -f *.gcov
	rm -f *_test

.PHONY: clean all_test engine all