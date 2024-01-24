CC = gcc
CFLAGS = -g -fprofile-arcs -ftest-coverage -O0
CFLAGS_NO_COV = -g -O0
LDFLAGS = -lgcov

# Define directories
S = src
T = test
DB = db
OBJS = $(S)/log_record.o $(S)/skiplist.o $(S)/fio.o $(S)/index.o $(S)/data_file.o $(S)/engine.o
TESTS = $(patsubst $(T)/%.c,$(T)/%,$(wildcard $(T)/*.c))

# Create database directory
$(shell mkdir -p $(DB))

# Default target
all: engine

# Generic rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Test targets
$(T)/skiplist_test: $(T)/skiplist_test.c $(OBJS)
$(T)/index_test: $(T)/index_test.c $(OBJS)
$(T)/fio_test: $(T)/fio_test.c $(OBJS)
$(T)/log_record_test: $(T)/log_record_test.c $(OBJS)
$(T)/engine_test: $(T)/engine_test.c $(OBJS)

# Pattern rule for test executables
$(T)/%:
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Run all tests
all_test: $(TESTS)
	for test in $^; do ./$$test; done
	gcov -pbr --object-directory $(S) $(S)/*

# Engine target
engine: $(S)/client.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean target
clean:
	rm -f $(S)/*.o $(S)/*.gcno $(S)/*.gcda $(S)/*.gcov
	rm -f $(T)/*.o $(T)/*.gcno $(T)/*.gcda $(T)/*.gcov $(TESTS)
	rm -f *.gcov

.PHONY: clean all_test engine all