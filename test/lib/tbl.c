
#include "../../src/lib/tbl.h"
#include <string.h>
#include <stdio.h>

static size_t sh(un u) {
    char *s = u.p;
    size_t h = 11;
    char c;
    while ((c = *s++)) h = c + (h << 4);
    return h;
}

static bool cmp(un a, un b) {
    return strcmp(a.p, b.p) == 0;
}

static void kv_f(te *t) {
    free(t->d[0].p);
    free(t);
}

static te *kv_i(const char *s, int64_t v) {
    te *t = te_i(2, &malloc, (void*) &kv_f);
    t->d[0].p = malloc(strlen(s) + sizeof(char));
    strcpy(t->d[0].p, s);
    t->d[1].i6 = v;
    return t;
}

static void pt(tbl *t) {
    for (size_t i = 0; i < t->b->l; i++) {
        printf("%lu:%p", i, t->b->d[i].p);
        if (i < t->b->l - 1) printf(", ");
    }
    putchar('\n');
}

void btable(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &sh, &cmp, tl, b);
    tbl_a(t, kv_i("Hello", 123));
    tbl_a(t, kv_i("World", 345));
    pt(t);
    te *kv;
    if (tbl_g_i(t, P("World"), &kv) != TBL_STAT(OK)) exit(11);
    printf("%lu\n", kv->d[1].i6);
    tbl_a(t, kv_i("World", 789));
    if (tbl_g_i(t, P("World"), &kv) != TBL_STAT(OK)) exit(11);
    printf("%lu\n", kv->d[1].i6);
    if (tbl_s(t, P("Hello"), &kv) != TBL_STAT(OK)) exit(11);
    printf("%lu\n", kv->d[1].i6);
    kv_f(kv);
    te *h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((te*) h->d[0].p)->d[0].p);
        h = h->d[2].p;
    }
    pt(t);
    tbl_f(t);
}

int main(void) {
    btable();
    return 0;
}
