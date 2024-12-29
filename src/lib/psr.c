
#include "psr.h"

psr *psr_i(alfn *pa, frfn *pf, frfn *ef, psr_tbl_i *prti, psr_pf *df, tkn *tt, vr *ts) {
    psr *p = pa(sizeof(psr));
    p->r = 1;
    p->idc = PARSER(USR);
    p->pa = pa;
    p->pf = pf;
    p->ef = ef;
    p->prti = prti;
    p->df = df;
    p->tt = tt;
    p->ts = ts;
    p->pt = prti();
    return p;
}

size_t psr_a(psr *const p, size_t id, psr_pf *pf, size_t l, ...) {
    tbl *tl = p->pt;
    te *kv;
    va_list a;
    size_t tkn_id;
    va_start(a, l);
    while (l > 0) {
        tkn_id = va_arg(a, size_t);
        if (tbl_g_i(tl, U6(tkn_id), &kv) == TBL_STAT(NF)) {
            // TODO add
        }
        tl = kv->d[2].p;
        l--;
    }
    va_end(a);
    if (kv->d[0].u6) return kv->d[0].u6;
    kv->d[0] = U6(p->idc++);
    kv->d[1] = P(pf);
    return kv->d[0].u6;
}

void psr_f(psr *p) {
    if (!p || --p->r > 0) return;
    tkn_f(p->tt);
    vr_f(p->ts);
    tbl_f(p->pt);
    p->pf(p);
}
