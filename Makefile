CC = gcc

# Define directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
BUILD_DIR = build

$(shell mkdir -p $(BUILD_DIR))

# test for skiplist
$(BUILD_DIR)/skiplist_test: $(TEST_DIR)/skiplist_test.c $(BUILD_DIR)/skiplist.o
	$(CC) -o $@ $^

$(BUILD_DIR)/skiplist.o: $(SRC_DIR)/skiplist.c
	$(CC) -c -o $@ $<


# test for index
$(BUILD_DIR)/index_test: $(TEST_DIR)/index_test.c $(BUILD_DIR)/skiplist.o
	$(CC) -o $@ $^

# test fio
$(BUILD_DIR)/fio_test: $(TEST_DIR)/fio_test.c 
	$(CC) -o $@ $^

all_test: $(BUILD_DIR)/skiplist_test $(BUILD_DIR)/index_test $(BUILD_DIR)/fio_test
	./$(BUILD_DIR)/skiplist_test
	./$(BUILD_DIR)/index_test
	./$(BUILD_DIR)/fio_test




clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: clean all_test