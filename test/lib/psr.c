
#include "psr_t.h"

static psr *bpsr(const char *const pgm) {
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    vr *v = vr_i(10, &malloc, (void*) &te_f, &free);
    psr *p = psr_i(&malloc, &free, &psr_entry_free, &psr_mktbl, t, v);
    te *lst_stp = te_i(1, &malloc, &free);
    lst_stp->d[0] = U6(tkn_a(t, TCUST(RS), "]", &tkn_ft));
    te *blk_stp = te_i(1, &malloc, &free);
    blk_stp->d[0] = U6(tkn_a(t, TCUST(RB), "}", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, tkn_a(t, TCUST(SEMI), ";", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, tkn_a(t, TCUST(NL), "\n", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RS));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RB));
    psr_a(p, PCUST(INT), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_int_i, 1, TCUST(NUM));
    psr_a(p, PCUST(SUM), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "Σ", &tkn_ft));
    psr_a(p, PCUST(SUB), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "-", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, TCUST(WS));
    psr_a(p, PCUST(FLT), PSR_MODE(ONCE), NULL, NULL,  &psr_val_m, &psr_flt_i, 3, TCUST(NUM), tkn_a(t, TOKEN(UN), ".", &tkn_ft), TCUST(NUM));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), lst_stp, &psr_lst_e, &psr_val_m, &psr_lst_i, 1, tkn_a(t, TCUST(LS), "[", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), blk_stp, &psr_lst_e, &psr_val_m, &psr_lst_i, 1, tkn_a(t, TCUST(LB), "{", &tkn_ft));
    return p;
}

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

static void rpsr(psr *p) {
    psr_stat pstat;
    te *nh = te_i(3, &malloc, &free);
    if ((pstat = psr_n(p, nh)) != PSR_STAT(END)) exit(pstat);
    te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
    printf("%s\n", p->tt->s->d);
    node_p(n, p->tt->s, 0);
    putchar('\n');
    psr_f(p);
    node_f(n);
    te_f(nh);
}

int main(void) {
    rpsr(ppsr(bpsr("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]")));
    rpsr(bpsr("{3.2 - 2.1\n1 Σ [1;2;3]}"));
    return 0;
}
