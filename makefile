SRC_DIR := ./src
INCLUDE_DIR := ./include
OBJ_DIR := ./obj
BIN_DIR := ./bin

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

CPPSTD := c++17

CC := g++
CFLAGS := -Wall -Wextra -std=$(CPPSTD) # -Werror
LIBS :=
DEBUG_FLAGS := -g -fsanitize=address,undefined
RELEASE_FLAGS := -O2

TARGET := game-of-life

.PHONY: all debug release run format clean

all: $(BIN_DIR)/$(TARGET)

# Build rule
$(BIN_DIR)/$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) $^ -o $@ $(LIBS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

run: all
	$(BIN_DIR)/$(TARGET)

format:
	find ./src ./include -iname '*.hpp' -o -iname '*.cpp' | xargs clang-format --style=file -i --verbose

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf ./build