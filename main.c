#include "rbt.h"

int main(void) {
    rbt* arvre = rbt_construct();

    for (int i = 0; i < 22; ++i) {
        rbt_insert(arvre, i);
    }

    rbt_print(arvre);
}