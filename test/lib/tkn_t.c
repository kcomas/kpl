
#include "tkn_t.h"

size_t tkn_sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

void tkn_entry_free(void *v) {
    te *t = (te*) v;
    tbl_f(t->d[3].p);
    free(t);
}

tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tkn_sh, &c4_eq, tl, b);
    return t;
}

tkn_stat tkn_df(tkn *const t, te *const m) {
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

tkn_stat tkn_num(tkn *const t, te *const m) {
    while (isdigit(t->s->d[t->pos])) {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
}

tkn_stat tkn_nl(tkn *const t, te *const m) {
    (void) m;
    t->lno++;
    t->cno = 1;
    return TKN_STAT(OK);
}

tkn_stat tkn_ft(tkn *const t, te *const m) {
    (void) t;
    (void) m;
    return TKN_STAT(OK);
}

tkn_stat tkn_ws(tkn *const t, te *const m) {
    while (t->s->d[t->pos] == ' ') {
        t->pos++;
        m->d[4].u6++;
    }
    return TKN_STAT(OK);
}
