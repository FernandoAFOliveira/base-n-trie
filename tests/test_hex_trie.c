#include <assert.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Use the hex convenience creator */
    BaseNTrie *trie = trie_create_hex();

    /* Two distinct payloads */
    const char *payloadA = "A"; // arbitrary payloads
    const char *payloadB = "B";

    /* Insert keys with payloads */
    assert(trie_insert(trie, "A1", (void *)payloadA) == 0);
    assert(trie_insert(trie, "B2", (void *)payloadB) == 0);

    // existing hex digits only
    assert(trie_search(trie, "A1") == (void *)payloadA);
    assert(trie_search(trie, "B2") == (void *)payloadB);

    /* Non-existent key returns NULL */
    assert(trie_search(trie, "C3") == NULL);

    /* Delete returns the old payload */
    void *deleted = trie_delete(trie, "A1");
    assert(deleted == (void *)payloadA);

    /* After deletion, "A1" is gone but "B2" remains */
    assert(trie_search(trie, "A1") == NULL);
    assert(trie_search(trie, "B2") == (void *)payloadB);

    trie_destroy(trie);
    return 0; /* all asserts passed */
}
