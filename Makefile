# Detect operating system
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    RM := del /Q
    MKDIR := mkdir
    SEP := \\
    EXE := .exe
    RUN_PREFIX :=
else
    DETECTED_OS := $(shell uname -s)
    RM := rm -f
    MKDIR := mkdir -p
    SEP := /
    EXE :=
    RUN_PREFIX := ./
endif

# Output OS detection info
define print_os_info
	@echo "Detected OS: $(DETECTED_OS)"
	@echo "Using path separator: '$(SEP)'"
	@echo "Executable extension: '$(EXE)'"
endef

# Compiler settings and flags
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -I./src
CXXFLAGS := $(CFLAGS)
LDFLAGS :=

# Directories
PROJECT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
BUILD_SRC_DIR := $(BUILD_DIR)$(SEP)src
BUILD_TEST_DIR := $(BUILD_DIR)$(SEP)test

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_SRC_DIR)/%.o)

# Main executable
TARGET := $(BUILD_SRC_DIR)/heart_rate$(EXE)

# Default target
.PHONY: all
all: 
	$(call print_os_info)
	@echo "Building main program..."
	@$(MAKE) --no-print-directory $(TARGET)

# Create build directories
$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(BUILD_SRC_DIR): $(BUILD_DIR)
	$(MKDIR) $(BUILD_SRC_DIR)

$(BUILD_TEST_DIR): $(BUILD_DIR)
	$(MKDIR) $(BUILD_TEST_DIR)

# Compile C source files
$(BUILD_SRC_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_SRC_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link main program
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Run the program
.PHONY: run
run: 
	@echo "Building and running program..."
	@$(MAKE) --no-print-directory $(TARGET)
	$(RUN_PREFIX)$(TARGET) $(ARGS)

# Build and run tests using CMake
.PHONY: test
test:	$(BUILD_TEST_DIR)
	@echo "Building and running tests..."
	@cd $(BUILD_TEST_DIR) && cmake ${PROJECT_DIR}/${TEST_DIR} && cmake --build .
	@cd $(BUILD_TEST_DIR) && ctest --output-on-failure --verbose

# Run tests with valgrind
.PHONY: memcheck
memcheck: $(BUILD_TEST_DIR) test
	@echo "Running tests with valgrind..."
	@cd $(BUILD_TEST_DIR) && ctest -T memcheck --output-on-failure --overwrite MemoryCheckCommandOptions="--leak-check=full --error-exitcode=1" --verbose

# Clean build files
.PHONY: clean
clean:
	@echo "Cleaning build files..."
ifeq ($(DETECTED_OS),Windows)
	if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif

# Show help
.PHONY: help
help:
	$(call print_os_info)
	@echo "\nAvailable targets:"
	@echo "  all        - Build the main program (default)"
	@echo "  run ARGS=   - Build and run the main program with arguments"
	@echo "  test       - Build and run tests"
	@echo "  memcheck   - Run the tests with valgrind"
	@echo "  clean      - Remove build files"
	@echo "  help       - Show this help message and exit"
