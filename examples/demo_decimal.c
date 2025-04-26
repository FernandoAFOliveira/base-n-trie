#include <stdio.h>
#include "../src/base_n_trie.h"

int main(void) {
    /* Create a decimal trie with built-in mapping */
    BaseNTrie *trie = trie_create_decimal();

    /* Two payloads we’ll attach to the keys */
    const char *payload1 = "first value";
    const char *payload2 = "second value";

    /* Insert the keys with their payloads */
    trie_insert(trie, "407", (void *)payload1);
    trie_insert(trie, "12345", (void *)payload2);

    /* Search for existing and missing keys */
    void *found1 = trie_search(trie, "407");
    void *found2 = trie_search(trie, "999");

    printf("search \"407\" → %s, found? %d\n", (char *)found1, found1 != NULL);
    printf("search \"999\" → %s, found? %d\n", found2 ? (char *)found2 : "NULL",
           found2 != NULL);

    /* Delete one key and print its old payload */
    void *deletedPayload = trie_delete(trie, "407");
    printf("deleted \"407\" payload → %s\n",
           deletedPayload ? (char *)deletedPayload : "NULL");

    /* Verify deletion */
    void *postDelete = trie_search(trie, "407");
    printf("after delete, search \"407\" found? %d\n", postDelete != NULL);

    /* Clean up */
    trie_destroy(trie);
    return 0;
}
