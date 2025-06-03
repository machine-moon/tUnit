.PHONY: all build clean compile lint tidy check test

# Directories
BUILD_DIR := build

# Source and header files
SRCS := $(shell find src -name '*.cpp')
HDRS := $(shell find include -name '*.h')
ALL_FILES := $(SRCS) $(HDRS) main.cpp

# Default target
all: build compile test

# Build targets
build: clean
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_CXX_COMPILER=g++

compile: build
	@cmake --build $(BUILD_DIR) --config Debug

clean:
	@rm -rf $(BUILD_DIR)
	@clear

# Code quality targets
lint:
	@clang-format -i $(ALL_FILES)

tidy:
	@clang-tidy $(SRCS) -- -Iinclude

check: lint tidy

# Test target
test: build
	@cd $(BUILD_DIR) && ctest --output-on-failure
