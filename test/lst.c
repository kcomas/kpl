
#include "../src/lib/lst.h"
#include "t.h"

I;

static void lst_i6_p(const lst * const l) {
    te *h = l->h;
    while (h) {
        printf("%ld ", h->d[0].i6);
        h = h->d[2].p;
    }
    putchar('\n');
}

static const alfr lm = { .a = malloc, .f = free };

T(lst_ab_af_sf_sb, {
    lst *l = lst_i(&lm, &lm, NULL);
    lst_ab(l, I6(1));
    lst_ab(l, I6(2));
    lst_ab(l, I6(3));
    lst_i6_p(l);
    lst_af(l, I6(4));
    A(l->h->d[0].i6 == 4, "Inv head");
    lst_i6_p(l);
    un u;
    lst_sf(l, &u);
    lst_sb(l, &u);
    A(l->t->d[0].i6 == 2, "Inv tail");
    lst_i6_p(l);
    lst_f(l);
});

#define ABV_NUMS 1, 2, 3, 4, 5

T(lst_abv, {
    lst *l = lst_i(&lm, &lm, NULL);
    const int64_t nums[] = {ABV_NUMS};
    size_t n = 5;
    A(lst_abv(l, n, ABV_NUMS) == LST_STAT(OK), "lst_abv");
    A(l->l == n, "len");
    lst_i6_p(l);
    size_t i = 0;
    te *h = l->h;
    while (h) {
        A(h->d[0].i6 == nums[i++], "nums_i");
        h = h->d[2].p;
    }
    lst_f(l);
});
