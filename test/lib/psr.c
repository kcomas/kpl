
#include "psr_t.h"

void btest(void) {
    //const char *pgm = "0 Σ [12;44;67]";
    const char *pgm = "0 Σ 12 - 5";
    psr_stat pstat;
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    vr *v = vr_i(10, &malloc, (void*) &te_f, &free);
    psr *p = psr_i(&malloc, &free, &psr_entry_free, &psr_mktbl, t, v);
    te *stmt_stop = te_i(2, &malloc, &free);
    stmt_stop->d[0] = U6(TCUST(SEMI));
    stmt_stop->d[1] = U6(TCUST(NL));
    psr_a(p, PCUST(INT), PSR_MODE(ONCE), te_c(stmt_stop), &psr_val_m, &psr_val_i, 1, TCUST(NUM));
    psr_a(p, PCUST(SUM), PSR_MODE(ONCE), te_c(stmt_stop), &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "Σ", &tkn_ft));
    psr_a(p, PCUST(SUB), PSR_MODE(ONCE), te_c(stmt_stop), &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "-", &tkn_ft));
    psr_a(p, PCUST(WS), PSR_MODE(ONCE), NULL, NULL, NULL, 1, TCUST(WS));
    te *nh = te_i(3, &malloc, &free);
    if ((pstat = psr_n(p, nh)) != PSR_STAT(END)) exit(pstat);
    te *n = nh->d[0].p;
    node_p(n, p->tt->s, 0);
    te_f(stmt_stop);
    psr_f(p);
    node_f(n);
    te_f(nh);
}

int main(void) {
    btest();
    return 0;
}
