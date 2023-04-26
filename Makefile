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

$(BUILD_DIR)/log_record.o: $(SRC_DIR)/log_record.c
	$(CC) -c -o $@ $<

$(BUILD_DIR)/skiplist.o: $(SRC_DIR)/skiplist.c
	$(CC) $(FLAG) -c -o $@ $<

$(BUILD_DIR)/fio.o: $(SRC_DIR)/fio.c
	$(CC) -c -o $@ $<

$(BUILD_DIR)/index.o: $(SRC_DIR)/index.c
	$(CC) -c -o $@ $<

$(BUILD_DIR)/data_file.o: $(SRC_DIR)/data_file.c
	$(CC) -c -o $@ $<

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

all_test: $(BUILD_DIR)/skiplist_test $(BUILD_DIR)/index_test $(BUILD_DIR)/fio_test $(BUILD_DIR)/log_record_test
	./$(BUILD_DIR)/skiplist_test
	./$(BUILD_DIR)/index_test
	./$(BUILD_DIR)/fio_test
	./$(BUILD_DIR)/log_record_test




clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: clean all_test