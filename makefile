CC=gcc
CFLAGS=-g -Wall

SOURCE_DIR=source
BINARY_DIR=binaries
TEST_DIR=test

SOURCES=$(wildcard $(SOURCE_DIR)/*.c)
OBJECTS=$(patsubst $(SOURCE_DIR)/%.c, $(BINARY_DIR)/%.o, $(SOURCES))

all: test

.PHONY: test
test: $(TEST_DIR)/test
	@echo Running tests
	@$(TEST_DIR)/test

$(TEST_DIR)/test: $(BINARY_DIR) $(OBJECTS) $(TEST_DIR)/test.c
	@echo Compiling tests
	@$(CC) $(CFLAGS) $(TEST_DIR)/test.c test-framework/unity.c $(OBJECTS) -o $(TEST_DIR)/test

$(OBJECTS): $(SOURCES)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(SOURCES) -c -o $(OBJECTS)

$(BINARY_DIR):
	@mkdir $@

.PHONY: clean
clean:
	rm -rf $(BINARY_DIR) test/test
