
#include "psr_t.h"

void btest(void) {
    const char *pgm = "0 Σ [12;44;67]";
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    psr *p = psr_i(&malloc, &free, &psr_entry_free, &psr_mktbl, t);
    psr_f(p);
}

int main(void) {
    btest();
    return 0;
}
