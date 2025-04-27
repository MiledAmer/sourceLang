# Compiler & flags
CC = gcc
# delete -std=c99 it was after -Wextra
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
DEPS_DIR = dependencies/rax
BUILD_DIR = build

# Files
TARGET = server
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(BUILD_DIR)/rax.o

# Default target
all: $(TARGET)

# Create build dir if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile RAX dependency
$(BUILD_DIR)/rax.o: $(DEPS_DIR)/rax.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile each source file to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link everything into final binary
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS) -lm

# Clean
clean:
	rm -rf $(BUILD_DIR) *.o $(TARGET)
