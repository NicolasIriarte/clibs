# Builds and runs one of this library's bundled examples (examples/minimal, examples/production, ...).
#
# Usage (from this directory, or `make -C testing ...` from elsewhere):
#
#   make                       # builds examples/minimal
#   make EXAMPLE=production    # builds a different example
#   make run                   # builds, then runs it
#   make LOGGER_DIR=path/to/logger   # where logger.h is, if not beside this directory
#   make clean
#
# This is a convenience for trying out or smoke-testing what's already in examples/ -- it is not meant
# to replace wiring testsuite.c into your own project's build system. See README.md's "Building" section
# for the plain-cc commands to do that.

CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Werror
EXAMPLE ?= minimal

# testsuite.c logs through logger.h. In this repository that is the sibling directory; a copy of this
# library unpacked on its own has to be told where its logger is.
LOGGER_DIR ?= ../logger

EXAMPLE_DIR := examples/$(EXAMPLE)
BUILD_DIR := $(EXAMPLE_DIR)/build
REGISTRY := $(BUILD_DIR)/testsuite_registry.h
BIN := $(BUILD_DIR)/$(EXAMPLE)

# Same pattern gen_test_registry itself looks for, so this rebuilds when a story is added or edited.
# A story being removed is caught by the registry rule below instead: a file that is gone has no
# timestamp to be newer than anything.
STORY_SOURCES := $(wildcard $(EXAMPLE_DIR)/[0-9]*_*.c)
SOURCES := $(EXAMPLE_DIR)/main.c $(STORY_SOURCES) testsuite.c

.PHONY: all run clean FORCE

all: $(BIN)

# Regenerated on every invocation and only replaced if it came out different, so adding or removing a
# story rebuilds and nothing else does.
$(REGISTRY): FORCE
	@./gen_test_registry $(EXAMPLE_DIR) $@.new
	@if cmp -s $@.new $@; then rm $@.new; else mv $@.new $@; fi

$(BIN): $(REGISTRY) $(SOURCES) testsuite.h
	@test -f $(LOGGER_DIR)/logger.h || { \
	  echo "No logger.h in '$(LOGGER_DIR)'. Pass LOGGER_DIR=path/to/logger." >&2; exit 1; }
	$(CC) $(CFLAGS) -I$(BUILD_DIR) -I. -I$(LOGGER_DIR) $(SOURCES) -o $(BIN)

run: $(BIN)
	./$(BIN)

clean:
	rm -rf examples/*/build
