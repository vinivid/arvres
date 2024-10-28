#include "rbt.h"
#include <stdio.h>

int main(void) {
    rbt* arvre = rbt_construct();

    for (int i = 0; i < 22; ++i) {
        rbt_insert(arvre, i);
    }

    rbt_insert(arvre, 2);
    rbt_node* a = rbt_search(arvre, 31);

    printf("a: %p\n", a);
    printf("root 0 :%d\n", arvre->root->key);
    rbt_delete(arvre, 13);
    printf("root 1 : %d\n", arvre->root->key);
    rbt_print(arvre);
    rbt_delete(arvre, 14);
    printf("root 2 : %d\n", arvre->root->key);
    rbt_print(arvre);
    rbt_delete(arvre, 16);
    printf("root 3 : %d\n", arvre->root->key);
    rbt_print(arvre);
    rbt_delete(arvre, 0);
    printf("root 4 : %d\n", arvre->root->key);
    rbt_print(arvre);
    rbt_destroy(arvre);
}