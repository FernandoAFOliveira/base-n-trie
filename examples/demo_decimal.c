#include <stdio.h>
#include "../src/base_n_trie.h"

int main(void) {
    BaseNTrie *t = trie_create(10); /* still hard-coded base 10 */

    trie_insert(t, "407");
    trie_insert(t, "12345");

    printf("search 407   → %d\n", trie_search(t, "407"));
    printf("search 999   → %d\n", trie_search(t, "999"));

    trie_delete(t, "407");
    printf("after delete search 407 → %d\n", trie_search(t, "407"));

    trie_destroy(t);
    return 0;
}
