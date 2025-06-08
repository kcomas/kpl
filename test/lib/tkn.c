
#include "../../src/lib/tkn.h"
#include <ctype.h>
#include <stdio.h>

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
    t->cno = m->d[2].u6;
    t->pos = m->d[3].u6;
    size_t e = 0;
    un c = c4_g((char*) t->s->d, t->pos, &e);
    if (!isalnum(c.c.a)) return TKN_STAT(INV);
    while (isalnum(c.c.a)) {
        t->cno++;
        t->pos = e + 1;
        c = c4_g((char*) t->s->d, t->pos, &e);
    }
    m->d[0].u6 = TOKEN(VAR);
    m->d[4].u6 = t->pos;
    return TKN_STAT(OK);
}

static tkn_stat num(tkn *const t, te *const m) {
    while (isdigit(t->s->d[t->pos])) {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
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

static void standard(tkn *const t) {
    tkn_a(t, " ", TOKEN(UN), &ws);
    tkn_a(t, "\n", TOKEN(UN), &nl);
    tkn_a(t, "0", TOKEN(NUM), &num);
    tkn_a(t, "1", TOKEN(NUM), &num);
    tkn_a(t, "2", TOKEN(NUM), &num);
    tkn_a(t, "3", TOKEN(NUM), &num);
    tkn_a(t, "4", TOKEN(NUM), &num);
    tkn_a(t, "5", TOKEN(NUM), &num);
    tkn_a(t, "6", TOKEN(NUM), &num);
    tkn_a(t, "7", TOKEN(NUM), &num);
    tkn_a(t, "8", TOKEN(NUM), &num);
    tkn_a(t, "9", TOKEN(NUM), &num);
}

static void tknize(tkn *const t) {
    tkn_stat tstat;
    te *m = te_i(5, &malloc, &free);
    while ((tstat = tkn_n(t, m)) == TKN_STAT(OK)) {
        printf("id:%ld,lno:%lu,cno:%lu,start:%lu,end:%lu,", m->d[0].i6, m->d[1].u6, m->d[2].u6, m->d[3].u6, m->d[4].u6);
        for (size_t i = m->d[3].u6; i < m->d[4].u6; i++) putchar(t->s->d[i]);
        putchar('\n');
    }
    if (tstat != TKN_STAT(END)) exit(55);
    te_f(m);
}

static void btest(void) {
    const char *pgm = "sigma 123 Σ  si \n  bar bee sig ΣΩ";
    printf("%s\n", pgm);
    tkn *t = tkn_i(&malloc, &free, &entry_free, &mktbl, &df, mc_i_cstr(pgm, &malloc, &free));
    standard(t);
    tkn_a(t, "sigma", TOKEN(UN), &ft);
    tkn_a(t, "sig", TOKEN(UN), &ft);
    tkn_a(t, "Σ", TOKEN(UN), &ft);
    tkn_a(t, "ΣΩ", TOKEN(UN), &ft);
    tkn_a(t, "bar", TOKEN(UN), &ft);
    tkn_a(t, "bee", TOKEN(UN), &ft);
    tkn_p(t->t, 0);
    tknize(t);
    tkn_f(t);
}

static void stest(void) {
    const char *pgm = "0 Σ [12;44;67]\n";
    printf("%s\n", pgm);
    tkn *t = tkn_i(&malloc, &free, &entry_free, &mktbl, &df, mc_i_cstr(pgm, &malloc, &free));
    standard(t);
    tkn_a(t, "Σ", TOKEN(UN), &ft);
    tkn_a(t, ";", TOKEN(UN), &ft);
    tkn_a(t, "[", TOKEN(UN), &ft);
    tkn_a(t, "]", TOKEN(UN), &ft);
    tkn_p(t->t, 0);
    tknize(t);
    tkn_f(t);
}

int main(void) {
    btest();
    stest();
    return 0;
}
