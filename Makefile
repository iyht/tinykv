CC = gcc
FLAG = -g

# Define directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
BUILD_DIR = build
DB_DIR = db
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(DB_DIR))

all: engine

$(BUILD_DIR)/log_record.o: $(SRC_DIR)/log_record.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/skiplist.o: $(SRC_DIR)/skiplist.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/fio.o: $(SRC_DIR)/fio.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/index.o: $(SRC_DIR)/index.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/data_file.o: $(SRC_DIR)/data_file.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/engine.o: $(SRC_DIR)/engine.c
	$(CC) $(FLAG) -c -o $@ $<

# test for skiplist
$(BUILD_DIR)/skiplist_test: $(TEST_DIR)/skiplist_test.c $(BUILD_DIR)/skiplist.o
	$(CC) $(FLAG) -o $@ $^


# test for index
$(BUILD_DIR)/index_test: $(TEST_DIR)/index_test.c $(BUILD_DIR)/index.o $(BUILD_DIR)/skiplist.o
	$(CC) $(FLAG) -o $@ $^

# test fio
$(BUILD_DIR)/fio_test: $(TEST_DIR)/fio_test.c $(BUILD_DIR)/fio.o
	$(CC) -o $@ $^

# test log_record
$(BUILD_DIR)/log_record_test: $(TEST_DIR)/log_record_test.c $(BUILD_DIR)/log_record.o $(BUILD_DIR)/fio.o $(BUILD_DIR)/data_file.o
	$(CC) -o $@ $^

# test engine
$(BUILD_DIR)/engine_test: $(TEST_DIR)/engine_test.c $(BUILD_DIR)/log_record.o $(BUILD_DIR)/skiplist.o $(BUILD_DIR)/fio.o $(BUILD_DIR)/index.o $(BUILD_DIR)/data_file.o $(BUILD_DIR)/engine.o
	$(CC) $(FLAG) -o $@ $^

all_test: $(BUILD_DIR)/skiplist_test $(BUILD_DIR)/index_test $(BUILD_DIR)/fio_test $(BUILD_DIR)/log_record_test $(BUILD_DIR)/engine_test
	./$(BUILD_DIR)/skiplist_test
	./$(BUILD_DIR)/index_test
	./$(BUILD_DIR)/fio_test
	./$(BUILD_DIR)/log_record_test
	./$(BUILD_DIR)/engine_test


engine: $(SRC_DIR)/client.c $(BUILD_DIR)/log_record.o $(BUILD_DIR)/skiplist.o $(BUILD_DIR)/fio.o $(BUILD_DIR)/index.o $(BUILD_DIR)/data_file.o $(BUILD_DIR)/engine.o
	$(CC) $(FLAG) -o $@ $^



clean:
	rm -rf $(BUILD_DIR)/*
	rm engine
	rm a.out

.PHONY: clean all_test engine all