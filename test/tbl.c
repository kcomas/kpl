
#include "../src/lib/tbl.h"
#include "t.h"

static void kv_f(te *t) {
    mc_f(t->d[0].p);
    t->af->f(t);
}

static const alfr tm = { .a = malloc, .f = free };

static te *kv_i(const char *s, int64_t v) {
    te *t = te_i(2, &tm, (void*) kv_f);
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

T(tbl) {
    lst *tl = lst_i(&tm, &tm, (void*) te_f);
    te *b = te_i(1, &tm, NULL);
    tbl *t = tbl_i(&tm, tbl_mc_sdbm, tbl_mc_eq, tl, b);
    A(tbl_a(t, kv_i("Hello", 123)) == TBL_STAT(RES), "tbl_a");
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
