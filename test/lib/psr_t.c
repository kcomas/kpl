
#include "psr_t.h"

size_t psr_sh(un u) {
    return u.u6;
}

void psr_ef(void *p) {
    te *t = (te*) p;
    tbl_f(t->d[3].p);
    free(t);
}

tbl *psr_mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tkn_sh, &c4_eq, tl, b);
    return t;
}

psr_stat psr_ign(psr *const p, te **const n) {
    (void) p;
    (void) n;
    return PSR_STAT(IGN);
}

psr_stat psr_val(psr *const p, te **const n) {
    (void) p;
    (void) n;
    return PSR_STAT(OK);
}

psr_stat psr_df(psr *const p, te **const n) {
    (void) p;
    (void) n;
    return PSR_STAT(INV);
}

