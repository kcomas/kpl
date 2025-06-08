
#include "../../src/lib/tkn.h"
#include <stdio.h>

#define TOKEN(N) TOKEN_##N

typedef enum {
    TOKEN(SWORD),
    TOKEN(WS),
    TOKEN(SC),
    TOKEN(SCOC),
    TOKEN(SIG),
    TOKEN(NL)
} token;

static size_t sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &sh, &c4_eq, tl, b);
    return t;
}

static void entry_free(void *v) {
    te *t = (te*) v;
    tbl_f(t->d[3].p);
    free(t);
}

static tkn_stat df(tkn *const t, te *const m) {
    (void) t;
    (void) m;
    return TKN_STAT(INV);
}

static tkn_stat nl(tkn *const t, te *const m) {
    (void) m;
    t->lno++;
    t->cno = 1;
    return TKN_STAT(OK);
}

static tkn_stat ft(tkn *const t, te *const m) {
    (void) t;
    (void) m;
    return TKN_STAT(OK);
}

static tkn_stat ws(tkn *const t, te *const m) {
    while (t->s->d[t->pos] == ' ') {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
}

static void tkn_p(tbl *tl, size_t idnt) {
    te *h = tl->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        c4 c = kv->d[0].c;
        printf("%c%c%c%c ", c.a, c.b, c.c, c.d);
        ssize_t id = kv->d[1].i6;
        if (id > -1) printf("%ld", id);
        putchar('\n');
        if (kv->d[3].p) tkn_p(kv->d[3].p, idnt + 1);
        h = h->d[2].p;
    }
}

static void btest(void) {
    const char *pgm = "sigma Σ   \n   sig ΣΩ";
    tkn *t = tkn_i(&malloc, &free, &entry_free, &mktbl, &df, mc_i_cstr(pgm, &malloc, &free));
    tkn_a(t, " ", TOKEN(WS), &ws);
    tkn_a(t, "sigma", TOKEN(SWORD), &ft);
    tkn_a(t, "sig", TOKEN(SIG), &ft);
    tkn_a(t, "Σ", TOKEN(SC), &ft);
    tkn_a(t, "ΣΩ", TOKEN(SCOC), &ft);
    tkn_a(t, "\n", TOKEN(NL), &nl);
    tkn_p(t->t, 0);
    te *m = te_i(5, &malloc, &free);
    tkn_stat tstat;
    while ((tstat = tkn_n(t, m)) == TKN_STAT(OK)) {
        printf("id:%ld,lno:%lu,cno:%lu,start:%lu,end:%lu,", m->d[0].i6, m->d[1].u6, m->d[2].u6, m->d[3].u6, m->d[4].u6);
        for (size_t i = m->d[3].u6; i < m->d[4].u6; i++) putchar(t->s->d[i]);
        putchar('\n');
    }
    if (tstat != TKN_STAT(END)) exit(55);
    te_f(m);
    tkn_f(t);
}

int main(void) {
    btest();
    return 0;
}
