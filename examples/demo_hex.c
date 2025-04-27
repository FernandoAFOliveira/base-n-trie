#include <stdio.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Create an hex trie with built-in mapping */
    BaseNTrie *trie = trie_create_hex();

    /* Two payloads we’ll attach to the keys */
    const char *payload1 = "first value";
    const char *payload2 = "second value";

    /* Insert the keys with their payloads */
    trie_insert(trie, "1A", (void *)payload1);
    trie_insert(trie, "B2", (void *)payload2);

    /* Search for existing and missing keys */
    void *found1 = trie_search(trie, "1A");
    void *found2 = trie_search(trie, "C3");

    printf("search \"1A\" → %s, found? %d\n", (char *)found1, found1 != NULL);
    printf("search \"C3\" → %s, found? %d\n", found2 ? (char *)found2 : "NULL",
           found2 != NULL);

    /* Delete one key and print its old payload */
    void *deletedPayload = trie_delete(trie, "1A");
    printf("deleted \"1A\" payload → %s\n",
           deletedPayload ? (char *)deletedPayload : "NULL");

    /* Verify deletion */
    void *postDelete = trie_search(trie, "1A");
    printf("after delete, search \"1A\" found? %d\n", postDelete != NULL);

    /* Clean up */
    trie_destroy(trie);
    return 0;
}
