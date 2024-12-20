
#include "../../src/lib/tkn.h"

static size_t sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &sh, &c4_eq, tl, b);
    return t;
}

static tkn_stat df(tkn *const t, tkn_m *const m) {
    return TKN_STAT(NF);
}

int main(void) {
    const char *pgm = "0 Σ [1;2.2;333]";
    tkn *t = tkn_i(&malloc, &free, &mktbl, &df, mc_i_cstr(pgm, &malloc, &free));
    tkn_f(t);
    return 0;
}
