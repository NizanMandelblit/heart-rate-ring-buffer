ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    CMAKE_GENERATOR := MinGW Makefiles
else
	DETECTED_OS := $(shell uname -s)
	CMAKE_GENERATOR := Unix Makefiles
endif
RM := rm -f
MKDIR := mkdir -p
SEP := /
EXE :=
RUN_PREFIX := ./


# Output OS detection info
define print_os_info
	@echo "Detected OS: $(DETECTED_OS)"
endef

# Compiler settings and flags
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra -Werror -fanalyzer -I./src
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
	@cd $(BUILD_TEST_DIR) && cmake -G "$(CMAKE_GENERATOR)" ${PROJECT_DIR}/${TEST_DIR} && cmake --build .
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

.PHONY: package
PROJECT_NAME := heart_rate_project
package:
	@echo "Packaging project for distribution..."
	zip -r $(PROJECT_NAME).zip $(SRC_DIR) $(TEST_DIR) Makefile README .gitignore .git

# Show help
.PHONY: help
help:
	$(call print_os_info)
	@echo "\nAvailable targets:"
	@echo "  all        - Build the main program (default)"
	@echo "  run ARGS=  - Build and run the main program with arguments"
	@echo "  test       - Build and run tests"
ifeq ($(DETECTED_OS),Linux)
	@echo "  memcheck   - Run the tests with valgrind (Linux only)"
endif
	@echo "  clean      - Remove build files"
	@echo "  package    - Package project for distribution"
	@echo "  help       - Show this help message and exit"
