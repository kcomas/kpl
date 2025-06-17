
#include "../src/atg_x64.h"
#include "opt_t.h"
#include "gen_t.h"

atg *batg = NULL;

static __attribute__((constructor(107))) void atg_con(void) {
    batg = atg_b(atg_i(&ast_am, &al_te, &al_err, atg_err, cti, lst_i(&al_lst, &al_te, NULL), ali(), tbl_i(&al_tbl, type_un_hsh, type_un_eq, lst_i(&al_lst, &al_te, (void*) te_f), te_i(5, &al_te, NULL)), gen_i_gen(bg), as_i_as(ba)));
}

static __attribute__((destructor)) void atg_des(void) {
    atg_f(batg);
}

static size_t p_des = 0; // index for destructor code so it in m

static void atg_des_verify(_tests *_t, atg *t, te *restrict tn, const void *fg, gen_st *st) {
    E();
    te *h;
    if (tbl_g_i(t->dt, P(tn), &h) != TBL_STAT(OK)) {
        type_p(tn);
        putchar('\n');
        A(0, "des type nf");
    }
    type_p(h->d[0].p);
    putchar('\n');
    err *e = NULL;
    gen *g = NULL;
    gen_st *sc = NULL;
    void *fn = NULL;
    atg_stat stat = atg_d_n(t, h, &g, &fn, gen_type_des, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(stat == ATG_STAT(OK), "atg_d_n");
    if (fn) {
        A(fn == fg, "fn ptr neq");
        h->d[1] = P(fn);
        return;
    }
    if (g) {
        sc = gen_st_i_gen_st(st);
        A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
        gen_x64_opt(g, sc);
    }
    const gen *gc = fg;
    gen_p(g, NULL);
    bool eq = gen_code_eq(g, gc);
    if (!eq) {
        printf("--- DIFF ---\n");
        if (gc) gen_p(gc, NULL);
    }
    A(eq, "des_eq");
    if (g) {
        size_t pc = p;
        gen_stat gstat = gen_n(g, sc, t->a, &e);
        if (e) {
            err_p(e, true);
            err_f(e);
        }
        A(gstat == GEN_STAT(OK), "gen_n");
        as_stat astat = as_n(t->a, &p, m, &e);
        if (e) {
            err_p(e, true);
            err_f(e);
        }
        A(astat == AS_STAT(OK), "as_n");
        h->d[1] = P(&m[pc]);
        p = p_des;
    }
    gen_st_f(sc);
    gen_f(g);
}

#define D(TN, GC) atg_des_verify(_t, t, TN, GC, st)

static void atg_verify(_tests *_t, atg *t, ast *a, te *restrict an, te *restrict tn, const gen *gc, gen_st *st) {
    E();
    err *e = NULL;
    gen *g = NULL;
    atg_stat astat = atg_n(t, &g, a, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(astat == ATG_STAT(OK), "atg_n");
    ast_p(an, 0);
    bool eq = ast_eq(an, tn);
    if (!eq) {
        printf("\n--- DIFF ---\n");
        ast_p(tn, 0);
    }
    putchar('\n');
    A(eq, "ast_eq");
    gen_st *sc = gen_st_i_gen_st(st);
    A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
    gen_x64_opt(g, sc);
    gen_p(g, NULL);
    eq = gen_code_eq(g, gc);
    if (!eq) {
        printf("--- DIFF ---\n");
        if (gc) gen_p(gc, NULL);
    }
    A(eq, "gen_code_eq");
    gen_stat gstat = gen_n(g, sc, t->a, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    gen_f(g);
}

#define V(CN, GC) atg_verify(_t, t, a, an, CN, GC, st)

static void atg_run(_tests *_t, atg *t, te *an, uint32_t elcmp) {
    E();
    err *e = NULL;
    A(as_n(t->a, &p, m, &e) == AS_STAT(OK), "as_n");
    as_code_p(t->a, m);
    uint32_t el = ((te*) an->d[4].p)->d[4].u5;
    A(el == elcmp, "el");
    ssize_t ep = as_lbl_g_c_i(t->a, el);
    A(ep > -1, "ep");
    A(atg_z(t, an->d[3].p, m, ep) == NULL, "inv ret");
    atg_f(t);
    te_f(an);
}

#define AR(ELC) gen_st_f(st); \
    ast_f(a); \
    atg_run(_t, t, an, ELC); \
    E()

#define AI(PGM, DL, QL) IC(PGM); \
    RC(); \
    fast(_t, a, &an, bopt, false); \
    p = p_des; /* keep destructors */ \
    atg *t = atg_i_atg(batg); \
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL)); \
    size_t cdl = t->dt->i->l; \
    A(atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->dt->i->l - cdl == DL && t->q->l == QL, "atg_q");

T(tbl) {
    atg_tbl_p(batg->ot, AST_CLS(O), 0);
    atg_tbl_p(batg->at, AST_CLS(A), 0);
    atg_tbl_p(batg->vt, AST_CLS(V), 0);
}

T(fnadd3) {
    AI(TPGM(fnadd3), 0, 2);
    te *ft = TFN(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, I5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 2)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(NEG), gen_tmp(gc, X64_TYPE(I6), 1), gen_tmp(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 1), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, I5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(I6), I6(1)), gen_data(gc, X64_TYPE(I6), I6(2)), gen_data(gc, X64_TYPE(I6), I6(3))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(fnf6muli6cstdiv) {
    AI(TPGM(fnf6muli6cstdiv), 0, 2);
    te *ft = TFN(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, I5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), SN(U6, U6(2)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CST), gen_tmp(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 1)));
    S(gen_a(gc, GEN_OP(DIV), gen_tmp(gc, X64_TYPE(F6), 2), gen_tmp(gc, X64_TYPE(F6), 1), gen_tmp(gc, X64_TYPE(F6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 2), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, I5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(F6), 3), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(F6), F6(4.4)), gen_data(gc, X64_TYPE(F6), F6(6.6)), gen_data(gc, X64_TYPE(U6), U6(2))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(F6), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(apltypefn) {
    AI(TPGM(apltypefn), 0, 2);
    te *ft = TFN(FN, TS(I6), 2, "a", TS(I6), 0, "b", TS(I6), 1);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, I5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 1), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, I5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(I6), I6(3)), gen_data(gc, X64_TYPE(I6), I6(4))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(facloop) {
    AI(TPGM(facloop), 0, 1);
    te *cn = RN(SN(_G, I5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(5)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(LTE), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_stkv(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(SUB), gen_stkv(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(GT), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 1)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *s = mc_i_cstr("f", &al_mc);
    te *kv;
    A(tbl_g_i(et, P(s), &kv) == TBL_STAT(OK), "inv et");
    mc_f(s);
    A(kv->d[1].i6 == 120, "inv exp value");
    tbl_f(et);
}

T(facrec) {
    AI(TPGM(facrec), 0, 2);
    te *ft = TFN(FN, TS(U6), 1, "n", TS(U6), 0);
    te *cn = RN(LN(LT(1, "fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, I5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, SN(U6, U6(5)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(LTE), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(U6), 1), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 2), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(U6), 1)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 2)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, I5(3)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 4), gen_call_m(gc, 1, gen_data(gc, X64_TYPE(U6), U6(5))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(U6), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(3);
}

T(fibrec) {
    AI(TPGM(fibrec), 0, 2);
    te *ft = TFN(FN, TS(I6), 1, "n", TS(I6), 0);
    te *cn = RN(LN(LT(1, "fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, I5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, SN(I6, I6(8)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(GT), gen_arg(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(GTE), gen_arg(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(3)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(I6), 1), gen_arg(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(2))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 2), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(I6), 1)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(I6), 3), gen_arg(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 4), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(I6), 3)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 4), gen_tmp(gc, X64_TYPE(I6), 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(I6), 0), NULL, NULL));
    V(cn, gc);
    te_f(ft);
    te_f(cn);
    gen_f(gc);
    cn = RN(SN(_G, U5(5)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 6), gen_call_m(gc, 1, gen_data(gc, X64_TYPE(I6), U6(8))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 6)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(5);
}

T(ackrec) {
    AI(TPGM(ackrec), 0, 2);
    te *ft = TFN(FN, TS(U6), 2, "m", TS(U6), 0, "n", TS(U6), 1);
    te *cn = RN(LN(LT(1, "ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, U5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(U6, U6(2)), SN(U6, U6(1)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 1), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_arg(gc, X64_TYPE(U6), 1), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(LTE), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(U6), 2), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 2), gen_data(gc, X64_TYPE(U6), U6(1))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(LTE), gen_arg(gc, X64_TYPE(U6), 1), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 6)));
    S(gen_a(gc, GEN_OP(LTE), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 6)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(U6), 4), gen_arg(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(U6), 5), gen_arg(gc, X64_TYPE(U6), 1), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 6), gen_call_m(gc, 2, gen_arg(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 5)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 4), gen_tmp(gc, X64_TYPE(U6), 6)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_arg(gc, X64_TYPE(U6), 1), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(U6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    gen_f(gc);
    cn = RN(SN(_G, U5(7)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 7), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(U6), 9), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(U6), U6(1))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(U6), 9)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(7);
}

T(scope) {
    AI(TPGM(scope), 0, 2);
    tbl *args = fld_type_tbl_i(true, 1, "x", TS(I6), 0);
    tbl *scope = fld_type_tbl_i(true, 1, "s", TS(I6), 0);
    te *ft = TFS(NF, TS(VD), args, scope);
    te *fe = EN("x", FLG(-1, LTE_FLG(F)), te_c(ft));
    te *cn = RN(LN(LT(2, "s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6), "x", FLG(-1, LTE_FLG(F)), te_c(ft)), L(6,
        ON(TS(I6), DFN, EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
        ON(te_c(ft), DFN, te_c(fe), SN(_G, U5(-1))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(1)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(2)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(3)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_stka(gc, X64_TYPE(MI6), 0), gen_stka(gc, X64_TYPE(MI6), 0), gen_arg(gc, X64_TYPE(I6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), NULL,  NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(ft);
    te_f(fe);
    gen_f(gc);
    cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_w(gc, gen_vr(gc, 1, gen_stkv(gc, X64_TYPE(I6), 0)), gen_vr(gc, 1, gen_data(gc, X64_TYPE(I6), I6(1)))), gen_lbl(gc, (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_w(gc, gen_vr(gc, 1, gen_stkv(gc, X64_TYPE(I6), 0)), gen_vr(gc, 1, gen_data(gc, X64_TYPE(I6), I6(2)))), gen_lbl(gc, (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_w(gc, gen_vr(gc, 1, gen_stkv(gc, X64_TYPE(I6), 0)), gen_vr(gc, 1, gen_data(gc, X64_TYPE(I6), I6(3)))), gen_lbl(gc, (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *s = mc_i_cstr("s", &al_mc);
    te *kv;
    A(tbl_g_i(et, P(s), &kv) == TBL_STAT(OK), "inv et");
    mc_f(s);
    A(kv->d[1].i6 == 6, "inv exp value");
    tbl_f(et);

}

T(efn) {
    AI(TPGM(efn), 0, 2);
    te *tfn = TFN(FN, TS(F6), 3, "z", TS(F6), 0, "y", TS(F6), 1, "x", TS(F6), 2);
    te *cn = RN(LN(LT(1, "addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), L(1,
        ON(te_c(tfn), DFN, EN("addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), SN(_G, U5(-1)))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(F6), 0), gen_arg(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(F6), 2)));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(F6), 0), gen_tmp(gc, X64_TYPE(F6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 1),  NULL, NULL));
    V(cn, gc);
    te_f(cn);
    te_f(tfn);
    gen_f(gc);
    cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_data(gc, X64_TYPE(U5), U5(-1)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *s = mc_i_cstr("addmul", &al_mc);
    te *kv;
    A(tbl_g_i(et, P(s), &kv) == TBL_STAT(OK), "inv et");
    double x = 1.1, y = 2.2, z = 3.3;
    double r = ((double(*)(double, double, double)) &m[kv->d[1].u6])(z, y, x);
    A(r == z * (x + y), "addmul inv");
    mc_f(s);
    tbl_f(et);
}

T(teadd) {
    AI(TPGM(teadd), 1, 1);
    te *arr[] = {type_s_i(&al_te, NULL, TYPE(I6)), type_s_i(&al_te, NULL, TYPE(I6)), type_s_i(&al_te, NULL, TYPE(I6))};
    te *tte = type_te_i_v(&al_te, NULL, 3, te_c(arr[0]), te_c(arr[1]), te_c(arr[2]));
    D(tte, NULL);
    te_f(tte);
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(3)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48))), gen_data(gc, X64_TYPE(I6), I6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 1), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 2), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 3), gen_tmp(gc, X64_TYPE(I6), 2), gen_tmp(gc, X64_TYPE(I6), 1)));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(I6), 3), NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(vrmul) {
    AI(TPGM(vrmul), 1, 1);
    te *tvr = type_v_i(&al_te, NULL, TYPE(VR), type_i(&al_te, NULL, TYPE(F6)));
    D(tvr, NULL);
    te_f(tvr);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(1)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(1.1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U4(40))), gen_stkv(gc, X64_TYPE(F6), 0), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(GTE), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(100)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_stkv(gc, X64_TYPE(F6), 0), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(2))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), 1), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 2), gen_stkv(gc, X64_TYPE(F6), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(MM), 1), gen_tmp(gc, X64_TYPE(M), 2)), gen_data(gc, X64_TYPE(M), P(vr_ab)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(MM), 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LT), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(100)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(F6), 3, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 3))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 3), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 3), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    gen_f(gc);
    te_f(cn);
    AR(0);
}

T(st) {
    AI(TPGM(st), 1, 1);
    te *std = type_h_i(&al_te, NULL, TYPE(ST), fld_type_tbl_i(false, 4, "a", TS(I6), "b", TS(F6), "c", TS(U6), "d", TS(SG))), *kv;
    err *e = NULL;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(56)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    gen_type_aff(gc, std, &e, "");
    D(std, gc);
    void *des = NULL;
    A(tbl_g_i(t->dt, P(std), &kv) == TBL_STAT(OK), "std des nf");
    des = kv->d[1].p;
    te_f(std);
    gen_f(gc);
    te *cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(2.2)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(4)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(des))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_stkv(gc, X64_TYPE(F6), 0), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(2))));
    S(gen_a(gc, GEN_OP(CST), gen_tmp(gc, X64_TYPE(U6), 1), gen_tmp(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48))), gen_tmp(gc, X64_TYPE(U6), 1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_char(gc, "Hi"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(56))), gen_tmp(gc, X64_TYPE(M), 2), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 3), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(48))), NULL));
    S(gen_a(gc, GEN_OP(CST), gen_tmp(gc, X64_TYPE(I6), 3), gen_tmp(gc, X64_TYPE(U6), 3), NULL));
    S(gen_a(gc, GEN_OP(MUL), gen_tmp(gc, X64_TYPE(I6), 3), gen_data(gc, X64_TYPE(I6), I6(2)), gen_tmp(gc, X64_TYPE(I6), 3)));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_tmp(gc, X64_TYPE(I6), 3), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(ST)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_char(gc, "a")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_char(gc, "b")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(F6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_char(gc, "c")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_char(gc, "d")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 4), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(56))), gen_data(gc, X64_TYPE(U3), U3(offsetof(mc, d)))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *s = mc_i_cstr("s", &al_mc);
    A(tbl_g_i(et, P(s), &kv) == TBL_STAT(OK), "inv et");
    mc_f(s);
    type_p(kv->d[2].p);
    putchar('\n');
    te *arr = kv->d[1].p;
    A(arr->d[0].i6 == 6, "s`a");
    A(arr->d[1].f6 == 2.2, "s`b");
    A(arr->d[2].u6 == 3, "s`c");
    A(strcmp("Hi", (char*) ((mc*) arr->d[3].p)->d) == 0, "s`d");
    opt_exp_tbl_f(et);
}

T(import) {
    AI(TPGM(import), 0, 1);
    te *cn = RN(SN(_G, U5(0))), *kv;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(ET)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_char(gc, "b")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_data(gc, X64_TYPE(I6), I6(5050))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(10101)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *v = mc_i_cstr("v", &al_mc);
    A(tbl_g_i(et, P(v), &kv) == TBL_STAT(OK), "inv et");
    mc_f(v);
    A(kv->d[1].i6 == 10101, "inv exp v");
    opt_exp_tbl_f(et);
}

#define LFACCU5 392

T(lfac) {
    AI(TPGM(lfac), 0, 1);
    te *cn = RN(SN(_G, U5(0))), *kv;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 0), gen_call_m(gc, 1, gen_data(gc, X64_TYPE(I6), I6(6))), gen_data(gc, X64_TYPE(U5), U5(LFACCU5))));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *e = mc_i_cstr("e", &al_mc);
    A(tbl_g_i(et, P(e), &kv) == TBL_STAT(OK), "inv et");
    mc_f(e);
    A(kv->d[1].i6 == 720, "inv exp v");
    opt_exp_tbl_f(et);
}

T(istr) {
    AI(TPGM(istr), 0, 1);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(987)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(3.14)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(125))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 2), gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(offsetof(mc, d)))));
    S(gen_a(gc, GEN_OP(CALLV), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 5, gen_tmp(gc, X64_TYPE(M), 2), gen_tmp(gc, X64_TYPE(U6), 0), gen_char(gc, "a: %ld, b: %lf"), gen_stkv(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(F6), 0)), gen_data(gc, X64_TYPE(M), P(snprintf))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(offsetof(mc, l)))), gen_tmp(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 3), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(offsetof(mc, d)))));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(fnscope) {
    AI(TPGM(fnscope), 0, 5);
    te *fn = TFN(FN, TS(F6), 1, "n", TS(F6), 0);
    tbl *args = fld_type_tbl_i(true, 1, "n", TS(F6), 0);
    tbl *scope = fld_type_tbl_i(true, 1, "v", TS(F6), 0);
    te *ft = TFS(NF, TS(F6), args, scope);
    te *cn = RN(LN(LT(3, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "a", FLG(-1, LTE_FLG(F)), te_c(fn), "c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(5,
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), NN(T, te_c(fn))),
        ON(te_c(fn), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(1, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
                AN(TS(F6), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(3, "v", FLG(0, LTE_FLG(L)), TS(F6), "f", FLG(-3, LTE_FLG(F)), te_c(ft), "n", FLG(0, LTE_FLG(A)), TS(F6)), L(3,
                ON(TS(F6), DFN, EN("v", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(4.3))),
                ON(te_c(ft), DFN, EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)),
                    LN(LT(2, "n", FLG(0, LTE_FLG(A)), TS(F6), "v", FLG(0, LTE_FLG(S)), TS(F6)), L(1,
                        ON(TS(F6), SUB, EN("n", FLG(0, LTE_FLG(A)), TS(F6)), EN("v", FLG(0, LTE_FLG(S)), TS(F6)))))
                )),
                AN(TS(F6), EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
            ON(te_c(fn), DFN, EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -4)),
            ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(1,
                SN(F6, F6(9.6)))))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -4), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(F6), 0), gen_call_m(gc, 1, gen_arg(gc, X64_TYPE(F6), 0)), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(3, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "a", FLG(-1, LTE_FLG(F)), te_c(fn), "c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(5,
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), NN(T, te_c(fn))),
        ON(te_c(fn), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(1, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
                AN(TS(F6), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(3, "v", FLG(0, LTE_FLG(L)), TS(F6), "f", FLG(-3, LTE_FLG(F)), te_c(ft), "n", FLG(0, LTE_FLG(A)), TS(F6)), L(3,
                ON(TS(F6), DFN, EN("v", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(4.3))),
                ON(te_c(ft), DFN, EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), SN(_G, (uint32_t) -3)),
                AN(TS(F6), EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
            ON(te_c(fn), DFN, EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -4)),
            ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(1,
                SN(F6, F6(9.6)))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -3), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SUB), gen_tmp(gc, X64_TYPE(F6), 1), gen_arg(gc, X64_TYPE(F6), 0), gen_stka(gc, X64_TYPE(MF6), 0)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 1), NULL, NULL));
    V(cn, gc);
    te_f(ft);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(3, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "a", FLG(-1, LTE_FLG(F)), te_c(fn), "c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(5,
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), NN(T, te_c(fn))),
        ON(te_c(fn), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(1, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
                AN(TS(F6), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -2)),
            ON(te_c(fn), DFN, EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -4)),
            ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(1,
                SN(F6, F6(9.6)))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -2), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(4.3)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(F6), 2), gen_call_w(gc, gen_vr(gc, 1, gen_stkv(gc, X64_TYPE(F6), 0)), gen_vr(gc, 1, gen_arg(gc, X64_TYPE(F6), 0))), gen_lbl(gc, (uint32_t) -3)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 2), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(3, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "a", FLG(-1, LTE_FLG(F)), te_c(fn), "c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(5,
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), NN(T, te_c(fn))),
        ON(te_c(fn), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -1)),
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -2)),
            ON(te_c(fn), DFN, EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), SN(_G, (uint32_t) -4)),
            ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(1,
                SN(F6, F6(9.6)))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(F6), 3), gen_call_m(gc, 1, gen_arg(gc, X64_TYPE(F6), 0)), gen_lbl(gc, (uint32_t) -2)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 3), NULL, NULL));
    V(cn, gc);
    te_f(fn);
    te_f(cn);
    gen_f(gc);
    cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(F6), 4), gen_call_m(gc, 1, gen_data(gc, X64_TYPE(F6), F6(9.6))), gen_lbl(gc, (uint32_t) -4)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(F6), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(or) {
    AI(TPGM(or), 0, 1);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(EQ), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(I6), 1)));
    S(gen_a(gc, GEN_OP(CALLVNPR), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}
