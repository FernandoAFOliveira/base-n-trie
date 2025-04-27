/**
 * @file base_n_trie.c
 * @brief A generic N-ary trie implementation.
 *
 * This file defines the functions declared in base_n_trie.h:
 *   - trie_create / trie_destroy
 *   - trie_insert / trie_search / trie_delete
 *   - print_trie
 *
 * Internally, each node holds a payload pointer and a per-trie
 * destructor (if supplied), so that when you destroy the trie or
 * delete a key, your payloads get freed correctly.
 *
 *  @author Fernando Oliveira
 */

#include "base_n_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// —––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// Helpers for the “convenience” alphabets.  These must have external linkage.
uint8_t dec_to_index(char symbol) {
    return (uint8_t)(symbol - '0');
}
char dec_to_char(uint8_t index) {
    return (char)('0' + index);
}

uint8_t oct_to_index(char symbol) {
    return (uint8_t)(symbol - '0');
}
char oct_to_char(uint8_t index) {
    return (char)('0' + index);
}

uint8_t hex_to_index(char c) {
    if (c >= '0' && c <= '9')
        return (uint8_t)(c - '0');
    return (uint8_t)(10 + (uint8_t)(toupper((unsigned char)c) - 'A'));
}
char hex_to_char(uint8_t index) {
    return index < 10 ? (char)('0' + index) : (char)('A' + (index - 10));
}
// —––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

//
// -- Safe allocation --
//
static void *xmalloc(size_t bytes) {
    void *p = malloc(bytes);
    if (!p) {
        fprintf(stderr, "base-n-trie: out of memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

//
// -- Node definition (visible in .c only) --
//
typedef struct TrieNode_s {
    struct TrieNode_s **children; // array of length trie->base
    struct TrieNode_s *parent;    // NULL for root
    void *value;                  // NULL = no data; non-NULL = stored data
} TrieNode;

//
// -- Trie handle definition --
//
struct BaseNTrie_s {
    uint8_t base;
    TrieNode *root;
    to_index_fn to_index;
    to_char_fn to_char;
    destructor_fn dtor;
};

//
// -- Forward helpers --
//
static TrieNode *create_node(BaseNTrie *trie, TrieNode *parent);
static void destroy_subtrie(BaseNTrie *trie, TrieNode *node);
static void cleanup_empty_branches(BaseNTrie *trie, TrieNode *start);
static bool node_has_children(BaseNTrie *trie, TrieNode *node);
static void _print_all(const BaseNTrie *trie, TrieNode *node, char *buf,
                       int depth);

//
// -- Public API --
//
/**
 * @brief Allocate a new trie.
 * @param base      The size of the alphabet (e.g. 10 for decimal).
 * @param to_index  Function that maps a symbol → [0..base-1].
 * @param to_char   Function that maps an index → symbol.
 * @param dtor      Optional destructor to free your payloads (or NULL).
 * @return          A fresh BaseNTrie*, or exits on allocation failure.
 */
BaseNTrie *trie_create(uint8_t base, to_index_fn to_index, to_char_fn to_char,
                       destructor_fn dtor) {
    BaseNTrie *trie = xmalloc(sizeof *trie);
    trie->base = base;
    trie->to_index = to_index;
    trie->to_char = to_char;
    trie->dtor = dtor;
    trie->root = create_node(trie, NULL);
    return trie;
}
/**
 * @brief Free all nodes in the trie (and payloads via the dtor) and the handle
 * itself.
 */
void trie_destroy(BaseNTrie *trie) {
    if (trie->root) {
        destroy_subtrie(trie, trie->root);
    }
    free(trie);
}

int trie_insert(BaseNTrie *trie, const char *key, void *value) {
    TrieNode *current_node = trie->root;
    size_t key_length = strlen(key);

    for (size_t i = 0; i < key_length; ++i) {
        char symbol = key[i];
        uint8_t symbol_index = trie->to_index(symbol);

        // allocate child array if absent
        if (current_node->children[symbol_index] == NULL) {
            current_node->children[symbol_index] =
                create_node(trie, current_node);
        }
        current_node = current_node->children[symbol_index];
    }

    // attach the payload
    current_node->value = value;
    return 0;
}

void *trie_search(const BaseNTrie *trie, const char *key) {
    TrieNode *current_node = trie->root;
    size_t key_length = strlen(key);

    for (size_t i = 0; i < key_length; ++i) {
        char symbol = key[i];
        uint8_t symbol_index = trie->to_index(symbol);

        TrieNode *next = current_node->children[symbol_index];
        if (!next) {
            return NULL;
        }
        current_node = next;
    }
    return current_node->value;
}

void *trie_delete(BaseNTrie *trie, const char *key) {
    // first find the terminal node
    TrieNode *current_node = trie->root;
    size_t key_length = strlen(key);

    for (size_t i = 0; i < key_length; ++i) {
        char symbol = key[i];
        uint8_t symbol_index = trie->to_index(symbol);
        current_node = current_node->children[symbol_index];
        if (!current_node) {
            return NULL;
        }
    }

    // detach and return old value
    void *old_value = current_node->value;
    if (!old_value) {
        return NULL;
    }
    current_node->value = NULL;

    // prune any empty branches upward
    cleanup_empty_branches(trie, current_node);
    return old_value;
}

/**
 * @brief Print all stored keys to stdout.
 */
void print_trie(BaseNTrie *trie) {
    char buf[MAX_KEY_LENGTH];
    buf[0] = '\0';
    _print_all(trie, trie->root, buf, 0);
}

//
// -- Internal helpers --
//

static TrieNode *create_node(BaseNTrie *trie, TrieNode *parent) {
    TrieNode *node = xmalloc(sizeof *node);
    node->parent = parent;
    node->value = NULL;
    // children array:
    node->children = xmalloc((size_t)trie->base * sizeof *node->children);
    for (uint8_t i = 0; i < trie->base; ++i) {
        node->children[i] = NULL;
    }
    printf("creating node, base=%u, at %p\n", trie->base, node->children);
    return node;
}
/* Recursively free a subtree rooted at node. */
static void destroy_subtrie(BaseNTrie *trie, TrieNode *node) {
    for (uint8_t i = 0; i < trie->base; ++i) {
        if (node->children[i]) {
            destroy_subtrie(trie, node->children[i]);
        }
    }
    // TEMP DEBUG: force content change so CI picks up the new version
    if (0)
        fprintf(stderr, "DEBUG destroy_subtrie called\n");
    // Only call the user-supplied destructor if both a payload and a dtor
    // exist:
    if (node->value && trie->dtor) {
        trie->dtor(node->value);
    }
    free(node->children);
    free(node);
}

static bool node_has_children(BaseNTrie *trie, TrieNode *node) {
    for (uint8_t i = 0; i < trie->base; ++i)
        if (node->children[i])
            return true;
    return false;
}

static void cleanup_empty_branches(BaseNTrie *trie, TrieNode *start) {
    TrieNode *current = start;
    while (current->parent) {
        TrieNode *parent = current->parent;

        // if node still has payload or any child → stop
        if (current->value != NULL || node_has_children(trie, current)) {
            break;
        }

        // find and unlink current from parent
        for (uint8_t i = 0; i < trie->base; ++i) {
            if (parent->children[i] == current) {
                parent->children[i] = NULL;
                break;
            }
        }

        // free current
        free(current->children);
        free(current);

        // move up
        current = parent;
    }
}

/* ----------------------------------------------------------------
 *  _print_all
 *
 *  Recursively walk the subtree, building up buf[] and printing
 *  whenever node->value != NULL.
 */
static void _print_all(const BaseNTrie *trie, TrieNode *node, char *buf,
                       int depth) {
    if (node->value) {
        buf[depth] = '\0';
        puts(buf);
    }
    for (uint8_t i = 0; i < trie->base; ++i) {
        TrieNode *child = node->children[i];
        if (child) {
            buf[depth] = trie->to_char(i);
            _print_all(trie, child, buf, depth + 1);
        }
    }
}
