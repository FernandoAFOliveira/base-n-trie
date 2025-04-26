#include <assert.h>
#include "../src/base_n_trie.h"

int main(void) {
    BaseNTrie *t = trie_create(10); /* decimal for now */

    assert(trie_insert(t, "42") == 0);
    assert(trie_insert(t, "420") == 0);

    assert(trie_search(t, "42") == 1);
    assert(trie_search(t, "7") == 0);

    trie_delete(t, "42");
    assert(trie_search(t, "42") == 0);
    assert(trie_search(t, "420") == 1);

    trie_destroy(t);
    return 0; /* all asserts passed */
}
