#include "base_n_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void *xmalloc(size_t n) {
    void *p = malloc(n);
    if (!p) {
        fprintf(stderr, "base-n-trie: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

/* static helper‑prototypes, e.g.: */

static TrieNode *create_node(void);
static void destroy_node(TrieNode *node);
static int get_digit_index(char digit);
static char index_to_char(int idx);
static int trie_delete_recursive(TrieNode *node, const char *key, size_t len,
                                 size_t pos);
static int has_children(TrieNode *node);

/* Helpers to print everything in the trie */
static void _print_all(TrieNode *node, char *buf, int depth);

/* public definitions (signatures come from the header) */
BaseNTrie *trie_create(void) {
    BaseNTrie *t = xmalloc(sizeof *t);
    t->root = create_node();
    return t;
}

TrieNode *create_node(void) {
    TrieNode *n = xmalloc(sizeof *n);
    n->is_terminal = 0;
    for (int i = 0; i < DIGIT_COUNT; i++)
        n->children[i] = NULL;
    return n;
}

void destroy_dectrie(BaseNTrie *trie) {
    destroy_node(trie->root);
    free(trie);
}

void destroy_node(TrieNode *node) {
    if (!node)
        return;
    for (int i = 0; i < DIGIT_COUNT; i++)
        destroy_node(node->children[i]);
    free(node);
}

int get_digit_index(char digit) {
    if (digit < '0' || digit > '9') {
        fprintf(stderr, "Invalid key character: %c\n", digit);
        exit(EXIT_FAILURE);
    }
    return digit - '0';
}

char index_to_char(int idx) {
    return (char)('0' + idx);
}

void insert(BaseNTrie *trie, const char *key) {
    TrieNode *p = trie->root;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        int idx = get_digit_index(key[i]);
        if (!p->children[idx])
            p->children[idx] = create_node();
        p = p->children[idx];
    }
    p->is_terminal = 1;
}

int trie_search(BaseNTrie *trie, const char *key) {
    TrieNode *p = trie->root;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        int idx = get_digit_index(key[i]);
        if (!p->children[idx])
            return 0;
        p = p->children[idx];
    }
    return p->is_terminal;
}

void trie_delete(BaseNTrie *trie, const char *key) {
    trie_delete_recursive(trie->root, key, strlen(key), 0);
}

int trie_delete_recursive(TrieNode *node, const char *key, size_t len,
                          size_t pos) {
    if (!node)
        return 0;
    if (pos == len) {
        if (!node->is_terminal)
            return 0;
        if (has_children(node)) {
            node->is_terminal = 0;
            return 0;
        }
        free(node);
        return 1;
    }
    int idx = get_digit_index(key[pos]);
    int child_deleted =
        trie_delete_recursive(node->children[idx], key, len, pos + 1);
    if (child_deleted) {
        node->children[idx] = NULL;
        if (pos > 0 && !node->is_terminal && !has_children(node)) {
            free(node);
            return 1;
        }
    }
    return 0;
}

int has_children(TrieNode *node) {
    if (!node)
        return 0;
    for (int i = 0; i < DIGIT_COUNT; i++)
        if (node->children[i])
            return 1;
    return 0;
}

void _print_all(TrieNode *node, char *buf, int depth) {
    if (node->is_terminal) {
        buf[depth] = '\0';
        puts(buf);
    }
    for (int d = 0; d < DIGIT_COUNT; d++) {
        if (node->children[d]) {
            buf[depth] = index_to_char(d);
            _print_all(node->children[d], buf, depth + 1);
        }
    }
}

void print_trie(BaseNTrie *trie) {
    char buf[MAX_KEY_LEN];
    _print_all(trie->root, buf, 0);
}
