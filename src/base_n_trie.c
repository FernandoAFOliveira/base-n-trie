#include "base_n_trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

//
// -- Public API --
//

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

void print_trie(BaseNTrie *trie) {
    char buffer[MAX_KEY_LENGTH];
    buffer[0] = '\0';

    // recursive print helper
    extern void _print_all(const BaseNTrie *trie, TrieNode *node, char *buf,
                           int depth);

    _print_all(trie, trie->root, buffer, 0);
}

//
// -- Internal helpers --
//

static TrieNode *create_node(BaseNTrie *trie, TrieNode *parent) {
    TrieNode *node = xmalloc(sizeof *node);
    node->parent = parent;
    node->value = NULL;
    // children array:
    node->children = xmalloc(sizeof *node->children * trie->base);
    for (uint8_t i = 0; i < trie->base; ++i)
        node->children[i] = NULL;
    return node;
}

static void destroy_subtrie(BaseNTrie *trie, TrieNode *node) {
    for (uint8_t i = 0; i < trie->base; ++i) {
        if (node->children[i]) {
            destroy_subtrie(trie, node->children[i]);
        }
    }
    // Free the stored value (if any) via the user’s destructor
    if (node->value && trie->dtor) {
        // Call the destructor function to free the value
        trie->dtor(node->value);
    }
    // Free the children array and the node itself
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

// recursive printer
void _print_all(const BaseNTrie *trie, TrieNode *node, char *buf, int depth) {
    if (node->value) {
        buf[depth] = '\0';
        puts(buf);
    }
    for (uint8_t i = 0; i < trie->base; ++i) {
        if (node->children[i]) {
            buf[depth] = trie->to_char(i);
            _print_all(trie, node->children[i], buf, depth + 1);
        }
    }
}
