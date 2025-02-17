
#include "../src/lib/tbl.h"
#include "t.h"

I;

static void kv_f(te *t) {
    t->af->f(t->d[0].p);
    t->af->f(t);
}

static const alfr tm = { .a = malloc, .f = free };

static te *kv_i(const char *s, int64_t v) {
    te *t = te_i(2, &tm, (void*) kv_f);
    t->d[0].p = tm.a(strlen(s) + sizeof(char));
    strcpy(t->d[0].p, s);
    t->d[1].i6 = v;
    return t;
}

static void pt(tbl *t) {
    for (size_t i = 0; i < t->b->l; i++) {
        te *li = t->b->d[i].p;
        printf("%lu:%p", i, li);
        if (li) {
            te *kv = li->d[0].p;
            printf(" %s,%lu", (char*) kv->d[0].p, kv->d[1].i6);
        }
        if (i < t->b->l - 1) printf(", ");
    }
    putchar('\n');
}

T(tbl) {
    lst *tl = lst_i(&tm, &tm, (void*) te_f);
    te *b = te_i(1, &tm, NULL);
    tbl *t = tbl_i(&tm, tbl_sdbm, tbl_str_eq, tl, b);
    A(tbl_a(t, kv_i("Hello", 123)) == TBL_STAT(RES), "tbl_a");
    pt(t);
    A(tbl_a(t, kv_i("World", 345)) == TBL_STAT(RES), "tbl_a");
    pt(t);
    te *h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((te*) h->d[0].p)->d[0].p);
        h = h->d[2].p;
    }
    te *kv;
    A(tbl_g_i(t, P("World"), &kv) == TBL_STAT(OK), "tbl_g_i");
    printf("%lu\n", kv->d[1].i6);
    A(tbl_a(t, kv_i("World", 789)) == TBL_STAT(OK), "tbl_a");
    A(tbl_g_i(t, P("World"), &kv) == TBL_STAT(OK), "tbl_g_i");
    printf("%lu\n", kv->d[1].i6);
    pt(t);
    A(tbl_s(t, P("Hello"), &kv) == TBL_STAT(OK), "tbl_s");
    printf("%lu\n", kv->d[1].i6);
    kv_f(kv);
    A(tbl_g_i(t, P("asdf"), &kv) == TBL_STAT(NF), "tbl_g_i");
    h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((te*) h->d[0].p)->d[0].p);
        h = h->d[2].p;
    }
    pt(t);
    tbl_f(t);
}
