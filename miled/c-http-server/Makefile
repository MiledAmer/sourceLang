# Compiler & flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
DEPS_DIR = dependencies
BUILD_DIR = build

# Include path for cJSON
CFLAGS += -I$(DEPS_DIR)/cJSON

# Files
TARGET = server
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(BUILD_DIR)/rax.o $(BUILD_DIR)/cJSON.o

# Default target
all: $(TARGET)

# Create build dir if not exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile RAX dependency
$(BUILD_DIR)/rax.o: $(DEPS_DIR)/rax/rax.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile cJSON dependency
$(BUILD_DIR)/cJSON.o: $(DEPS_DIR)/cJSON/cJSON.c | $(BUILD_DIR)
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
