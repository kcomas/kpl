
#include "../src/atg_x64.h"
#include "opt_t.h"
#include "gen_t.h"

atg *batg = NULL;

static __attribute__((constructor(107))) void atg_con(void) {
    batg = atg_b(atg_i(&ast_am, &ast_am, &ast_am, ast_err_p, cti, lst_i(&am, &am, NULL), ali(), gen_i_gen(bg), as_i_as(ba)));
}

static __attribute__((destructor)) void atg_des(void) {
    atg_f(batg);
}

static void atg_verify(_tests *_t, atg *t, ast *a, te *restrict an, te *restrict tn, gen *gc, gen_st *st) {
    E();
    err *e = NULL;
    gen *g;
    atg_stat astat = atg_n(t, &g, a, &e);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(astat == ATG_STAT(OK), "atg_n");
    ast_p(an, 0);
    putchar('\n');
    gen_p(g, NULL);
    A(ast_eq(an, tn), "ast_eq");
    A(gen_code_eq(g, gc), "gen_code_eq");
    gen_st *sc = gen_st_i_gen_st(st);
    A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
    gen_stat gstat = gen_n(g, sc, t->a, &e);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    gen_f(g);
}

#define V(CN, GC) atg_verify(_t, t, a, an, CN, GC, st);

static void atg_run(_tests *_t, atg *t, te *an, uint32_t elcmp) {
    E();
    size_t p = 0;
    err *e = NULL;
    A(as_n(t->a, &p, m, &e) == AS_STAT(OK), "as_n");
    as_code_p(t->a, m);
    uint32_t el = ((te*) an->d[4].p)->d[4].u5;
    te_f(an);
    A(el == elcmp, "el");
    ssize_t ep = as_lbl_g_c_i(t->a, el);
    atg_f(t);
    A(ep > -1, "ep");
    X64_RS();
    ((void (*)(void)) &m[ep])();
    X64_RR();
}

#define AR(E) gen_st_f(st); \
    ast_f(a); \
    atg_run(_t, t, an, E)

#define AI(PGM, QL) IC(PGM); \
    RC(); \
    fast(_t, a, &an, bopt, false); \
    atg *t = atg_i_atg(batg); \
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL)); \
    A(atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->q->l == QL, "atg_q")

T(tbl) {
    atg_tbl_p(batg->ot, AST_CLS(O), 0);
}

T(fnadd3) {
    AI(fnadd3, 2);
    te *ft = TFN(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2);
    te *cn = RN(LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), SN(_G, U5(0))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 2)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(NEG), gen_tmp(gc, X64_TYPE(I6), 1), gen_tmp(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 1), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, U5(1)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(I6), I6(1)), gen_data(gc, X64_TYPE(I6), I6(2)), gen_data(gc, X64_TYPE(I6), I6(3))), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(1);
}

T(fnf6muli6cstdiv) {
    AI(fnf6muli6cstdiv, 2);
    te *ft = TFN(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0);
    te *cn = RN(LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), SN(_G, U5(0))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), SN(U6, U6(2)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 1)));
    S(gen_a(gc, GEN_OP(CST), gen_tmp(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(DIV), gen_tmp(gc, X64_TYPE(F6), 2), gen_tmp(gc, X64_TYPE(F6), 0), gen_tmp(gc, X64_TYPE(F6), 1)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 2), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, U5(1)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(F6), 3), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(F6), F6(4.4)), gen_data(gc, X64_TYPE(F6), F6(6.6)), gen_data(gc, X64_TYPE(U6), U6(2))), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_tmp(gc, X64_TYPE(F6), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(1);
}

T(apltypefn) {
    AI(apltypefn, 2);
    te *ft = TFN(FN, TS(I6), 2, "a", TS(I6), 0, "b", TS(I6), 1);
    te *cn = RN(LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), SN(_G, U5(0))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 1), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, U5(1)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(I6), I6(3)), gen_data(gc, X64_TYPE(I6), I6(4))), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(1);
}
