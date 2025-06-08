
#include "tkn.h"

void tkn_p(tbl *tl, size_t idnt) {
    te *h = tl->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        c4 c = kv->d[0].c;
        printf("%c%c%c%c,", c.a, c.b, c.c, c.d);
        ssize_t id = kv->d[1].i6;
        if (id > -1) printf("%ld", id);
        putchar('\n');
        if (kv->d[3].p) tkn_p(kv->d[3].p, idnt + 1);
        h = h->d[2].p;
    }
}

void tkn_m_p(const te *m, const mc *s) {
        printf("tid:%lu,lno:%lu,cno:%lu,start:%lu,end:%lu,str:", m->d[0].i6, m->d[1].u6, m->d[2].u6, m->d[3].u6, m->d[4].u6);
    for (size_t i = m->d[3].u6; i < m->d[4].u6; i++) putchar(s->d[i]);
}

size_t tkn_sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

void tkn_entry_f(void *v) {
    te *t = v;
    tbl_f(t->d[3].p);
    t->af->f(t);
}

tkn_stat tkn_df(tkn *t, te *m) {
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
    m->d[0].u6 = TCUST(VAR);
    m->d[4].u6 = t->pos;
    return TKN_STAT(OK);
}

tkn_stat tkn_num(tkn *t, te *m) {
    while (isdigit(t->s->d[t->pos])) {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
}

tkn_stat tkn_nl(tkn *t, te *m) {
    (void) m;
    t->lno++;
    t->cno = 1;
    return TKN_STAT(OK);
}

tkn_stat tkn_ft(tkn *t, te *m) {
    (void) t;
    (void) m;
    return TKN_STAT(OK);
}

tkn_stat tkn_ws(tkn *t, te *m) {
    while (t->s->d[t->pos] == ' ') {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
}

tkn_stat tkn_sym(tkn *t, te *m) {
    size_t e = 0;
    un c = c4_g((char*) t->s->d, t->pos, &e); // `
    c = c4_g((char*) t->s->d, t->pos, &e);
    if (!isalnum(c.c.a)) return TKN_STAT(INV);
    while (isalnum(c.c.a)) {
        t->cno++;
        t->pos = e + 1;
        c = c4_g((char*) t->s->d, t->pos, &e);
    }
    m->d[0].u6 = TCUST(SYM);
    if (t->pos - m->d[3].u6 < 2) return TKN_STAT(INV);
    m->d[4].u6 = t->pos;
    return TKN_STAT(OK);
}

void tkn_b(tkn *t) {
    tkn_a(t, TCUST(NL), "\n", tkn_nl);
    tkn_a(t, TCUST(SEMI), ";", tkn_ft);
    tkn_a(t, TCUST(WS), " ", tkn_ws);
    tkn_a(t, TCUST(NUM), "0", tkn_num);
    tkn_a(t, TCUST(NUM), "1", tkn_num);
    tkn_a(t, TCUST(NUM), "2", tkn_num);
    tkn_a(t, TCUST(NUM), "3", tkn_num);
    tkn_a(t, TCUST(NUM), "4", tkn_num);
    tkn_a(t, TCUST(NUM), "5", tkn_num);
    tkn_a(t, TCUST(NUM), "6", tkn_num);
    tkn_a(t, TCUST(NUM), "7", tkn_num);
    tkn_a(t, TCUST(NUM), "8", tkn_num);
    tkn_a(t, TCUST(NUM), "9", tkn_num);
    tkn_a(t, TCUST(SYM), "`", tkn_sym);
}

int64_t tkn_g_i6(const te *t, const mc *s) {
    int64_t i = 0;
    for(size_t p = t->d[3].u6; p < t->d[4].u6; p++) {
        i *= 10;
        i += s->d[p] - '0';
    }
    return i;
}
