
#include "tkn.h"

uint16_t tkn_m_g_i(const te *m) {
    return u4_g_o(m->d[0], 0);
}

void tkn_m_s_i(te *m, uint16_t i) {
    m->d[0] = u4_s_o(m->d[0], 0, i);
}

uint16_t tkn_m_g_l(const te *m) {
    return u4_g_o(m->d[0], 1);
}

void tkn_m_s_l(te *m, uint16_t l) {
    m->d[0] = u4_s_o(m->d[0], 1, l);
}

uint32_t tkn_m_g_c(const te *m) {
    return u5_g_o(m->d[0], 1);
}

void tkn_m_s_c(te *m, uint32_t c) {
    m->d[0] = u5_s_o(m->d[0], 1, c);
}

uint32_t tkn_m_g_s(const te *m) {
    return u5_g_o(m->d[1], TKN_S);
}

void tkn_m_s_s(te *m, uint32_t s) {
    m->d[1] = u5_s_o(m->d[1], TKN_S, s);
}

uint32_t tkn_m_g_e(const te *m) {
    return u5_g_o(m->d[1], TKN_E);
}

void tkn_m_s_e(te *m, uint32_t e) {
    m->d[1] = u5_s_o(m->d[1], TKN_E, e);
}

tkn *tkn_i(const alfr *af, const alfr *ta, const alfr *ea, tkn_tbl_i ttif, tkn_pf df, mc *s) {
    tkn *t = af->a(sizeof(tkn));
    t->idc = TOKEN(_);
    t->r = t->lno = t->cno = 1;
    t->pos = 0;
    t->af = af;
    t->ta = ta;
    t->ea = ea;
    t->ttif = ttif;
    t->df = df;
    t->t = ttif();
    t->s = s;
    return t;
}

tkn *tkn_i_tkn(const tkn *t, mc *s) {
    tkn *tt = t->af->a(sizeof(tkn));
    tt->idc = TOKEN(_);
    tt->r = tt->lno = tt->cno = 1;
    tt->pos = 0;
    tt->af = t->af;
    tt->ta = t->ta;
    tt->ea = t->ea;
    tt->ttif = t->ttif;
    tt->df = t->df;
    tt->t = tbl_c(t->t);
    tt->s = s;
    return tt;
}

tkn *tkn_c(tkn *t) {
    t->r++;
    return t;
}

static void tkn_entry_f(void *v) {
    te *t = v;
    tbl_f(t->d[3].p);
    t->af->f(t);
}

static uint16_t entry_add(tkn *t, tbl *tl, size_t p, const char *s, uint16_t tid, tkn_pf *pf) {
    size_t e = 0;
    un c = c4_g(s, p, &e);
    for (;;) {
        te *kv = te_i(4, t->ta, tkn_entry_f);
        kv->d[0] = c;
        kv->d[1] = U4(TOKEN(UN));
        kv->d[3].p = t->ttif();
        tbl_a(tl, kv);
        tl = kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
        if (c.c.a == '\0') {
            if (tid) kv->d[1] = U4(tid);
            else kv->d[1] = U4(t->idc++);
            kv->d[2].p = pf;
            return kv->d[1].u4;
        }
    }
    return TOKEN(UN);
}

// tbl entry te[c4;id(0 for nf);tkn_pf;tbl]
uint16_t tkn_a(tkn *t, uint16_t tid, const char *s, tkn_pf *pf) {
    tbl *tl = t->t;
    size_t p = 0, e = 0;
    un c = c4_g(s, p, &e);
    te *kv = NULL;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) return entry_add(t, tl, p, s, tid, pf);
        tl = kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
    }
    if (kv->d[1].u4) return kv->d[1].u4;
    kv->d[1] = U4(t->idc++);
    kv->d[2] = P(pf);
    return kv->d[1].u4;
}

static void tkn_err_p(void *d) {
    tkn *t = d;
    printf("lno: %d, cno: %d", t->lno, t->cno);
}

tkn_stat tkn_err(tkn *t, err **e, const char *m) {
    if (!*e) *e = err_i(t->ea, tkn_err_p, (void*) tkn_f, tkn_c(t), m);
    return TKN_STAT(INV);
}

tkn_stat tkn_n(tkn *t, te *m, err **e) {
    size_t ce = 0;
    tkn_m_s_i(m, 0);
    tkn_m_s_l(m, t->lno);
    tkn_m_s_c(m, t->cno);
    tkn_m_s_s(m, t->pos);
    tkn_m_s_e(m, 0);
    un c = c4_g((char*) t->s->d, t->pos, &ce);
    if (c.c.a == '\0') return TKN_STAT(END);
    tbl *tl = t->t;
    tkn_pf *pf = t->df;
    te *kv;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) break;
        tl = kv->d[3].p;
        t->pos = ce + 1;
        t->cno++;
        if (kv->d[2].p) {
            pf = kv->d[2].p;
            tkn_m_s_i(m, kv->d[1].u4);
            tkn_m_s_e(m, t->pos);
        }
        c = c4_g((char*) t->s->d, t->pos, &ce);
    }
    return pf(t, m, e);
}

void tkn_s(tkn *t, te *m) {
    t->lno = tkn_m_g_l(m);
    t->cno = tkn_m_g_c(m);
    t->pos = tkn_m_g_s(m);
}

void tkn_f(tkn *t) {
    if (!t || --t->r > 0) return;
    tbl_f(t->t);
    mc_f(t->s);
    t->af->f(t);
}
