
#include "tkn.h"

tkn *tkn_i(const alfr *af, const alfr *ta, frfn *ef, tkn_tbl_i *ttif, tkn_pf *df, mc *s) {
    tkn *t = af->al(sizeof(tkn));
    t->idc = TOKEN(_);
    t->r = t->lno = t->cno = 1;
    t->pos = 0;
    t->af = af;
    t->ta = ta;
    t->ef = ef;
    t->ttif = ttif;
    t->df = df;
    t->t = ttif();
    t->s = s;
    return t;
}

static size_t entry_add(tkn *t, tbl *tl, size_t p, const char *s, size_t tid, tkn_pf *pf) {
    size_t e = 0;
    un c = c4_g(s, p, &e);
    for (;;) {
        te *kv = te_i(4, t->ta, t->ef);
        kv->d[0] = c;
        kv->d[1] = I6(TOKEN(UN));
        kv->d[3].p = t->ttif();
        tbl_a(tl, kv);
        tl = kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
        if (c.c.a == '\0') {
            if (tid) kv->d[1] = U6(tid);
            else kv->d[1] = U6(t->idc++);
            kv->d[2].p = pf;
            return kv->d[1].u6;
        }
    }
    return TOKEN(UN);
}

// tbl entry te[c4;id(0 for nf);tkn_pf;tbl]
size_t tkn_a(tkn *t, size_t tid, const char *s, tkn_pf *pf) {
    tbl *tl = t->t;
    size_t p = 0, e = 0;
    un c = c4_g(s, p, &e);
    te *kv;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) return entry_add(t, tl, p, s, tid, pf);
        tl = kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
    }
    if (kv->d[1].u6) return kv->d[1].u6;
    kv->d[1] = U6(t->idc++);
    kv->d[2] = P(pf);
    return kv->d[1].u6;
}

tkn_stat tkn_n(tkn *t, te *m) {
    size_t e = 0;
    m->d[0].u6 = 0;
    m->d[1].u6 = t->lno;
    m->d[2].u6 = t->cno;
    m->d[3].u6 = t->pos;
    m->d[4].u6 = 0;
    un c = c4_g((char*) t->s->d, t->pos, &e);
    if (c.c.a == '\0') return TKN_STAT(END);
    tbl *tl = t->t;
    tkn_pf *pf = t->df;
    te *kv;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) break;
        tl = kv->d[3].p;
        t->pos = e + 1;
        t->cno++;
        if (kv->d[2].p) {
            pf = kv->d[2].p;
            m->d[0] = kv->d[1];
            m->d[4].u6 = t->pos;
        }
        c = c4_g((char*) t->s->d, t->pos, &e);
    }
    return pf(t, m);
}

void tkn_s(tkn *t, te *m) {
    t->lno = m->d[1].u6;
    t->cno = m->d[2].u6;
    t->pos = m->d[3].u6;
}

void tkn_f(tkn *t) {
    if (!t || --t->r > 0) return;
    tbl_f(t->t);
    mc_f(t->s);
    t->af->fr(t);
}
