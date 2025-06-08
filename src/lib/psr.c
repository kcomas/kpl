
#include "psr.h"

psr *psr_i(alfn *pa, frfn *pf, tkn *tt, vr *ts, tbl *pt) {
    psr *p = pa(sizeof(psr));
    p->r = 1;
    p->pa = pa;
    p->pf = pf;
    p->tt = tt;
    p->ts = ts;
    p->pt = pt;
    return p;
}

void psr_f(psr *p) {
    if (!p || --p->r > 0) return;
    tkn_f(p->tt);
    vr_f(p->ts);
    tbl_f(p->pt);
    p->pf(p);
}
