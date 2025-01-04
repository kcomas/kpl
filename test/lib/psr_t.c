
#include "psr_t.h"

tbl *psr_mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tkn_sh, &c4_eq, tl, b);
    return t;
}

void psr_entry_free(void *p) {
    te *t = (te*) p;
    te_f(t->d[1].p);
    tbl_f(t->d[4].p);
    free(t);
}
