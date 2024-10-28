#include "rbt.h"
#include <stdio.h>

int main(void) {
    rbt* arvre = rbt_construct();

    for (int i = 0; i < 22; ++i) {
        rbt_insert(arvre, i);
    }

    rbt_node* a = rbt_search(arvre, 31);

    printf("a: %p\n", a);
    rbt_delete(arvre, 21);
    rbt_print(arvre);
    rbt_delete(arvre, 20);
    rbt_print(arvre);
    rbt_delete(arvre, 19);
    rbt_print(arvre);
    rbt_delete(arvre, 18);

    rbt_print(arvre);
    rbt_destroy(arvre);
}