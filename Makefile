CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2

SRC     := src/base_n_trie.c

# grab all demos under examples/
DEMO_SRCS := $(wildcard examples/demo_*.c)
DEMO_BINS := $(notdir $(basename $(DEMO_SRCS)))

# grab all tests under tests/
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(notdir $(basename $(TEST_SRCS)))

.PHONY: all demos tests check clean

# default
all: demos tests

#–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
# Build demos
demos: $(DEMO_BINS)

# pattern rule: demo_decimal ← examples/demo_decimal.c + library
$(DEMO_BINS): %: examples/%.c $(SRC)
	$(CC) $(CFLAGS) -Isrc $(SRC) $< -o $@

#–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
# Build tests
tests: $(TEST_BINS)

# pattern rule: basic ← tests/basic.c + library
$(TEST_BINS): %: tests/%.c $(SRC)
	$(CC) $(CFLAGS) -Isrc $(SRC) $< -o $@

#–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
# Build & run everything
check: all
	@echo "» Running demos…"
	@for b in $(DEMO_BINS); do \
	  echo "---- $$b ----"; \
	  ./$$b; \
	done
	@echo "» Running tests…"
	@for b in $(TEST_BINS); do \
	  echo "---- $$b ----"; \
	  ./$$b; \
	done
	@echo "✅  All demos & tests passed"

#–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
clean:
	-$(RM) $(DEMO_BINS) $(TEST_BINS)
