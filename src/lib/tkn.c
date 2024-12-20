
#include "tkn.h"

tkn *tkn_i(alfn *ta, frfn *tf, tkn_tbl_i *ttif, tkn_pf *df, mc *s) {
    tkn *t = ta(sizeof(tkn));
    t->r = t->lno = t->cno = 1;
    t->pos = 0;
    t->ta = ta;
    t->tf = tf;
    t->ttif = ttif;
    t->df = df;
    t->t = ttif();
    t->s = s;
    return t;
}

void tkn_f(tkn *t) {
    if (!t || --t->r > 0) return;
    tbl_f(t->t);
    mc_f(t->s);
    t->tf(t);
}
