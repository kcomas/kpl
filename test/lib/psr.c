
#include "psr_t.h"

void btest(void) {
    const char *pgm = "0 Σ [12;44;67]\n";
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    vr *v = vr_i(5, &malloc, &tkn_entry_free, &free);
    psr *p = psr_i(&malloc, &free, &psr_ef, &psr_mktbl, &psr_df, t, v);
    psr_f(p);
}

int main(void) {
    btest();
    return 0;
}
