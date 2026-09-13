# Builds and runs one of this library's bundled examples (examples/minimal, examples/production, ...).
#
# Usage (from this directory, or `make -C testing ...` from elsewhere):
#
#   make                       # builds examples/minimal
#   make EXAMPLE=production    # builds a different example
#   make run                   # builds, then runs it
#   make clean
#
# This is a convenience for trying out or smoke-testing what's already in examples/ -- it is not meant
# to replace wiring testsuite.c into your own project's build system. See README.md's "Building" section
# for the plain-cc commands to do that.

CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
EXAMPLE ?= minimal

EXAMPLE_DIR := examples/$(EXAMPLE)
BUILD_DIR := $(EXAMPLE_DIR)/build
REGISTRY := $(BUILD_DIR)/testsuite_registry.h
BIN := $(BUILD_DIR)/$(EXAMPLE)

# Same pattern gen_test_registry itself looks for, so this rebuilds exactly when a story is added,
# removed, or edited.
STORY_SOURCES := $(wildcard $(EXAMPLE_DIR)/[0-9]*_*.c)
SOURCES := $(EXAMPLE_DIR)/main.c $(STORY_SOURCES) testsuite.c

.PHONY: all run clean

all: $(BIN)

$(REGISTRY): $(STORY_SOURCES)
	./gen_test_registry $(EXAMPLE_DIR) $(REGISTRY)

$(BIN): $(REGISTRY) $(SOURCES) testsuite.h
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -I. -I../logger $(SOURCES) -o $(BIN)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf examples/*/build
