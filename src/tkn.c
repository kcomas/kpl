
#include "tkn.h"

void tkn_p(tbl *tl, size_t idnt) {
    te *h = tl->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        c4 c = kv->d[0].c;
        printf("%c%c%c%c,", c.a, c.b, c.c, c.d);
        printf("%u", kv->d[1].u4);
        putchar('\n');
        if (kv->d[3].p) tkn_p(kv->d[3].p, idnt + 1);
        h = h->d[2].p;
    }
}

void tkn_m_p(const te *m, const mc *s) {
        printf("tid:%u,lno:%u,cno:%u,start:%u,end:%u,str:", tkn_m_g_i(m), tkn_m_g_l(m), tkn_m_g_c(m), tkn_m_g_s(m), tkn_m_g_e(m));
    for (uint16_t i = tkn_m_g_s(m); i < tkn_m_g_e(m); i++) putchar(s->d[i]);
}

size_t tkn_sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

tkn_stat tkn_df(tkn *t, te *m) {
    t->cno = tkn_m_g_c(m);
    t->pos = tkn_m_g_s(m);
    size_t e = 0;
    un c = c4_g((char*) t->s->d, t->pos, &e);
    if (!isalnum(c.c.a)) return TKN_STAT(INV);
    while (isalnum(c.c.a)) {
        t->cno++;
        t->pos = e + 1;
        c = c4_g((char*) t->s->d, t->pos, &e);
    }
    tkn_m_s_i(m, TCUST(VAR));
    tkn_m_s_e(m, t->pos);
    return TKN_STAT(OK);
}

tkn_stat tkn_num(tkn *t, te *m) {
    while (isdigit(t->s->d[t->pos])) {
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
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
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
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
    tkn_m_s_i(m, TCUST(SYM));
    if (t->pos - tkn_m_g_s(m) < 2) return TKN_STAT(INV);
    tkn_m_s_e(m, t->pos);
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

tkn_stat tkn_g_i6(const te *t, const mc *s, int64_t *i) {
    uint64_t v = 0;
    for(uint32_t p = tkn_m_g_s(t); p < tkn_m_g_e(t); p++) {
        char c = s->d[p];
        if (c < '0' || c > '9') return TKN_STAT(INV);
        v *= 10;
        v += s->d[p] - '0';
        if (v > INT64_MAX) return TKN_STAT(INV);
    }
    *i = v;
    return TKN_STAT(OK);
}

tkn_stat tkn_g_mc(const te *t, const mc *s, ssize_t off, const alfr *af, mc **v) {
    ssize_t start = tkn_m_g_s(t);
    ssize_t end = tkn_m_g_e(t);
    size_t i = 0;
    if (off > 0) start += off;
    else end += off;
    if (start < 0 || end < 0 || start > end) return TKN_STAT(INV);
    size_t l = end - start + sizeof(char);
    *v = mc_i(l, af);
    (*v)->l = l;
    while (start < end) (*v)->d[i++] = s->d[start++];
    return TKN_STAT(OK);
}
