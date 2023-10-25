CC=gcc
CFLAGS=-g -Wall

SOURCE_DIR=source
BINARY_DIR=binaries
TEST_DIR=test

SOURCES=$(wildcard $(SOURCE_DIR)/*.c)
OBJECTS=$(patsubst $(SOURCE_DIR)/%.c, $(BINARY_DIR)/%.o, $(SOURCES))

all: em86 test

em86: $(BINARY_DIR) $(OBJECTS)
	@echo Linking $@
	@$(CC) $(CFLAGS) $(OBJECTS) -o $@

.PHONY: test
test: $(TEST_DIR)/test
	@echo Running tests
	@$(TEST_DIR)/test

$(TEST_DIR)/test: $(BINARY_DIR) $(BINARY_DIR)/em86.o $(TEST_DIR)/test.c
	@echo Compiling tests
	@$(CC) $(CFLAGS) $(TEST_DIR)/test.c test-framework/unity.c $(BINARY_DIR)/em86.o -o $(TEST_DIR)/test

$(BINARY_DIR)/%.o: $(SOURCE_DIR)/%.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) $< -c -o $@

$(BINARY_DIR):
	@echo Creating binaries directory
	@mkdir $@

.PHONY: clean
clean:
	rm -rf $(BINARY_DIR) ./em86 ./test/test
