
#include "psr_t.h"

void btest(void) {
    const char *pgm = "0 Σ [12;5.4;5 - 4;15]";
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    vr *v = vr_i(10, &malloc, (void*) &te_f, &free);
    psr *p = psr_i(&malloc, &free, &psr_entry_free, &psr_mktbl, t, v);
    te *stmt_stop = te_i(2, &malloc, &free);
    stmt_stop->d[0] = U6(tkn_a(t, TCUST(SEMI), ";", &tkn_ft));
    stmt_stop->d[1] = U6(tkn_a(t, TCUST(NL), "\n", &tkn_ft));
    te *lst_stop = te_i(1, &malloc, &free);
    lst_stop->d[0] = U6(tkn_a(t, TOKEN(UN), "]", &tkn_ft));
    psr_a(p, PCUST(INT), PSR_MODE(ONCE), te_c(stmt_stop), NULL, &psr_val_m, &psr_int_i, 1, TCUST(NUM));
    psr_a(p, PCUST(SUM), PSR_MODE(ONCE), te_c(stmt_stop), NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "Σ", &tkn_ft));
    psr_a(p, PCUST(SUB), PSR_MODE(ONCE), te_c(stmt_stop), NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "-", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, TCUST(WS));
    psr_a(p, PCUST(FLT), PSR_MODE(ONCE), te_c(stmt_stop), NULL,  &psr_val_m, &psr_flt_i, 3, TCUST(NUM), tkn_a(t, TOKEN(UN), ".", &tkn_ft), TCUST(NUM));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), te_c(lst_stop), &psr_lst_e, &psr_val_m, &psr_lst_i, 1, tkn_a(t, TOKEN(UN), "[", &tkn_ft));
    tkn_p(t->t, 0);
    psr_p(p->pt, 0);
    printf("%s\n", pgm);
    psr_stat pstat;
    te *nh = te_i(3, &malloc, &free);
    if ((pstat = psr_n(p, nh, NULL)) != PSR_STAT(END)) exit(pstat);
    te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
    node_p(n, t->s, 0);
    putchar('\n');
    te_f(stmt_stop);
    te_f(lst_stop);
    psr_f(p);
    node_f(n);
    te_f(nh);
}

int main(void) {
    btest();
    return 0;
}
