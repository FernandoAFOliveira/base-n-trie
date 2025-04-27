#ifndef BASE_N_TRIE_H
#define BASE_N_TRIE_H

#include <stddef.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

/*
 * Function pointer types for custom alphabets:
 *   symbol → index in [0..base-1]
 *   index  → symbol character
 */
typedef uint8_t (*to_index_fn)(char symbol);
typedef char (*to_char_fn)(uint8_t index);
typedef void (*destructor_fn)(void *value);

/* Opaque trie handle; fields are hidden from users */
typedef struct BaseNTrie_s BaseNTrie;

/* Public API */
BaseNTrie *trie_create(uint8_t base, to_index_fn to_index, to_char_fn to_char,
                       destructor_fn dtor);
void trie_destroy(BaseNTrie *trie);
int trie_insert(BaseNTrie *trie, const char *key, void *value);
void *trie_search(const BaseNTrie *trie, const char *key);
void *trie_delete(BaseNTrie *trie, const char *key);
void print_trie(BaseNTrie *trie);

/* Convenience: Decimal (base-10) */
static inline uint8_t dec_to_index(char symbol) {
    return (uint8_t)(symbol - '0');
}
static inline char dec_to_char(uint8_t index) {
    return (char)('0' + index);
}
static inline BaseNTrie *trie_create_decimal(void) {
    return trie_create(10, dec_to_index, dec_to_char, NULL);
}

/* Convenience: Octal (base-8) */
static inline uint8_t oct_to_index(char symbol) {
    return (uint8_t)(symbol - '0');
}
static inline char oct_to_char(uint8_t index) {
    return (char)('0' + index);
}
static inline BaseNTrie *trie_create_octal(void) {
    return trie_create(8, oct_to_index, oct_to_char, NULL);
}

/* Convenience: Hexadecimal (base-16) */
static inline uint8_t hex_to_index(char c) {
    if (c >= '0' && c <= '9')
        return (uint8_t)(c - '0');
    return (uint8_t)(10 + (uint8_t)(toupper((unsigned char)c) - 'A'));
}
static inline char hex_to_char(uint8_t index) {
    return index < 10 ? (char)('0' + index) : (char)('A' + (index - 10));
}
static inline BaseNTrie *trie_create_hex(void) {
    return trie_create(16, hex_to_index, hex_to_char, NULL);
}

/* Maximum key length buffer for print_trie */
#define MAX_KEY_LENGTH 256

#endif /* BASE_N_TRIE_H */
