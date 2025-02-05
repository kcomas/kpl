
#include "../../src/lib/tbl.h"
#include <string.h>
#include <stdio.h>

static size_t sh(un u) {
    char *s = u.p;
    size_t h = 11;
    char c;
    while ((c = *s++)) h += c;
    return h;
}

static bool cmp(un a, un b) {
    return strcmp(a.p, b.p) == 0;
}

static void kv_f(te *t) {
    t->af->fr(t->d[0].p);
    t->af->fr(t);
}

static const alfr tm = { .al = &malloc, .fr = &free };

static te *kv_i(const char *s, int64_t v) {
    te *t = te_i(2, &tm, (void*) &kv_f);
    t->d[0].p = tm.al(strlen(s) + sizeof(char));
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

void btable(void) {
    lst *tl = lst_i(&tm, &tm, (void*) &te_f);
    te *b = te_i(1, &tm, NULL);
    tbl *t = tbl_i(&tm, &sh, &cmp, tl, b);
    tbl_stat tstat;
    if ((tstat = tbl_a(t, kv_i("Hello", 123))) != TBL_STAT(RES)) exit(tstat);
    pt(t);
    if ((tstat = tbl_a(t, kv_i("World", 345))) != TBL_STAT(RES)) exit(tstat);
    pt(t);
    te *h = t->i->h;
    while (h) {
        printf("%s\n", (char*) ((te*) h->d[0].p)->d[0].p);
        h = h->d[2].p;
    }
    te *kv;
    if ((tstat = tbl_g_i(t, P("World"), &kv)) != TBL_STAT(OK)) exit(tstat);
    printf("%lu\n", kv->d[1].i6);
    if ((tstat = tbl_a(t, kv_i("World", 789))) != TBL_STAT(OK)) exit(tstat);
    if ((tstat = tbl_g_i(t, P("World"), &kv)) != TBL_STAT(OK)) exit(tstat);
    printf("%lu\n", kv->d[1].i6);
    pt(t);
    if ((tstat = tbl_s(t, P("Hello"), &kv)) != TBL_STAT(OK)) exit(tstat);
    printf("%lu\n", kv->d[1].i6);
    kv_f(kv);
    if ((tstat = tbl_g_i(t, P("asdf"), &kv)) != TBL_STAT(NF)) exit(tstat);
    h = t->i->h;
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
