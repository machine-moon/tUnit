.PHONY: all build clean test bin lint tidy check

BUILD_DIR := build

SRCS := $(shell find src -name '*.cpp')
HDRS := $(shell find include -name '*.h')
ALL_FILES := $(SRCS) $(HDRS) main.cpp

all: build

build: clean
	@mkdir $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) -G Ninja -DCMAKE_C_COMPILER=cc
	@cmake --build $(BUILD_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@clear
test:
	@ctest --test-dir $(BUILD_DIR)

bin:
	@./$(BUILD_DIR)/TestRunner
lint:
	@clang-format -i $(ALL_FILES)

tidy:
	@clang-tidy $(SRCS) -- -Iinclude

check: lint tidy
