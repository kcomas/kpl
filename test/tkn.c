
#include "../src/tkn.h"
#include "t.h"

I;

const alfr tm = { .a = malloc, .f = free };

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&tm, &tm, (void*) te_f);
    te *b = te_i(10, &tm, NULL);
    tbl *t = tbl_i(&tm, tkn_sh, c4_eq, tl, b);
    return t;
}

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
    const size_t tids[] = {1, TCUST(WS), TCUST(NUM), TCUST(WS), 3, TCUST(WS), TCUST(VAR), TCUST(WS), TCUST(NL), TCUST(WS), 5, TCUST(WS), 6, TCUST(WS), 2, TCUST(WS), 4};
    printf("%s\n", pgm);
    tkn *t = tkn_i(&tm, &tm, tkn_entry_f, tkn_mktbl, tkn_df, mc_i_cstr(pgm, &tm));
    tkn_b(t);
    tkn_a(t, TOKEN(UN), "sigma", tkn_ft); // 1
    tkn_a(t, TOKEN(UN), "sig", tkn_ft); // 2
    tkn_a(t, TOKEN(UN), "Σ", tkn_ft); // 3
    tkn_a(t, TOKEN(UN), "ΣΩ", tkn_ft); // 4
    tkn_a(t, TOKEN(UN), "bar", tkn_ft); // 5
    tkn_a(t, TOKEN(UN), "bee", tkn_ft); // 6
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});

T(stest, {
    const char *pgm = "0 Σ [12;44;67]\n";
    const size_t tids[] = {TCUST(NUM), TCUST(WS), 1, TCUST(WS), TCUST(LS), TCUST(NUM), TCUST(SEMI), TCUST(NUM), TCUST(SEMI), TCUST(NUM), TCUST(RS), TCUST(NL)};
    tkn *t = tkn_i(&tm, &tm, tkn_entry_f, tkn_mktbl, tkn_df, mc_i_cstr(pgm, &tm));
    printf("%s\n", pgm);
    tkn_b(t);
    tkn_a(t, TOKEN(UN), "Σ", tkn_ft);
    tkn_a(t, TCUST(LS), "[", tkn_ft);
    tkn_a(t, TCUST(RS), "]", tkn_ft);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});

T(symtest, {
    const char *pgm = "a`b asdf`1234";
    const size_t tids[] = {TCUST(VAR), TCUST(SYM), TCUST(WS), TCUST(VAR), TCUST(SYM)};
    printf("%s\n", pgm);
    tkn *t = tkn_i(&tm, &tm, tkn_entry_f, tkn_mktbl, tkn_df, mc_i_cstr(pgm, &tm));
    tkn_b(t);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
});
