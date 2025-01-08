
#include "tkn_t.h"

static void tknize(tkn *const t) {
    tkn_stat tstat;
    te *m = te_i(5, &malloc, &free);
    while ((tstat = tkn_n(t, m)) == TKN_STAT(OK)) {
        tkn_m_p(m, t->s);
        putchar('\n');
    }
    if (tstat != TKN_STAT(END)) exit(55);
    te_f(m);
}

static void btest(void) {
    const char *pgm = "sigma 123 Σ  si \n  bar bee sig ΣΩ";
    printf("%s\n", pgm);
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    tkn_a(t, TOKEN(UN), "sigma", &tkn_ft);
    tkn_a(t, TOKEN(UN), "sig", &tkn_ft);
    tkn_a(t, TOKEN(UN), "Σ", &tkn_ft);
    tkn_a(t, TOKEN(UN), "ΣΩ", &tkn_ft);
    tkn_a(t, TOKEN(UN), "bar", &tkn_ft);
    tkn_a(t, TOKEN(UN), "bee", &tkn_ft);
    tkn_p(t->t, 0);
    tknize(t);
    tkn_f(t);
}

static void stest(void) {
    const char *pgm = "0 Σ [12;44;67]\n";
    printf("%s\n", pgm);
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    tkn_a(t, TOKEN(UN), "Σ", &tkn_ft);
    tkn_a(t, TCUST(SEMI), ";", &tkn_ft);
    tkn_a(t, TOKEN(UN), "[", &tkn_ft);
    tkn_a(t, TOKEN(UN), "]", &tkn_ft);
    tkn_p(t->t, 0);
    tknize(t);
    tkn_f(t);
}

int main(void) {
    btest();
    stest();
    return 0;
}
