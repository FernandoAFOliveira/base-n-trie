#include <stdio.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Create an octal trie with built-in mapping */
    BaseNTrie *trie = trie_create_octal();

    /* Two payloads we’ll attach to the keys */
    const char *payload1 = "first value";
    const char *payload2 = "second value";

    /* Insert the keys with their payloads */
    trie_insert(trie, "17", (void *)payload1);
    trie_insert(trie, "7", (void *)payload2);

    /* Search for existing and missing keys */
    void *found1 = trie_search(trie, "17");
    void *found2 = trie_search(trie, "7");

    printf("search \"17\" → %s, found? %d\n", (char *)found1, found1 != NULL);
    printf("search \"7\" → %s, found? %d\n", found2 ? (char *)found2 : "NULL",
           found2 != NULL);

    /* Delete one key and print its old payload */
    void *deletedPayload = trie_delete(trie, "17");
    printf("deleted \"17\" payload → %s\n",
           deletedPayload ? (char *)deletedPayload : "NULL");

    /* Verify deletion */
    void *postDelete = trie_search(trie, "17");
    printf("after delete, search \"17\" found? %d\n", postDelete != NULL);

    /* Clean up */
    trie_destroy(trie);
    return 0;
}
