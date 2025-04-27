#ifndef BASE_N_TRIE_H
#define BASE_N_TRIE_H

// TEMP: force CI to pick up new header
#define CI_FORCE_HEADER_UPDATE

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

/**
 * Function pointer types for custom alphabets:
 *   - to_index_fn: map a symbol (char) to an index in [0..base-1]
 *   - to_char_fn:  map an index back to its symbol character
 *   - destructor_fn: optional callback to free user-provided payloads
 */
typedef uint8_t (*to_index_fn)(char symbol);
typedef char (*to_char_fn)(uint8_t index);
typedef void (*destructor_fn)(void *value);

/**
 * Opaque handle for a base-N trie.
 * All implementation details are hidden from the user.
 */
typedef struct BaseNTrie_s BaseNTrie;

/**
 * Create a new trie for symbols in [0..base-1].
 * @param base    Number of distinct symbols (e.g. 10 for decimal)
 * @param to_index Function to convert each key character to an index
 * @param to_char  Function to convert an index back to a character
 * @param dtor     Optional destructor callback for values (may be NULL)
 * @return pointer to a newly allocated trie, or NULL on failure
 */
BaseNTrie *trie_create(uint8_t base, to_index_fn to_index, to_char_fn to_char,
                       destructor_fn dtor);

/**
 * Destroy the trie, free all nodes, and invoke the destructor on each payload.
 * @param trie Pointer returned by trie_create()
 */
void trie_destroy(BaseNTrie *trie);

/**
 * Insert or update a key → value mapping in the trie.
 * @param trie  The trie handle
 * @param key   NUL-terminated string of symbols
 * @param value User-provided pointer; may be freed by destructor
 * @return 0 on success, non-zero on error
 */
int trie_insert(BaseNTrie *trie, const char *key, void *value);

/**
 * Search for a key in the trie.
 * @param trie The trie handle
 * @param key  NUL-terminated string of symbols
 * @return the stored value, or NULL if not found
 */
void *trie_search(const BaseNTrie *trie, const char *key);

/**
 * Delete a key from the trie.
 * @param trie The trie handle
 * @param key  NUL-terminated string of symbols
 * @return the old value pointer, or NULL if the key wasn't present
 */
void *trie_delete(BaseNTrie *trie, const char *key);

/**
 * Debug: print all stored keys (uses to_char_fn for output).
 */
void print_trie(BaseNTrie *trie);

/*---------------------------------------------------------------------------*/
/* Convenience factories for common alphabets (decimal, octal, hex): */

/**
 * Create a base-10 (decimal) trie.  Uses no destructor (dtor == NULL) so
 * payloads won’t be freed automatically.
 */
static inline BaseNTrie *trie_create_decimal(void) {
    extern uint8_t dec_to_index(char);
    extern char dec_to_char(uint8_t);
    return trie_create(10, dec_to_index, dec_to_char, NULL);
}

/**
 * Create a base-8 (octal) trie.  Uses no destructor (dtor == NULL) so
 * payloads won’t be freed automatically.
 */
static inline BaseNTrie *trie_create_octal(void) {
    extern uint8_t oct_to_index(char);
    extern char oct_to_char(uint8_t);
    return trie_create(8, oct_to_index, oct_to_char, NULL);
}

/**
 * Create a base-16 (hexadecimal) trie.  Uses no destructor (dtor == NULL) so
 * payloads won’t be freed automatically.
 * Accepts digits 0-9 and letters A-F/a-f.
 */
static inline BaseNTrie *trie_create_hex(void) {
    extern uint8_t hex_to_index(char);
    extern char hex_to_char(uint8_t);
    return trie_create(16, hex_to_index, hex_to_char, NULL);
}
/**--------------------------------------------------------------------------
 * Maximum buffer length for print_trie()
 *
 *----------------------------------------------------------------------- */
#define MAX_KEY_LENGTH 256

#endif /* BASE_N_TRIE_H */
