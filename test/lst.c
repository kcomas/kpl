
#include "../src/lib/mc.h"
#include "../src/lib/lst.h"
#include "t.h"

static void lst_i6_p(const lst * const l) {
    te *h = l->h;
    while (h) {
        printf("%ld ", h->d[0].i6);
        h = h->d[2].p;
    }
    putchar('\n');
}

T(lst_ab_af_sf_sb) {
    lst *l = lst_i(&al_lst, &al_te, NULL);
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
}

#define ABV_NUMS 1, 2, 3, 4, 5

T(lst_i_v) {
    const int64_t nums[] = {ABV_NUMS};
    size_t n = 5;
    lst *l = lst_i_v(&al_lst, &al_te, NULL, n, ABV_NUMS);
    A(l->l == n, "len");
    lst_i6_p(l);
    size_t i = 0;
    te *h = l->h;
    while (h) {
        A(h->d[0].i6 == nums[i++], "nums_i");
        h = h->d[2].p;
    }
    lst_f(l);
}

static bool vcmp(un a, un b) {
    return a.u6 == b.u6;
}

T(cmp) {
    size_t n = 5;
    lst *a = lst_i_v(&al_lst, &al_te, NULL, n, ABV_NUMS);
    lst *b = lst_i_v(&al_lst, &al_te, NULL, n, ABV_NUMS);
    A(lst_eq(a, b, vcmp), "lst_eq");
    lst *c = lst_i_v(&al_lst, &al_te, NULL, n, ABV_NUMS);
    c->t->d[0].u6 = 10;
    A(!lst_eq(b, c, vcmp), "!lst_eq");
    lst_f(a);
    lst_f(b);
    lst_f(c);
}

T(lst_sb) {
    size_t n = 5;
    lst *a = lst_i_v(&al_lst, &al_te, NULL, n, ABV_NUMS);
    un d;
    while (n > 0) {
        lst_sb(a, &d);
        n--;
    }
    A(a->l == 0, "lst_sb");
    lst_f(a);
}

static ssize_t cmp_str(un a, un b) {
    mc *ma = a.p;
    mc *mb = b.p;
    return strcmp((char*) ma->d, (char*) mb->d);
}

static ssize_t cmp_num(un a, un b) {
    if (a.i6 < b.i6) return -1;
    if (a.i6 > b.i6) return 1;
    return 0;
}

static bool eq_str(un a, un b) {
    return mc_eq(a.p, b.p);
}

static bool eq_num(un a, un b) {
    return a.i6 == b.i6;
}

T(sort) {
    lst *l = lst_i_v(&al_lst, &al_te, (void*) mc_f, 4, mc_i_cstr("d", &al_mc), mc_i_cstr("a", &al_mc), mc_i_cstr("c", &al_mc), mc_i_cstr("b", &al_mc));
    te *h = l->h;
    while (h) {
        printf("%s ", (char*) ((mc*) h->d[0].p)->d);
        h = h->d[2].p;
    }
    putchar('\n');
    lst_s(l, cmp_str);
    h = l->h;
    while (h) {
        printf("%s ", (char*) ((mc*) h->d[0].p)->d);
        h = h->d[2].p;
    }
    putchar('\n');
    h = l->t;
    while (h) {
        printf("%s ", (char*) ((mc*) h->d[0].p)->d);
        h = h->d[1].p;
    }
    putchar('\n');
    lst *c = lst_i_v(&al_lst, &al_te, (void*) mc_f, 4, mc_i_cstr("a", &al_mc), mc_i_cstr("b", &al_mc), mc_i_cstr("c", &al_mc), mc_i_cstr("d", &al_mc));
    A(lst_eq(l, c, eq_str), "not sorted");
    lst_f(l);
    lst_f(c);
    lst *a = lst_i_v(&al_lst, &al_te, NULL, 10, 8, 3, 9, 4, 10, 2, 7, 1, 6, 5);
    lst *b = lst_i_v(&al_lst, &al_te, NULL, 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    h = a->h;
    while (h) {
        printf("%ld ", h->d[0].i6);
        h = h->d[2].p;
    }
    putchar('\n');
    lst_s(a, cmp_num);
    h = a->h;
    while (h) {
        printf("%ld ", h->d[0].i6);
        h = h->d[2].p;
    }
    putchar('\n');
    A(lst_eq(a, b, eq_num), "not sorted num");
    lst_f(a);
    lst_f(b);
}
