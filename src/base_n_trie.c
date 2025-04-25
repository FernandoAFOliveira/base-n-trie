#include "base_n_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* static helper‑prototypes, e.g.: */

static DecTrieNode *create_node(void);
static void destroy_node(DecTrieNode *node);
static int get_digit_index(char digit);
static char index_to_char(int idx);
static int remove_string_recursive(DecTrieNode *node, const char *key,
                                   size_t len, size_t pos);
static int has_children(DecTrieNode *node);

/* Helpers to print everything in the trie */
static void _print_all(DecTrieNode *node, char *buf, int depth);

int main(void) {
    DecTrie *trie = create_dectrie();

    /* insert a bunch of numeric‐string keys */
    insert(trie, "123");
    insert(trie, "0039");
    insert(trie, "39");
    insert(trie, "1234");
    insert(trie, "12345");
    insert(trie, "123456");
    insert(trie, "1234567");

    puts("=== All keys before removal ===");
    print_trie(trie);

    /* test individual search */
    printf("search(\"123\")      = %d\n", search(trie, "123"));
    printf("search(\"0039\")     = %d\n", search(trie, "0039"));
    printf("search(\"123456\")   = %d\n", search(trie, "123456"));

    /* remove one */
    remove_string(trie, "123456");
    printf("\nremoved key \"123456\"\n\n");

    puts("=== All keys after removal ===");
    print_trie(trie);

    /* verify the removed key is gone */
    printf("\nsearch(\"123456\")   = %d\n", search(trie, "123456"));

    destroy_dectrie(trie);
    return 0;
}

/* public definitions (signatures come from the header) */
DecTrie *create_dectrie(void) {
    DecTrie *t = malloc(sizeof *t);
    t->root = create_node();
    return t;
}

DecTrieNode *create_node(void) {
    DecTrieNode *n = malloc(sizeof *n);
    n->is_terminal = 0;
    for (int i = 0; i < DIGIT_COUNT; i++)
        n->children[i] = NULL;
    return n;
}

void destroy_dectrie(DecTrie *trie) {
    destroy_node(trie->root);
    free(trie);
}

void destroy_node(DecTrieNode *node) {
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

void insert(DecTrie *trie, const char *key) {
    DecTrieNode *p = trie->root;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        int idx = get_digit_index(key[i]);
        if (!p->children[idx])
            p->children[idx] = create_node();
        p = p->children[idx];
    }
    p->is_terminal = 1;
}

int search(DecTrie *trie, const char *key) {
    DecTrieNode *p = trie->root;
    size_t len = strlen(key);
    for (size_t i = 0; i < len; i++) {
        int idx = get_digit_index(key[i]);
        if (!p->children[idx])
            return 0;
        p = p->children[idx];
    }
    return p->is_terminal;
}

void remove_string(DecTrie *trie, const char *key) {
    remove_string_recursive(trie->root, key, strlen(key), 0);
}

int remove_string_recursive(DecTrieNode *node, const char *key, size_t len,
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
        remove_string_recursive(node->children[idx], key, len, pos + 1);
    if (child_deleted) {
        node->children[idx] = NULL;
        if (pos > 0 && !node->is_terminal && !has_children(node)) {
            free(node);
            return 1;
        }
    }
    return 0;
}

int has_children(DecTrieNode *node) {
    if (!node)
        return 0;
    for (int i = 0; i < DIGIT_COUNT; i++)
        if (node->children[i])
            return 1;
    return 0;
}

void _print_all(DecTrieNode *node, char *buf, int depth) {
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

void print_trie(DecTrie *trie) {
    char buf[MAX_KEY_LEN];
    _print_all(trie->root, buf, 0);
}
