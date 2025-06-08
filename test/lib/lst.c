
#include "../../src/lib/lst.h"
#include <stdio.h>

static void lst_i6_p(const lst * const l) {
    lst_itm *h = l->h;
    while (h) {
        printf("%ld ", h->d.i6);
        h = h->n;
    }
    putchar('\n');
}

void basic(void) {
    lst *l = lst_i(&malloc, &malloc, &free, NULL, &free);
    lst_ab(l, I6(1));
    lst_ab(l, I6(2));
    lst_ab(l, I6(3));
    lst_i6_p(l);
    lst_af(l, I6(4));
    lst_i6_p(l);
    un u;
    lst_sf(l, &u);
    lst_sb(l, &u);
    lst_i6_p(l);
    lst_f(l);
}

int main(void) {
    basic();
    return 0;
}
