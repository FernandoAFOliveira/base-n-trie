#include <assert.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Use the octal convenience creator */
    BaseNTrie *trie = trie_create_octal();

    /* Two distinct payloads */
    const char *payloadA = "A"; // arbitrary payloads
    const char *payloadB = "B";

    /* Insert keys with payloads */
    assert(trie_insert(trie, "7", (void *)payloadA) == 0);
    assert(trie_insert(trie, "17", (void *)payloadB) == 0);

    // existing octal digits only
    assert(trie_search(trie, "7") == (void *)payloadA);
    assert(trie_search(trie, "17") == (void *)payloadB);

    /* Non-existent key returns NULL */
    assert(trie_search(trie, "8") == NULL);

    /* Delete returns the old payload */
    void *deleted = trie_delete(trie, "7");
    assert(deleted == (void *)payloadA);

    /* After deletion, "7" is gone but "17" remains */
    assert(trie_search(trie, "7") == NULL);
    assert(trie_search(trie, "17") == (void *)payloadB);

    trie_destroy(trie);
    return 0; /* all asserts passed */
}
