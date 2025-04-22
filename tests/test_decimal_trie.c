#include <stdio.h>
#include <assert.h>
#include "decimal_trie.h"

int main(void) {
    DecTrie *t = create_dectrie();

    /* Basic insert+search */
    insert(t, "123");
    insert(t, "0039");
    insert(t, "39");
    assert(search(t, "123") == 1);
    assert(search(t, "0039") == 1);
    assert(search(t, "39") == 1);
    assert(search(t, "999") == 0);

    /* remove_string */
    remove_string(t, "39");
    assert(search(t, "39") == 0);
    assert(search(t, "0039") == 1);  /* prefix should still work */

    destroy_dectrie(t);
    printf("✅ All decimal-trie tests passed!\n");
    return 0;
}
