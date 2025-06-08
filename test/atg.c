
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

T(fnadd3) {
    IC(fnadd3);
    RC();
    fast(_t, a, &an, bopt, false);
    atg *t = atg_i_atg(batg);
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    atg_tbl_p(t->ot, AST_CLS(O), 0);
    A(atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->q->l == 2, "atg_q");
    gen *g;
    err *e = NULL;
    atg_stat astat = atg_n(t, &g, a, &e);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(astat == ATG_STAT(OK), "atg_n");
    ast_p(an, 0);
    putchar('\n');
    gen_p(g, NULL);
    te *ft = TF(FN, TS(I6), 3, "a", TS(I6), "b", TS(I6), "c", TS(I6));
    te *cn = RN(LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), SN(_G, U5(0))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )));
    A(ast_eq(an, cn), "ast_eq");
    te_f(ft);
    te_f(cn);
    cn = NULL;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 2)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(NEG), gen_tmp(gc, X64_TYPE(I6), 1), gen_tmp(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 1), NULL, NULL));
    A(gen_code_eq(g, gc), "gen_code_eq");
    gen_f(gc);
    gen_st *sc = gen_st_i_gen_st(st);
    A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
    gen_stat gstat = gen_n(g, sc, t->a, &e);
    if (e) err_p(e);
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    sc = NULL;
    gen_f(g);
    g = gc = NULL;
    astat = atg_n(t, &g, a, &e);
    if (e) {
        err_p(e);
        err_f(e);
    }
    A(astat == ATG_STAT(OK), "atg_n");
    ast_p(an, 0);
    putchar('\n');
    gen_p(g, NULL);
    cn = RN(SN(_G, U5(1)));
    A(ast_eq(an, cn), "ast_eq");
    te_f(cn);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 3, gen_data(g, X64_TYPE(I6), I6(1)), gen_data(g, X64_TYPE(I6), I6(2)), gen_data(g, X64_TYPE(I6), I6(3))), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 3, gen_data(g, X64_TYPE(M), P(stdout)), gen_data(g, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_tmp(g, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    A(gen_code_eq(g, gc), "gen_code_eq");
    gen_f(gc);
    sc = gen_st_i_gen_st(st);
    A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
    gstat = gen_n(g, sc, t->a, &e);
    if (e) err_p(e);
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    gen_f(g);
    gen_st_f(st);
    ast_f(a);
    A(as_n(t->a, m, &e) == AS_STAT(OK), "as_n");
    as_code_p(t->a, m);
    uint32_t eidx = ((te*) an->d[4].p)->d[4].u5;
    te_f(an);
    A(eidx == 1, "eidx");
    ssize_t ep = as_lbl_g_c_i(t->a, eidx);
    A(ep > 0, "ep");
    atg_f(t);
    X64_RS();
    ((void (*)(void)) &m[ep])();
    X64_RR();
}
