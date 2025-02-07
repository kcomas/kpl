
#include "tkn_t.h"

I;

extern const alfr tm;

#define R(T, IDS) tkn_stat tstat; \
    te *m = te_i(5, &tm, NULL); \
    size_t id = 0; \
    while ((tstat = tkn_n(t, m)) == TKN_STAT(OK)) { \
        tkn_m_p(m, t->s); \
        putchar('\n'); \
        A(m->d[0].u6 == IDS[id++], "tid"); \
    } \
    A(tstat == TKN_STAT(END), "END"); \
    te_f(m);

T(btest, {
    const char *pgm = "sigma 123 Σ  si \n  bar bee sig ΣΩ";
    const size_t tids[] = {1, 1001, 1008, 1001, 3, 1001, 1010, 1001, 1000, 1001, 5, 1001, 6, 1001, 2, 1001, 4};
    printf("%s\n", pgm);
    tkn *t = tkn_i(&tm, &tm, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &tm));
    tkn_standard(t);
    tkn_a(t, TOKEN(UN), "sigma", &tkn_ft);
    tkn_a(t, TOKEN(UN), "sig", &tkn_ft);
    tkn_a(t, TOKEN(UN), "Σ", &tkn_ft);
    tkn_a(t, TOKEN(UN), "ΣΩ", &tkn_ft);
    tkn_a(t, TOKEN(UN), "bar", &tkn_ft);
    tkn_a(t, TOKEN(UN), "bee", &tkn_ft);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});

T(stest, {
    const char *pgm = "0 Σ [12;44;67]\n";
    const size_t tids[] = {1008, 1001, 1, 1001, 2, 1008, 1009, 1008, 1009, 1008, 3, 1000};
    tkn *t = tkn_i(&tm, &tm, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &tm));
    printf("%s\n", pgm);
    tkn_standard(t);
    tkn_a(t, TOKEN(UN), "Σ", &tkn_ft);
    tkn_a(t, TCUST(SEMI), ";", &tkn_ft);
    tkn_a(t, TOKEN(UN), "[", &tkn_ft);
    tkn_a(t, TOKEN(UN), "]", &tkn_ft);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});

T(symtest, {
    const char *pgm = "a`b asdf`1234";
    const size_t tids[] = {1010, 1011, 1001, 1010, 1011};
    printf("%s\n", pgm);
    tkn *t = tkn_i(&tm, &tm, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &tm));
    tkn_standard(t);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});
