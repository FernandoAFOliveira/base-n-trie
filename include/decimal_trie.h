#ifndef DECIMAL_TRIE_H
#define DECIMAL_TRIE_H

#include <stddef.h>

#define DIGIT_COUNT 10

typedef struct DecTrieNode_s {
    struct DecTrieNode_s *children[DIGIT_COUNT];
    int is_terminal;
} DecTrieNode;

typedef struct DecTrie_s {
    DecTrieNode *root;
} DecTrie;

/* Create/destroy */
DecTrie *create_dectrie(void);
void destroy_dectrie(DecTrie *trie);

/* Core operations */
void insert(DecTrie *trie, const char *key);
int  search(DecTrie *trie, const char *key);
void remove_string(DecTrie *trie, const char *key);

#endif /* DECIMAL_TRIE_H */
