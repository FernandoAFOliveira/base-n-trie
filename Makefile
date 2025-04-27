SHELL := /bin/bash  # Explicitly force bash (for consistent wildcard behavior)
CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2
SRC     := src/base_n_trie.c

# Use explicit wildcards for CI compatibility
DEMO_SRCS := $(shell find examples -maxdepth 1 -name '*.c')
DEMO_BINS := demo_decimal demo_hex demo_octal


TEST_SRCS := $(shell find tests -maxdepth 1 -name '*.c')
TEST_BINS := $(notdir $(basename $(TEST_SRCS)))

.PHONY: all demos tests check clean

all: demos tests

# Build demos using wildcard
demos: $(DEMO_BINS)
$(DEMO_BINS): %: examples/%.c $(SRC)
	$(CC) $(CFLAGS) -Isrc $(SRC) $< -o $@
	chmod 755 $@

# Build tests using wildcard
tests: $(TEST_BINS)
$(TEST_BINS): %: tests/%.c $(SRC)
	$(CC) $(CFLAGS) -Isrc $(SRC) $< -o $@
	chmod 755 $@

check: all
	@echo "» Running demos…"
	@for b in $(DEMO_BINS); do \
	  echo "---- $$b ----"; \
	  ./$$b || exit 1; \
	done
	@echo "» Running tests…"
	@for b in $(TEST_BINS); do \
	  echo "---- $$b ----"; \
	  ./$$b || exit 1; \
	done
	@echo "✅ All demos & tests passed"

clean:
	-$(RM) $(DEMO_BINS) $(TEST_BINS)