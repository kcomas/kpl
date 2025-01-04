
#include "psr.h"

psr *psr_i(alfn *pa, frfn *pf, frfn *pef, psr_tbl_i *pti, tkn *tt) {
    psr *p = pa(sizeof(psr));
    p->r = 1;
    p->pa = pa;
    p->pf = pf;
    p->pef = pef;
    p->pti = pti;
    p->tt = tt;
    p->pt = pti();
    return p;
}

size_t psr_a(psr *const p, size_t id, te *st, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...) {
    tbl *pt = p->pt;
    te *kv;
    size_t nid = 0;
    va_list tkns;
    va_start(tkns, nt);
    while (nt > 0) {
        size_t iid = va_arg(tkns, size_t);
        if (tbl_g_i(pt, U6(iid), &kv) == TBL_STAT(NF)) {
            while (nt > 0) {
                kv = te_i(6, p->pa, p->pef);
                kv->d[0] = U6(iid);
                kv->d[5] = P(p->pti());
                tbl_a(pt, kv);
                pt = kv->d[5].p;
                nt--;
            }
            kv->d[1] = U6(id);
            kv->d[2] = P(st);
            kv->d[3] = P(mf);
            kv->d[4] = P(nf);
            va_end(tkns);
            return id;
        }
        nid = kv->d[0].u6;
        pt = kv->d[4].p;
        nt--;
    }
    va_end(tkns);
    return nid;
}

void psr_f(psr *p) {
    if (!p || --p->r > 0) return;
    tkn_f(p->tt);
    tbl_f(p->pt);
    p->pf(p);
}
