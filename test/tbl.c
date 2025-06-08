
#include "../src/lib/tbl.h"
#include "t.h"

static void kv_f(te *t) {
    mc_f(t->d[0].p);
    t->af->f(t);
}

const alfr tm = { .a = malloc, .f = free };

static te *kv_i(const char *s, int64_t v) {
    te *t = te_i(2, &al_te, (void*) kv_f);
    t->d[0] = P(mc_i_cstr(s, &tm));
    t->d[1] = I6(v);
    return t;
}

static void pt(tbl *t) {
    for (size_t i = 0; i < t->b->l; i++) {
        te *li = t->b->d[i].p;
        printf("%lu:%p", i, li);
        if (li) {
            te *kv = li->d[0].p;
            printf(" %s,%lu", (char*) ((mc*) kv->d[0].p)->d, kv->d[1].i6);
        }
        if (i < t->b->l - 1) printf(", ");
    }
    putchar('\n');
}

static tbl *mt(size_t s) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(s, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_mc_sdbm, tbl_mc_eq, tl, b);
    return t;
}

T(tbl) {
    tbl *t = mt(2);
    A(tbl_a(t, kv_i("Hello", 123)) == TBL_STAT(OK), "tbl_a");
    pt(t);
    A(tbl_a(t, kv_i("World", 345)) == TBL_STAT(RES), "tbl_a");
    pt(t);
    te *h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((mc*) ((te*) h->d[0].p)->d[0].p)->d);
        h = h->d[2].p;
    }
    te *kv;
    mc *m;
    m = mc_i_cstr("World", &tm);
    A(tbl_g_i(t, P(m), &kv) == TBL_STAT(OK), "tbl_g_i");
    mc_f(m);
    printf("%lu\n", kv->d[1].i6);
    A(tbl_a(t, kv_i("World", 789)) == TBL_STAT(OK), "tbl_a");
    m = mc_i_cstr("World", &tm);
    A(tbl_g_i(t, P(m), &kv) == TBL_STAT(OK), "tbl_g_i");
    mc_f(m);
    printf("%lu\n", kv->d[1].i6);
    pt(t);
    m = mc_i_cstr("Hello", &tm);
    A(tbl_s(t, P(m), &kv) == TBL_STAT(OK), "tbl_s");
    mc_f(m);
    printf("%lu\n", kv->d[1].i6);
    kv_f(kv);
    m = mc_i_cstr("asdf", &tm);
    A(tbl_g_i(t, P(m), &kv) == TBL_STAT(NF), "tbl_g_i");
    mc_f(m);
    h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((mc*) ((te*) h->d[0].p)->d[0].p)->d);
        h = h->d[2].p;
    }
    pt(t);
    tbl_f(t);
}

static bool eq_fn(const te *restrict ea, const te *restrict eb) {
    return mc_eq(ea->d[0].p, eb->d[0].p) && ea->d[1].i6 == eb->d[1].i6;
}

T(eq) {
    tbl *a = mt(5);
    tbl_a(a, kv_i("a", 123));
    tbl_a(a, kv_i("b", 456));
    tbl *b = mt(5);
    tbl_a(b, kv_i("a", 123));
    tbl_a(b, kv_i("b", 456));
    A(tbl_eq(a, b, eq_fn), "tbl_eq");
    tbl *c = mt(5);
    tbl_a(c, kv_i("a", 123));
    tbl_a(c, kv_i("b", 789));
    A(!tbl_eq(a, c, eq_fn), "!tbl_eq");
    tbl_f(a);
    tbl_f(b);
    tbl_f(c);
}

static te *se(uint64_t a, uint64_t b) {
    te *t = te_i(2, &al_te, NULL);
    t->d[0].u6 = a;
    t->d[1].u6 = b;
    return t;
}

T(sparse) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(20, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    uint64_t x = 131081;
    tbl_a(t, se(x, 1));
    uint64_t y = 4295229449;
    tbl_a(t, se(y, 4));
    uint64_t z = 262153;
    tbl_a(t, se(z, 8));
    te *kv;
    A(tbl_g_i(t, U6(x), &kv) == TBL_STAT(OK), "tbl_g_i");
    A(kv->d[1].u6 == 1, "inv");
    A(tbl_g_i(t, U6(y), &kv) == TBL_STAT(OK), "tbl_g_i");
    A(kv->d[1].u6 == 4, "inv");
    A(tbl_g_i(t, U6(z), &kv) == TBL_STAT(OK), "tbl_g_i");
    A(kv->d[1].u6 == 8, "inv");
    tbl_f(t);
}
