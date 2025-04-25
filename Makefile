CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -O2
SRC_DIR := src
EX_DIR  := examples

LIB_OBJ := $(SRC_DIR)/base_n_trie.o
DEMO_OBJ:= $(EX_DIR)/demo_decimal.o

.PHONY: all demo clean

all: demo

demo: $(LIB_OBJ) $(DEMO_OBJ)
	$(CC) $(CFLAGS) -o demo $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EX_DIR)/%.o: $(EX_DIR)/%.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	$(RM) $(SRC_DIR)/*.o $(EX_DIR)/*.o demo
