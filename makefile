# Makefile for building a CMake project with Ninja

# Variables
BUILD_TYPE ?= Debug
BUILD_DIR := cmake-build-$(shell echo $(BUILD_TYPE) | tr '[:upper:]' '[:lower:]')
GENERATOR := Ninja
JOBS := 6

# Compiler flags
ifeq ($(BUILD_TYPE),Release)
  CFLAGS   := -Wall -Wextra -Wpedantic -O0 -g
else
  CFLAGS   := -Wall -Wextra -Wpedantic -O3
endif

# Default target
.PHONY: all
all: configure build

# Configure step
.PHONY: configure
configure:
	cmake -S . -B $(BUILD_DIR) \
		-G "$(GENERATOR)" \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_C_FLAGS="$(CFLAGS)"

# Build step
.PHONY: build
build:
	cmake --build $(BUILD_DIR) -- -j$(JOBS)

# Run tests (passes any CLI args to the program)
.PHONY: test
test: build
	@echo "Running program with args: $(ARGS)"
	@$(BUILD_DIR)/$$(basename $$(pwd)) $(ARGS)

# Clean build directory
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Reconfigure and rebuild from scratch
.PHONY: rebuild
rebuild: clean all
