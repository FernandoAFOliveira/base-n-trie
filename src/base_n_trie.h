#ifndef BASE_N_TRIE_H
#define BASE_N_TRIE_H

#include <stddef.h>
#include <stdint.h>

#define DIGIT_COUNT 10
#define MAX_KEY_LEN 20

typedef struct TrieNode_s {
    struct TrieNode_s *children[DIGIT_COUNT];
    int is_terminal;
} TrieNode;

typedef struct BaseNTrie_s {
    TrieNode *root;
} BaseNTrie;

/* Create/destroy */
BaseNTrie *trie_create(uint8_t base);
void trie_destroy(BaseNTrie *trie);

/* Core operations */
int trie_insert(BaseNTrie *trie, const char *key);
int trie_search(BaseNTrie *trie, const char *key);
int trie_delete(BaseNTrie *trie, const char *key);

void print_trie(BaseNTrie *trie);

#endif /* BASE_N_TRIE_H */
