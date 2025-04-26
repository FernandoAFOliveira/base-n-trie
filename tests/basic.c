#include <assert.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Use the decimal convenience creator */
    BaseNTrie *trie = trie_create_decimal();

    /* Two distinct payloads */
    const char *payloadA = "A";
    const char *payloadB = "B";

    /* Insert keys with payloads */
    assert(trie_insert(trie, "42", (void *)payloadA) == 0);
    assert(trie_insert(trie, "420", (void *)payloadB) == 0);

    /* Search returns the exact payload pointer */
    assert(trie_search(trie, "42") == (void *)payloadA);
    assert(trie_search(trie, "420") == (void *)payloadB);

    /* Non-existent key returns NULL */
    assert(trie_search(trie, "7") == NULL);

    /* Delete returns the old payload */
    void *deleted = trie_delete(trie, "42");
    assert(deleted == (void *)payloadA);

    /* After deletion, "42" is gone but "420" remains */
    assert(trie_search(trie, "42") == NULL);
    assert(trie_search(trie, "420") == (void *)payloadB);

    trie_destroy(trie);
    return 0; /* all asserts passed */
}
