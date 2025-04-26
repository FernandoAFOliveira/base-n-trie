#ifndef BASE_N_TRIE_H
#define BASE_N_TRIE_H

#include <stddef.h>
#include <stdint.h>

/*
 * Function pointer types for custom alphabets:
 * - symbol → index in [0..base-1]
 * - index → symbol character
 */
typedef uint8_t (*to_index_fn)(char symbol);
typedef char (*to_char_fn)(uint8_t index);

/*
 * Forward-declare the opaque trie handle.
 * Client code never sees its fields.
 */
typedef struct BaseNTrie_s BaseNTrie;

/*
 * Public API:
 *   trie_create:  allocate a new trie with given base & mapping functions
 *   trie_destroy: free all memory
 *
 *   trie_insert:  associate 'value' with 'key'; returns 0 on success
 *   trie_search:  return the stored value for 'key' or NULL
 *   trie_delete:  remove 'key' and return its old value or NULL
 *
 *   print_trie:   debug-print all stored keys (calls to_char for symbols)
 */
BaseNTrie *trie_create(uint8_t base, to_index_fn to_index, to_char_fn to_char);

void trie_destroy(BaseNTrie *trie);

int trie_insert(BaseNTrie *trie, const char *key, void *value);

void *trie_search(const BaseNTrie *trie, const char *key);

void *trie_delete(BaseNTrie *trie, const char *key);

/*
 * Optional convenience for a decimal-only trie:
 */
uint8_t dec_to_index(char symbol);
char dec_to_char(uint8_t index);

static inline BaseNTrie *trie_create_decimal(void) {
    return trie_create(10, dec_to_index, dec_to_char);
}

/*
 * For print_trie(): maximum key length buffer
 */
#define MAX_KEY_LENGTH 256

#endif /* BASE_N_TRIE_H */
