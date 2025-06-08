
#include "tkn.h"

tkn *tkn_i(alfn *ta, frfn *tf, frfn *ef, tkn_tbl_i *ttif, tkn_pf *df, mc *s) {
    tkn *t = ta(sizeof(tkn));
    t->r = t->lno = t->cno = 1;
    t->pos = 0;
    t->ta = ta;
    t->tf = tf;
    t->ef = ef;
    t->ttif = ttif;
    t->df = df;
    t->t = ttif();
    t->s = s;
    return t;
}

static void entry_add(const tkn *const t, tbl *tl, size_t p, const char *s, ssize_t id, tkn_pf *pf) {
    size_t e = 0;
    un c = c4_g(s, p, &e);
    for (;;) {
        te *kv = te_i(4, t->ta, t->ef);
        kv->d[0] = c;
        kv->d[1] = I6(-1);
        kv->d[3].p = t->ttif();
        tbl_a(tl, kv);
        tl = kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
        if (c.c.a == '\0') {
            kv->d[1] = I6(id);
            kv->d[2].p = pf;
            break;
        }
    }
}

// tbl entry te[c4;id(-1 for inv);tkn_pf;tbl]
void tkn_a(tkn *const t, const char *const s, ssize_t id, tkn_pf *pf) {
    tbl *tl = t->t;
    size_t p = 0, e = 0;
    un c = c4_g(s, p, &e);
    te *kv;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) return entry_add(t, tl, p, s, id, pf);
        tl = (tbl*) kv->d[3].p;
        p = e + 1;
        c = c4_g(s, p, &e);
    }
    kv->d[1] = I6(id);
    kv->d[2].p = pf;
}

tkn_stat tkn_n(tkn *const t, te *const m) {
    size_t e = 0;
    m->d[0].i6 = -1;
    m->d[1].u6 = t->lno;
    m->d[2].u6 = t->cno;
    m->d[3].u6 = t->pos;
    m->d[4].u6 = 0;
    if (t->pos >= t->s->l) return TKN_STAT(END);
    un c = c4_g((char*) t->s->d, t->pos, &e);
    if (c.c.a == '\0') return TKN_STAT(END);
    tbl *tl = t->t;
    tkn_pf *pf = t->df;
    te *kv;
    while (c.c.a != '\0') {
        if (tbl_g_i(tl, c, &kv) == TBL_STAT(NF)) break;
        tl = (tbl*) kv->d[3].p;
        t->pos = e + 1;
        t->cno++;
        if ((tkn_pf*) kv->d[2].p) {
            pf = (tkn_pf*) kv->d[2].p;
            m->d[0] = kv->d[1];
            m->d[4].u6 = t->pos;
        }
        if (t->pos >= t->s->l) break;
        c = c4_g((char*) t->s->d, t->pos, &e);
    }
    return pf(t, m);
}

void tkn_f(tkn *t) {
    if (!t || --t->r > 0) return;
    tbl_f(t->t);
    mc_f(t->s);
    t->tf(t);
}
