
#include "../src/tkn.h"
#include "t.h"

static const alfr tm = { .a = malloc, .f = free };

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tkn_sh, c4_eq, tl, b);
    return t;
}

static tkn *btkn = NULL;

static __attribute__((constructor)) void tkn_con(void) {
    btkn = tkn_i(&tm, &al_te, &al_err, tkn_mktbl, tkn_df, mc_i(0, &al_mc));
    tkn_b(btkn);
}

static __attribute__((destructor)) void tkn_des(void) {
    tkn_f(btkn);
}

#define R(T, IDS) tkn_stat tstat; \
    te *m = te_i(2, &al_te, NULL); \
    err *e = NULL; \
    size_t id = 0; \
    while ((tstat = tkn_n(t, m, &e)) == TKN_STAT(OK)) { \
        tkn_m_p(m, t->s); \
        putchar('\n'); \
        A(tkn_m_g_i(m) == IDS[id++], "tid"); \
    } \
    A(tstat == TKN_STAT(END), "END"); \
    te_f(m);

T(btest) {
    const char *pgm = "sigma 123 Σ  si \n  bar bee sig ΣΩ";
    const uint16_t tids[] = {1, TCUST(WS), TCUST(NUM), TCUST(WS), 3, TCUST(WS), TCUST(VAR), TCUST(WS), TCUST(NL), TCUST(WS), 5, TCUST(WS), 6, TCUST(WS), 2, TCUST(WS), 4};
    printf("%s\n", pgm);
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    tkn_a(t, TOKEN(UN), "sigma", tkn_ft); // 1
    tkn_a(t, TOKEN(UN), "sig", tkn_ft); // 2
    tkn_a(t, TOKEN(UN), "Σ", tkn_ft); // 3
    tkn_a(t, TOKEN(UN), "ΣΩ", tkn_ft); // 4
    tkn_a(t, TOKEN(UN), "bar", tkn_ft); // 5
    tkn_a(t, TOKEN(UN), "bee", tkn_ft); // 6
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
}

T(stest) {
    const char *pgm = "0 Σ [12;44;67]\n";
    const uint16_t tids[] = {TCUST(NUM), TCUST(WS), 3, TCUST(WS), TCUST(LS), TCUST(NUM), TCUST(SEMI), TCUST(NUM), TCUST(SEMI), TCUST(NUM), TCUST(RS), TCUST(NL)};
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    printf("%s\n", pgm);
    tkn_a(t, TCUST(LS), "[", tkn_ft);
    tkn_a(t, TCUST(RS), "]", tkn_ft);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
}

T(cmt) {
    const char *pgm = "// a b c\nd";
    const uint16_t tids[] = {TCUST(CMT), TCUST(NL), TCUST(VAR)};
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    printf("%s\n", pgm);
    tkn_a(t, TCUST(CMT), "//", tkn_cmt);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
}

T(symtest) {
    const char *pgm = "a`b asdf`1234";
    const char *syms[] = {"b", "1234"};
    size_t i = 0;
    const size_t tids[] = {TCUST(VAR), TCUST(SYM), TCUST(WS), TCUST(VAR), TCUST(SYM)};
    printf("%s\n", pgm);
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    tkn_p(t->t, 0);
    tkn_stat tstat;
    te *m = te_i(2, &al_te, NULL);
    err *e = NULL;
    size_t id = 0;
    while ((tstat = tkn_n(t, m, &e)) == TKN_STAT(OK)) {
        tkn_m_p(m, t->s);
        putchar('\n');
        A(tkn_m_g_i(m) == tids[id++], "tid");
        if (tkn_m_g_i(m) == TCUST(SYM)) {
            mc *v;
            A(tkn_g_mc(m, t->s, 1, &al_mc, &v) == TKN_STAT(OK), "tkn_g_mc");
            printf("%s\n", (char*) v->d);
            A(strcmp(syms[i++], (char*) v->d) == 0, "sym");
            mc_f(v);
        }
    }
    A(tstat == TKN_STAT(END), "END");
    te_f(m);
    tkn_f(t);
}

T(inttest) {
    const char *pgm = "a 1 b 21 c 321";
    const int64_t nums[] = {1, 21, 321};
    size_t i = 0;
    const size_t tids[] = {TCUST(VAR), TCUST(WS), TCUST(NUM), TCUST(WS), TCUST(VAR), TCUST(WS), TCUST(NUM), TCUST(WS), TCUST(VAR), TCUST(WS), TCUST(NUM)};
    printf("%s\n", pgm);
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    tkn_p(t->t, 0);
    tkn_stat tstat;
    te *m = te_i(2, &al_te, NULL);
    err *e = NULL;
    size_t id = 0;
    while ((tstat = tkn_n(t, m, &e)) == TKN_STAT(OK)) {
        tkn_m_p(m, t->s);
        putchar('\n');
        A(tkn_m_g_i(m) == tids[id++], "tid");
        if (tkn_m_g_i(m) == TCUST(NUM)) {
            int64_t v = 0;
            A(tkn_g_i6(m, t->s, &v) == TKN_STAT(OK), "tkn_g_i6");
            A(nums[i++] == v, "num");
        }
    }
    A(tstat == TKN_STAT(END), "END");
    te_f(m);
    tkn_f(t);
}

T(err) {
    const char *pgm = "a`$";
    printf("%s\n", pgm);
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    tkn_stat tstat;
    te *m = te_i(2, &al_te, NULL);
    err *e = NULL;
    while ((tstat = tkn_n(t, m, &e)) == TKN_STAT(OK)) {
        tkn_m_p(m, t->s);
        putchar('\n');
    }
    A(e, "err");
    err_p(e, true);
    A(tstat == TKN_STAT(INV), "END");
    te_f(m);
    tkn_f(t);
    err_f(e);
}

T(str) {
    const char *pgm = "a \"b\" c";
    const uint16_t tids[] = {TCUST(VAR), TCUST(WS), TCUST(STR), TCUST(WS), TCUST(VAR)};
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    printf("%s\n", pgm);
    tkn_p(t->t, 0);
    R(t, tids);
    tkn_f(t);
}

T(strtest) {
    const char *pgm = "\"abc\" 12 \"1\"";
    mc *strs[] = {mc_i_cstr("abc", &al_mc), mc_i_cstr("1", &al_mc)};
    size_t i = 0;
    const size_t tids[] = {TCUST(STR), TCUST(WS), TCUST(NUM), TCUST(WS), TCUST(STR)};
    printf("%s\n", pgm);
    tkn *t = tkn_i_tkn(btkn, mc_i_cstr(pgm, &al_mc));
    tkn_p(t->t, 0);
    tkn_stat tstat;
    te *m = te_i(2, &al_te, NULL);
    err *e = NULL;
    size_t id = 0;
    while ((tstat = tkn_n(t, m, &e)) == TKN_STAT(OK)) {
        tkn_m_p(m, t->s);
        putchar('\n');
        A(tkn_m_g_i(m) == tids[id++], "tid");
        if (tkn_m_g_i(m) == TCUST(STR)) {
            mc *v;
            A(tkn_g_str(m, t->s, &al_mc, &v) == TKN_STAT(OK), "tkn_g_str");
            A(mc_eq(strs[i++], v), "str");
            mc_f(v);
        }
    }
    A(tstat == TKN_STAT(END), "END");
    te_f(m);
    tkn_f(t);
    for (size_t i = 0; i < 2; i++) mc_f(strs[i]);
}
