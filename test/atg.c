
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
        UNLOCK();
        as_stat astat = as_n(t->a, &p, m, &e);
        LOCK();
        if (e) {
            err_p(e, true);
            err_f(e);
        }
        A(astat == AS_STAT(OK), "as_n");
        h->d[1] = P(&m[pc]);
        as_f(t->a);
        t->a = as_i_as(ba);
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

static void atg_run(_tests *_t, atg *t, te *an, uint32_t elcmp, err **e) {
    E();
    UNLOCK();
    as_stat stat = as_n(t->a, &p, m, e);
    LOCK();
    if (*e) err_p(*e, true);
    A(stat == AS_STAT(OK), "as_n");
    as_code_p(t->a, m);
    uint32_t el = ((te*) an->d[4].p)->d[4].u5;
    A(el == elcmp, "el");
    ssize_t ep = as_lbl_g_c_i(t->a, el);
    A(ep > -1, "ep");
    *e = atg_y(t, an->d[3].p, m, ep);
}

#define AE(ELC) gen_st_f(st); \
    ast_f(a); \
    atg_run(_t, t, an, ELC, &e); \
    E(); \
    atg_f(t); \
    te_f(an); \


#define AR(ELC) AE(ELC); \
    A(e == NULL, "inv ret")

#define AI(PGM, DL, QL) IC(PGM); \
    RC(); \
    err *e = NULL; \
    fast(_t, a, &an, bopt, false); \
    atg *t = atg_i_atg(batg); \
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL)); \
    size_t cdl = t->dt->i->l; \
    bool okq = atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->dt->i->l - cdl == DL && t->q->l == QL; \
    if (!okq) printf("D: %lu, L: %lu\n", t->dt->i->l - cdl, t->q->l); \
    A(okq, "atg_q")

T(tbl) {
    atg_tbl_p(batg->ot, AST_CLS(O), 0);
    atg_tbl_p(batg->at, AST_CLS(A), 0);
    atg_tbl_p(batg->vt, AST_CLS(V), 0);
    p = 0; // do not reset per test, need reset here
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(F6), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 2)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(U6), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 6)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(U6), 9)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(F6), 3, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 3))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 3), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 3), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "a")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(F6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "b")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(U6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "c")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 4), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(56))), gen_data(gc, X64_TYPE(U3), U3(offsetof(mc, d)))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "d")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_data(gc, X64_TYPE(I6), I6(5050))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "b")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(10101)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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

#define LFACCU5 61440

T(lfac) {
    AI(TPGM(lfac), 0, 1);
    te *cn = RN(SN(_G, U5(0))), *kv;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 0), gen_call_m(gc, 1, gen_data(gc, X64_TYPE(I6), I6(6))), gen_data(gc, X64_TYPE(U5), U5(LFACCU5))));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_tmp(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(I6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(32))), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *me = mc_i_cstr("e", &al_mc);
    A(tbl_g_i(et, P(me), &kv) == TBL_STAT(OK), "inv et");
    mc_f(me);
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
            LN(LT(2, "b", U6(0), NULL, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
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
            LN(LT(2, "b", U6(0), NULL, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
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
            LN(LT(2, "b", U6(0), NULL, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
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
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(F6), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
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
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(I6), 1)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 1)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(un) {
    AI(TPGM(un), 1, 1);
    te *utd = type_h_i(&al_te, NULL, TYPE(UN), fld_type_tbl_i(false, 2, "a", TS(I6), "b", TS(F6)));
    D(utd, NULL);
    te_f(utd);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(F6), F6(2.2)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(UN)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "a")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(F6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "b")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 4), gen_call_m(gc, 2, gen_char(gc, "b"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 2, 0, 4, NULL, 5, 6, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(F6), 5), gen_idx_m(gc, X64_TYPE(F6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(F6), 5)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 8), gen_call_m(gc, 2, gen_char(gc, "a"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 6, 0, 8, NULL, 6, 6, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 9), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 9)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AE(0);
    err_p(e, true);
    A(e->m == atg_un_inv_str, "inv err str");
    A(((mc*) ((te*) e->d)->d[2].p)->d[0] == 'a', "inv err mc char");
    err_f(e);
}

T(unmatch) {
    AI(TPGM(unmatch), 1, 1);
    te *utd = type_h_i(&al_te, NULL, TYPE(UN), fld_type_tbl_i(false, 2, "a", TS(I6), "b", TS(I6))), *kv;
    D(utd, NULL);
    te_f(utd);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(2)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), (uint32_t) -1), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 3), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(66))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 5), gen_tmp(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_tmp(gc, X64_TYPE(U6), 3), gen_call_m(gc, 4, gen_tmp(gc, X64_TYPE(M), 5), gen_tmp(gc, X64_TYPE(U6), 3), gen_char(gc, "a %ld"), gen_tmp(gc, X64_TYPE(I6), (uint32_t) -1)), gen_data(gc, X64_TYPE(M), P(snprintf))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 3), gen_tmp(gc, X64_TYPE(U6), 3), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(16))), gen_tmp(gc, X64_TYPE(U6), 3), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), (uint32_t) -2), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 6), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 6), gen_tmp(gc, X64_TYPE(U6), 6), gen_data(gc, X64_TYPE(U6), U6(66))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 6), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 8), gen_tmp(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_tmp(gc, X64_TYPE(U6), 6), gen_call_m(gc, 4, gen_tmp(gc, X64_TYPE(M), 8), gen_tmp(gc, X64_TYPE(U6), 6), gen_char(gc, "b %ld"), gen_tmp(gc, X64_TYPE(I6), (uint32_t) -2)), gen_data(gc, X64_TYPE(M), P(snprintf))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 6), gen_tmp(gc, X64_TYPE(U6), 6), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(16))), gen_tmp(gc, X64_TYPE(U6), 6), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_char(gc, "inv"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 2), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 10), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 10)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *et = tbl_c(an->d[3].p);
    AR(0);
    A(et->i->l == 1, "inv et");
    mc *me = mc_i_cstr("c", &al_mc);
    A(tbl_g_i(et, P(me), &kv) == TBL_STAT(OK), "inv et");
    mc_f(me);
    me = mc_i_cstr("b 2", &al_mc);
    A(mc_eq(kv->d[1].p, me), "inv exp v");
    mc_f(me);
    opt_exp_tbl_f(et);
}

static void ns_gen(_tests *_t, gen *gc, uint16_t lno, uint32_t cno, uint32_t lbla, uint32_t lblb) {
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, lbla), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 2, gen_char(gc, "./ex/sum.kpl"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 0))), gen_data(gc, X64_TYPE(M), P(ns_f_rs))));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, lblb)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 4), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 2, 0, 4, NULL, lno, cno, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, lblb), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 5), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 6), gen_tmp(gc, X64_TYPE(M), 5), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 6)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
}

T(ns) {
    AI(TPGM(ns), 2, 1);
    te *ter = TV(ER, TS(SG));
    te *utd = type_h_i(&al_te, NULL, TYPE(UN), fld_type_tbl_i(false, 2, "e", te_c(ter), "v", TS(SG)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, utd, &e, "");
    D(utd, gc);
    te_f(utd);
    gen_f(gc);
    D(ter, mc_f);
    te_f(ter);
    te *cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    ns_gen(_t, gc, 4, 3, 0, 1);
    E();
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(iline) {
    AI(TPGM(iline), 0, 1);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    ns_gen(_t, gc, 1, 25, 0, 1);
    E();
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(unsg) {
    AI(TPGM(unsg), 1, 1);
    te *u = TH(UN, 2, "s", TS(SG), "v", TS(I6)), *kv;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, u, &e, "");
    D(u, gc);
    gen_f(gc);
    te *cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    A(tbl_g_i(t->dt, P(u), &kv) == TBL_STAT(OK), "cannot get un des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_char(gc, "a"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 2), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(UN)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 3), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "s")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "v")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(u);
    AR(0);
}

T(vridx) {
    vr *v;
    AI(TPGM(vridx), 5, 1);
    te *et = chk_rt_err_t(&al_te), *tsg = type_te_i_v(&al_te, NULL, 2, TS(SG), TS(SG)), *vv = TV(VR, te_c(tsg)), *u = TH(UN, 2, "e", te_c(et), "v", te_c(tsg)), *kv;
    D(vv, te_f);
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    gen_type_aff(gc, tsg, &e, "");
    D(tsg, gc);
    gen_f(gc);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, u, &e, "");
    D(u, gc);
    gen_f(gc);
    D(et, te_f);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    gen_type_aff(gc, u, et->d[2].p, "");
    D(et->d[2].p, gc);
    gen_f(gc);
    atg_d_p(t);
    te *cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(te_f))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    A(tbl_g_i(t->dt, P(tsg), &kv) == TBL_STAT(OK), "cannot get te(sg;sg) des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_char(gc, "a"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))), gen_tmp(gc, X64_TYPE(M), 2), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 3), gen_call_m(gc, 2, gen_char(gc, "b"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 3), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 4), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 5), gen_call_m(gc, 2, gen_char(gc, "c"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 4), gen_data(gc, X64_TYPE(U3), U3(32))), gen_tmp(gc, X64_TYPE(M), 5), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 6), gen_call_m(gc, 2, gen_char(gc, "d"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 4), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 6), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48))), gen_tmp(gc, X64_TYPE(M), 4), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(2)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 8), gen_idx_m(gc, X64_TYPE(M), 3, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 7)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 9), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 8), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 9)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 10), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 8), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 10)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 7), gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 7), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    A(tbl_g_i(t->dt, P(u), &kv) == TBL_STAT(OK), "cannot get er un des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 11), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 17), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(I6), I6(-1))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 18), gen_tmp(gc, X64_TYPE(I6), 17), NULL));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(I6), 17), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 3)));
    S(aply_e_vr_u(gc, gen_tmp(gc, X64_TYPE(U6), 18), gen_stkv(gc, X64_TYPE(M), 0), 3, 4, 5, 11, 12, 13, 14, 15, 16, NULL, 4, 3, X64_TYPE(M), true, 0, 1));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 11), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 19), gen_stkv(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(UN)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 19), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 7)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(ER)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 19), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(U3), U3(1))), gen_data(gc, X64_TYPE(M), P(err_p)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "e")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 7), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 20), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 19), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(TE)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 21), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 20), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 21)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 22), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 20), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 22)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "v")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 23), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(I6), 29), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(I6), I6(-1))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 30), gen_tmp(gc, X64_TYPE(I6), 29), NULL));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(I6), 29), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 8)));
    S(aply_e_vr_u(gc, gen_tmp(gc, X64_TYPE(U6), 30), gen_stkv(gc, X64_TYPE(M), 0), 8, 9, 10, 23, 24, 25, 26, 27, 28, NULL, 5, 3, X64_TYPE(M), true, 0, 1));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 2), gen_tmp(gc, X64_TYPE(M), 23), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 11)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 2)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 33), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 31, 0, 33, NULL, 5, 7, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 31), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 11), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 34), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 2), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 35), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 34), gen_data(gc, X64_TYPE(U3), U3(32))), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 36), gen_tmp(gc, X64_TYPE(M), 35), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 36)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 38), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 44), gen_data(gc, X64_TYPE(I6), I6(3)), NULL));
    S(aply_e_vr_u(gc, gen_tmp(gc, X64_TYPE(U6), 44), gen_stkv(gc, X64_TYPE(M), 0), 13, 14, 15, 38, 39, 40, 41, 42, 43, NULL, 6, 3, X64_TYPE(M), true, 0, 1));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 3), gen_tmp(gc, X64_TYPE(M), 38), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 12)));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), (uint32_t) -1), gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(40))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(ER)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 45), gen_tmp(gc, X64_TYPE(MM), (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(M), 45), gen_data(gc, X64_TYPE(U3), U3(1))), gen_data(gc, X64_TYPE(M), P(err_p)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 12), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 2)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(et);
    te_f(tsg);
    te_f(vv);
    te_f(u);
    tbl *xt = tbl_c(an->d[3].p);
    AR(0);
    A(xt->i->l == 1, "inv et");
    mc *mv = mc_i_cstr("v", &al_mc);
    A(tbl_g_i(xt, P(mv), &kv) == TBL_STAT(OK), "inv et");
    mc_f(mv);
    v = kv->d[1].p;
    A(v->l == 2, "inv vr len");
    te *ab = v->d[0].p;
    A(ab->l == 2, "inv te len");
    te *cd = v->d[1].p;
    A(cd->l == 2, "inv te len");
    opt_exp_tbl_f(xt);
}

T(nscope) {
    AI(TPGM(nscope), 0, 3);
    tbl *args = fld_type_tbl_i(true, 0);
    tbl *fbs = fld_type_tbl_i(true, 1, "x", TS(F6), 0);
    te *fb = TFS(NF, TS(F6), args, fbs);
    te *cn = RN(LN(LT(2, "x", FLG(0, LTE_FLG(L)), TS(F6), "fa", FLG(-1, LTE_FLG(F)), te_c(fb)), L(4,
        ON(TS(F6), DFN, EN("x", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(6.6))),
        ON(te_c(fb), DFN, EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), ON(te_c(fb), CST, NN(T, te_c(fb)),
            LN(LT(1, "fb", FLG(-2, LTE_FLG(F)), te_c(fb)), L(2,
                ON(te_c(fb), DFN, EN("fb", FLG(-2, LTE_FLG(F)), te_c(fb)), SN(_G, U5(-2))),
                AN(TS(F6), EN("fb", FLG(-2, LTE_FLG(F)), te_c(fb)), NULL)
        )))),
        AN(TS(F6), EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), NULL),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("x", FLG(0, LTE_FLG(L)), TS(F6)))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -2), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(DIV), gen_stka(gc, X64_TYPE(MF6), 0), gen_stka(gc, X64_TYPE(MF6), 0), gen_data(gc, X64_TYPE(F6), F6(2.0))));
    S(gen_a(gc, GEN_OP(LEAVE), gen_stka(gc, X64_TYPE(MF6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(2, "x", FLG(0, LTE_FLG(L)), TS(F6), "fa", FLG(-1, LTE_FLG(F)), te_c(fb)), L(4,
        ON(TS(F6), DFN, EN("x", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(6.6))),
        ON(te_c(fb), DFN, EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), SN(_G, U5(-1))),
        AN(TS(F6), EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), NULL),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("x", FLG(0, LTE_FLG(L)), TS(F6)))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(F6), 0), gen_call_w(gc, gen_vr(gc, 1, gen_stka(gc, X64_TYPE(MF6), 0)), NULL), gen_lbl(gc, (uint32_t) -2)));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(F6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(fb);
    cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(F6), 0), gen_data(gc, X64_TYPE(F6), F6(6.6)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(F6), 1), gen_call_w(gc, gen_vr(gc, 1, gen_stkv(gc, X64_TYPE(F6), 0)), NULL), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(F6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_stkv(gc, X64_TYPE(F6), 0)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AR(0);
}

T(cj) {
    AI(TPGM(cj), 1, 3);
    tbl *args = fld_type_tbl_i(true, 0);
    tbl *scope = fld_type_tbl_i(true, 1, "c", TS(I6), 0);
    te *nf = TFS(NF, TS(I6), args, scope), *kv;
    te *cj = TV(CJ, te_c(nf));
    te *fn = type_f_i(&al_te, NULL, TYPE(FN), te_c(cj), tbl_c(args), NULL);
    te *cn = RN(LN(LT(3, "mc", FLG(-1, LTE_FLG(F)), te_c(fn), "cc", FLG(0, LTE_FLG(L)), te_c(cj), "e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), L(5,
        ON(te_c(fn), DFN, EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)),
            ON(te_c(fn), CST, NN(T, te_c(fn)), LN(LT(2, "c", FLG(0, LTE_FLG(L)), TS(I6), "-1", FLG(-2, LTE_FLG(F)), te_c(nf)), L(2,
                ON(TS(I6), DFN, EN("c", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
                ON(te_c(cj), CST, NN(T, te_c(cj)),
                    ON(te_c(nf), DFN, EN("-1", FLG(-2, LTE_FLG(F)), te_c(nf)), SN(_G, U5(-2))))
            )))),
        ON(te_c(cj), DFN, EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), AN(te_c(cj), EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(I6), DFN, EN("e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL))
    )));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -2), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_stka(gc, X64_TYPE(MI6), 0), gen_stka(gc, X64_TYPE(MI6), 0), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(LEAVE), gen_stka(gc, X64_TYPE(MI6), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(3, "mc", FLG(-1, LTE_FLG(F)), te_c(fn), "cc", FLG(0, LTE_FLG(L)), te_c(cj), "e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), L(5,
        ON(te_c(fn), DFN, EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)), SN(_G, U5(-1))),
        ON(te_c(cj), DFN, EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), AN(te_c(cj), EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(I6), DFN, EN("e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_tmp(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), gen_lbl(gc, (uint32_t) -2), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(nf);
    te_f(cj);
    te_f(fn);
    cn = RN(SN(_G, U5(0)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 0), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 2), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 3), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 4), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 5), gen_call_w(gc, gen_vr(gc, 1, gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(32)))), NULL), gen_tmp(gc, X64_TYPE(M), 4)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 5)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 6), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 7), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 8), gen_call_w(gc, gen_vr(gc, 1, gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(32)))), NULL), gen_tmp(gc, X64_TYPE(M), 7)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 8)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 9), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 10), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 9), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(I6), 11), gen_call_w(gc, gen_vr(gc, 1, gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 9), gen_data(gc, X64_TYPE(U3), U3(32)))), NULL), gen_tmp(gc, X64_TYPE(M), 10)));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 1), gen_tmp(gc, X64_TYPE(I6), 11), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(I6), 1), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    gen_f(gc);
    te_f(cn);
    tbl *xt = tbl_c(an->d[3].p);
    AR(0);
    A(xt->i->l == 1, "inv et");
    mc *mv = mc_i_cstr("e", &al_mc);
    A(tbl_g_i(xt, P(mv), &kv) == TBL_STAT(OK), "inv et");
    mc_f(mv);
    A(kv->d[1].i6 == 3, "inv exp value");
    opt_exp_tbl_f(xt);
}

T(leap) {
    type_rf_test();
    AI(TPGM(leap), 1, 1);
    te *vt = TV(VR, TS(I6)), *kv;
    D(vt, NULL);
    te_f(vt);
    te *cn = RN(SN(_G, U5(0)));
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(2000)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(0)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(GT), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(2030)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(MOD), gen_tmp(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(400))));
    S(gen_a(gc, GEN_OP(EQ), gen_data(gc, X64_TYPE(I6), I6(0)), gen_tmp(gc, X64_TYPE(I6), 1), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(MOD), gen_tmp(gc, X64_TYPE(I6), 2), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(100))));
    S(gen_a(gc, GEN_OP(NE), gen_data(gc, X64_TYPE(I6), I6(0)), gen_tmp(gc, X64_TYPE(I6), 2), gen_tmp(gc, X64_TYPE(U3), 3)));
    S(gen_a(gc, GEN_OP(MOD), gen_tmp(gc, X64_TYPE(I6), 4), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(4))));
    S(gen_a(gc, GEN_OP(EQ), gen_data(gc, X64_TYPE(I6), I6(0)), gen_tmp(gc, X64_TYPE(I6), 4), gen_tmp(gc, X64_TYPE(U3), 5)));
    S(gen_a(gc, GEN_OP(AND), gen_tmp(gc, X64_TYPE(U3), 6), gen_tmp(gc, X64_TYPE(U3), 3), gen_tmp(gc, X64_TYPE(U3), 5)));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(U3), 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_tmp(gc, X64_TYPE(U3), 6), gen_data(gc, X64_TYPE(U3), U3(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), 8), gen_stkv(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 9), gen_stkv(gc, X64_TYPE(I6), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(MM), 8), gen_tmp(gc, X64_TYPE(M), 9)), gen_data(gc, X64_TYPE(M), P(vr_ab)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(MM), 8), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_stkv(gc, X64_TYPE(I6), 0), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(LTE), gen_stkv(gc, X64_TYPE(I6), 0), gen_data(gc, X64_TYPE(I6), I6(2030)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 6)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 10), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 3, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 10))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 10), gen_tmp(gc, X64_TYPE(U6), 10), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 10), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 5)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    tbl *xt = tbl_c(an->d[3].p);
    AR(0);
    A(xt->i->l == 1, "inv et");
    mc *mv = mc_i_cstr("v", &al_mc);
    A(tbl_g_i(xt, P(mv), &kv) == TBL_STAT(OK), "inv et");
    mc_f(mv);
    int64_t cmp[] = {2000, 2004, 2008, 2012, 2016, 2020, 2024, 2028};
    size_t l = 8;
    vr *v = kv->d[1].p;
    A(v->l == l, "vr inv len");
    for (size_t i = 0; i < v->l; i++) {
        printf("%ld ", v->d[i].i6);
        A(v->d[i].i6 == cmp[i], "inv year");
    }
    putchar('\n');
    opt_exp_tbl_f(xt);
}

T(vrup) {
    AI(TPGM(vrup), 1, 2);
    te *ue = chk_rt_err_t(&al_te), *ut = TH(UN, 2, "e", te_c(ue), "v", TS(I6)), *kv;
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, ut, &e, "");
    D(ut, gc);
    gen_f(gc);
    te *vt = TV(VR, TS(I6));
    te *ft = TFN(FN, te_c(ut), 1, "a", te_c(vt), 0);
    te *cn = RN(LN(LT(2, "v", FLG(0, LTE_FLG(L)), te_c(vt), "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(3,
        ON(te_c(vt), DFN, EN("v", FLG(0, LTE_FLG(L)), te_c(vt)), VN(te_c(vt), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))),
           ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), SN(_G, (uint32_t) -1)),
           ON(TS(I6), UNER, NULL, AN(te_c(ut), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, EN("v", FLG(0, LTE_FLG(L)), te_c(vt)))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 12), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(aply_e_vr_err_u(gc, gen_tmp(gc, X64_TYPE(U6), 12), gen_arg(gc, X64_TYPE(MM), 0), 3, 4, 6, 7, 8, 9, NULL, 4, 15));
    A(tbl_g_i(t->dt, P(ut), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 10), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 10), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 10), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 6), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 10), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 11), gen_data(gc, X64_TYPE(I6), I6(5)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 3, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 12)), gen_tmp(gc, X64_TYPE(I6), 11), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(I6), 11), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 6)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 13), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 3, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 13))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 13), gen_tmp(gc, X64_TYPE(U6), 13), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 13), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 5)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 26), gen_data(gc, X64_TYPE(I6), I6(5)), NULL));
    S(aply_e_vr_err_u(gc, gen_tmp(gc, X64_TYPE(U6), 26), gen_arg(gc, X64_TYPE(MM), 0), 10, 11, 20, 21, 22, 23, NULL, 6, 7));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 24), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 24), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 24), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 20), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 24), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 11), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 25), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 3, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 26)), gen_tmp(gc, X64_TYPE(I6), 25), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(I6), 25), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 27), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 27), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 27), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 27), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 27), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(vt);
    te_f(ue);
    te_f(ut);
    te_f(ft);
    cn = RN(SN(_G, U5(12)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 12), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 28), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(3)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 28), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 28), gen_data(gc, X64_TYPE(U3), U3(48))), gen_data(gc, X64_TYPE(I6), I6(2)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 28), gen_data(gc, X64_TYPE(U3), U3(56))), gen_data(gc, X64_TYPE(I6), I6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 28), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 28), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 28), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 28), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 29), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 0))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 29), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 14)));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 29), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 13)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 30), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 29), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 30), gen_data(gc, X64_TYPE(U3), U3(0))), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 30), gen_data(gc, X64_TYPE(U3), U3(0))), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 29)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 30), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 13), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 33), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 31, 0, 33, NULL, 9, 2, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 31), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 14), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 34), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 29), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 29)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(I6), 34), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    AE(12);
    err_p(e, true);
    kv = e->d;
    uint64_t lc = 6;
    lc <<= 32;
    lc += 7;
    A(kv->d[1].u6 == lc, "inv line no, char no");
    mc *s = kv->d[2].p;
    A(s->d[0] == '5', "inv vr idx err num");
    err_f(e);
}

T(stll) {
    AI(TPGM(stll), 2, 3);
    atg_d_p(t);
    te *kv;
    te *uv = TH(UN, 1, "y", TS(VD));
    te *utt = TH(UN, 2, "y", TS(VD), "z", TRF(2));
    te *stt = TH(ST, 2, "i", TS(I6), "n", te_c(utt));
    te *fa = TFN(FN, TS(VD), 2, "x", te_c(stt), 0, "i", TS(I6), 1);
    te *fb = TFN(FN, TS(VD), 1, "x", te_c(stt), 0);
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    gen_type_aff(gc, stt, &e, "");
    D(stt, gc);
    gen_f(gc);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, utt, &e, "");
    D(utt, gc);
    gen_f(gc);
    te *cn = RN(LN(LT(4, "l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt), "a", FLG(-1, LTE_FLG(F)), te_c(fa), "i", FLG(1, LTE_FLG(L)), TS(I6), "b", FLG(-2, LTE_FLG(F)), te_c(fb)), L(7,
        ON(te_c(stt), DFN, EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt)), ON(te_c(stt), MTCH, LN(NULL, NULL), LN(NULL, L(2,
                ZTN("i", TS(VD), SN(I6, I6(0))),
                ZTN("n", TS(VD), ZTN("y", te_c(utt), NULL))
            )))),
        ON(te_c(fa), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fa)), ON(te_c(fa), CST, NN(T, te_c(fa)),
            LN(LT(3, "i", FLG(1, LTE_FLG(A)), TS(I6), "x", FLG(0, LTE_FLG(A)), te_c(stt), "0", FLG(0, LTE_FLG(L)), TS(SG)), L(2,
                ON(TS(VD), DUMP, SN(U5, U5(1)), ON(TS(SG), DFN, EN("0", FLG(0, LTE_FLG(L)), TS(SG)),
                    AN(TS(SG), CS("a: %ld"), L(1, EN("i", FLG(1, LTE_FLG(A)), TS(I6)))))),
                ON(TS(VD), MTCH,
                    LN(NULL, L(1, ZTN("n", te_c(utt), EN("x", FLG(0, LTE_FLG(A)), te_c(stt))))),
                    LN(LT(4, "x", U6(0), NULL, "i", FLG(1, LTE_FLG(O) | LTE_FLG(A)), TS(I6), "a", U6(0), NULL, "z", FLG(-1, LTE_FLG(Y)), te_c(stt)), L(2,
                        ZTN("y", TS(_M), ON(te_c(stt), AGN,
                            ZTN("z", te_c(stt), ZTN("n", te_c(utt), EN("x", FLG(0, LTE_FLG(A)), te_c(stt)))),
                            ON(te_c(stt), MTCH, LN(NULL, NULL),
                                LN(LT(1, "i", FLG(1, LTE_FLG(O) | LTE_FLG(A)), TS(I6)), L(2,
                                    ZTN("i", TS(VD), EN("i", FLG(1, LTE_FLG(O) | LTE_FLG(A)), TS(I6))),
                                    ZTN("n", TS(VD), ZTN("y", te_c(uv), NULL))
                                ))))),
                        ZTN("z", TS(_M), AN(TS(VD), EN("a", FLG(-1, LTE_FLG(F)), te_c(fa)), L(2,
                            EN("z", FLG(-1, LTE_FLG(Y)), te_c(stt)),
                            EN("i", FLG(1, LTE_FLG(O) | LTE_FLG(A)), TS(I6))
                        )))
                    )))
            )))),
       ON(TS(I6), DFN, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
       ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), LTE, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(5))))),
            LN(NULL, L(1, AN(TS(VD), EN("a", FLG(-1, LTE_FLG(F)), te_c(fa)), L(2,
                EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt)),
                ON(TS(I6), ADDA, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))
            ))))),
       ON(TS(VD), DUMP, SN(U5, U5(1)), EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt))),
       ON(te_c(fb), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fb)), SN(_G, U5(-2))),
        AN(TS(VD), EN("b", FLG(-2, LTE_FLG(F)), te_c(fb)), L(1,
            EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -2), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(67))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 1), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 2), gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 3), gen_idx_m(gc, X64_TYPE(I6), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(32))), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_tmp(gc, X64_TYPE(U6), 0), gen_call_m(gc, 4, gen_tmp(gc, X64_TYPE(M), 2), gen_tmp(gc, X64_TYPE(U6), 0), gen_char(gc, "b: %ld"), gen_tmp(gc, X64_TYPE(I6), 3)), gen_data(gc, X64_TYPE(M), P(snprintf))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(U3), U3(16))), gen_tmp(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 4), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), 6), gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(MM), 6), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), (uint32_t) -2), gen_tmp(gc, X64_TYPE(MM), 6), gen_data(gc, X64_TYPE(U3), U3(40))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(MM), (uint32_t) -2)), gen_lbl(gc, (uint32_t) -2), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(LN(LT(4, "l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt), "a", FLG(-1, LTE_FLG(F)), te_c(fa), "i", FLG(1, LTE_FLG(L)), TS(I6), "b", FLG(-2, LTE_FLG(F)), te_c(fb)), L(7,
        ON(te_c(stt), DFN, EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt)), ON(te_c(stt), MTCH, LN(NULL, NULL), LN(NULL, L(2,
                ZTN("i", TS(VD), SN(I6, I6(0))),
                ZTN("n", TS(VD), ZTN("y", te_c(utt), NULL))
            )))),
        ON(te_c(fa), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fa)), SN(_G, U5(-1))),
       ON(TS(I6), DFN, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
       ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), LTE, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(5))))),
            LN(NULL, L(1, AN(TS(VD), EN("a", FLG(-1, LTE_FLG(F)), te_c(fa)), L(2,
                EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt)),
                ON(TS(I6), ADDA, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))
            ))))),
       ON(TS(VD), DUMP, SN(U5, U5(1)), EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt))),
       ON(te_c(fb), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fb)), SN(_G, U5(-2))),
        AN(TS(VD), EN("b", FLG(-2, LTE_FLG(F)), te_c(fb)), L(1,
            EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(stt))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 7), gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(U6), U6(67))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 8), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i))));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 9), gen_tmp(gc, X64_TYPE(M), 8), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_tmp(gc, X64_TYPE(U6), 7), gen_call_m(gc, 4, gen_tmp(gc, X64_TYPE(M), 9), gen_tmp(gc, X64_TYPE(U6), 7), gen_char(gc, "a: %ld"), gen_arg(gc, X64_TYPE(I6), 1)), gen_data(gc, X64_TYPE(M), P(snprintf))));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 7), gen_tmp(gc, X64_TYPE(U6), 7), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 8), gen_data(gc, X64_TYPE(U3), U3(16))), gen_tmp(gc, X64_TYPE(U6), 7), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 8), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 8), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 10), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(SG)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(M), 10)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), 12), gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(MM), 12), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    A(tbl_g_i(t->dt, P(stt), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 13), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 13), gen_data(gc, X64_TYPE(U3), U3(32))), gen_arg(gc, X64_TYPE(I6), 1), NULL));
    A(tbl_g_i(t->dt, P(utt), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 14), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 14), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 14), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 13), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 14), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 13), NULL, NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), 15), gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 17), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 17), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 17), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 13), NULL));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(M), 16), gen_tmp(gc, X64_TYPE(MM), 15), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 16)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(MM), 15), gen_tmp(gc, X64_TYPE(M), 17), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(MM), 12), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(REF), gen_tmp(gc, X64_TYPE(MM), (uint32_t) -1), gen_tmp(gc, X64_TYPE(MM), 12), gen_data(gc, X64_TYPE(U3), U3(40))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 2, gen_tmp(gc, X64_TYPE(MM), (uint32_t) -1), gen_arg(gc, X64_TYPE(I6), 1)), gen_lbl(gc, (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(SN(_G, U5(3)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    A(tbl_g_i(t->dt, P(stt), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 18), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 18), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    A(tbl_g_i(t->dt, P(utt), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 19), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 19), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 19), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 18), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 19), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 18), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 18), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(gen_a(gc, GEN_OP(GT), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(5)), gen_lbl(gc, 5)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_stkv(gc, X64_TYPE(I6), 1), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 2, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_stkv(gc, X64_TYPE(I6), 1)), gen_lbl(gc, (uint32_t) -1), NULL));
    S(gen_a(gc, GEN_OP(LTE), gen_stkv(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(5)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(ST)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "i")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 20), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(UN)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 20), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 7)));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VD)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "y")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 7), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(RF)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "z")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_sym_str)), gen_char(gc, "n")), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 0))), gen_lbl(gc, (uint32_t) -2), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_stkv(gc, X64_TYPE(M), 0), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(uv);
    te_f(utt);
    te_f(stt);
    te_f(fa);
    te_f(fb);
    tbl *xt = tbl_c(an->d[3].p);
    AR(3);
    mc *mv = mc_i_cstr("l", &al_mc);
    A(tbl_g_i(xt, P(mv), &kv) == TBL_STAT(OK), "inv et");
    kv = kv->d[1].p;
    for (int64_t i = 0; i <= 6; i++) {
        A(kv->d[0].i6 == i, "inv st `i");
        kv = kv->d[1].p;
        kv = kv->d[1].p;
    }
    mc_f(mv);
    opt_exp_tbl_f(xt);
}

T(validm) {
    AI(TPGM(validm), 3, 2);
    te *vv = TV(VR, TV(VR, TS(I6))), *kv;
    te *vi = TV(VR, TS(I6));
    te *ee = chk_rt_err_t(&al_te);
    te *ev = TH(UN, 2, "e", te_c(ee), "v", te_c(vi));
    te *ue = TH(UN, 2,  "e", te_c(ee), "v", TS(VD));
    te *fn = TFN(FN, te_c(ue), 1, "x", te_c(vv), 0);
    D(vv, vr_f);
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(JMP), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, ev, &e, "");
    D(ev, gc);
    gen_f(gc);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, ue, &e, "");
    D(ue, gc);
    gen_f(gc);
    te *cn = RN(LN(LT(3, "a", FLG(0, LTE_FLG(L)), te_c(vv), "b", FLG(1, LTE_FLG(L)), te_c(vv), "v", FLG(-1, LTE_FLG(F)), te_c(fn)), L(6,
        ON(te_c(vv), DFN, EN("a", FLG(0, LTE_FLG(L)), te_c(vv)), VN(te_c(vv), L(2,
            VN(te_c(vi), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))),
            VN(te_c(vi), L(3, SN(I6, I6(4)), SN(I6, I6(5)), SN(I6, I6(6))))
        ))),
        ON(te_c(vv), DFN, EN("b", FLG(1, LTE_FLG(L)), te_c(vv)), VN(te_c(vv), L(2,
            VN(te_c(vi), L(2, SN(I6, I6(1)), SN(I6, I6(2)))),
            VN(te_c(vi), L(1, SN(I6, I6(4))))
        ))),
        ON(te_c(fn), DFN, EN("v", FLG(-1, LTE_FLG(F)), te_c(fn)), SN(_G, U5(-1))),
        ON(TS(VD), UNER, NULL, AN(te_c(ue), EN("v", FLG(-1, LTE_FLG(F)), te_c(fn)),
            L(1, EN("a", FLG(0, LTE_FLG(L)), te_c(vv))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("a", FLG(0, LTE_FLG(L)), te_c(vv))),
        ON(TS(VD), UNER, NULL, AN(te_c(ue), EN("v", FLG(-1, LTE_FLG(F)), te_c(fn)),
            L(1, EN("b", FLG(1, LTE_FLG(L)), te_c(vv)))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 4), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 0), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(MM), 0), gen_data(gc, X64_TYPE(U3), U3(16))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(U6), 0), gen_tmp(gc, X64_TYPE(U6), 0), NULL));
    S(gen_a(gc, GEN_OP(NE), gen_stkv(gc, X64_TYPE(U6), 0), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 2), gen_call_m(gc, 2, gen_char(gc, "Empty"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(atg_rt_err_u(gc, 3, 4, 2, NULL, 5, 14, atg_user_inv_str));
    A(tbl_g_i(t->dt, P(ue), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 5), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 5), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 5), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 3), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    A(tbl_g_i(t->dt, P(ev), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 6), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 12), gen_data(gc, X64_TYPE(I6), I6(0)), NULL));
    S(aply_e_vr_u(gc, gen_tmp(gc, X64_TYPE(U6), 12), gen_arg(gc, X64_TYPE(MM), 0), 2, 3, 4, 6, 7, 8, 9, 10, 11, NULL, 6, 8, X64_TYPE(M), true, 0, 1));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 3), gen_tmp(gc, X64_TYPE(M), 6), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 5)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 15), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 13, 14, 15, NULL, 6, 11, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 13), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 16), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 17), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 16), gen_data(gc, X64_TYPE(U3), U3(16))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(U6), 1), gen_tmp(gc, X64_TYPE(U6), 17), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(U6), 2), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(GTE), gen_stkv(gc, X64_TYPE(U6), 2), gen_stkv(gc, X64_TYPE(U6), 0), gen_lbl(gc, 7)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 6), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 18), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(aply_e_vr_u(gc, gen_stkv(gc, X64_TYPE(U6), 2), gen_arg(gc, X64_TYPE(MM), 0), 10, 11, 12, 18, 19, 20, 21, 22, 23, NULL, 7, 31, X64_TYPE(M), true, 0, 1));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 4), gen_tmp(gc, X64_TYPE(M), 18), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 4), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 13)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 26), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 24, 25, 26, NULL, 7, 34, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 24), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 13), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 27), gen_idx_m(gc, X64_TYPE(M), 2, gen_stkv(gc, X64_TYPE(M), 4), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 28), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 27), gen_data(gc, X64_TYPE(U3), U3(16))), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_stkv(gc, X64_TYPE(U6), 1), gen_tmp(gc, X64_TYPE(U6), 28), gen_lbl(gc, 9)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 8), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 30), gen_call_m(gc, 2, gen_char(gc, "Inv"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(atg_rt_err_u(gc, 31, 32, 30, NULL, 7, 38, atg_user_inv_str));
    A(tbl_g_i(t->dt, P(ue), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 33), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 33), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 33), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 31), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 33), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 9), NULL, NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_stkv(gc, X64_TYPE(U6), 2), gen_stkv(gc, X64_TYPE(U6), 2), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_stkv(gc, X64_TYPE(U6), 2), gen_stkv(gc, X64_TYPE(U6), 0), gen_lbl(gc, 6)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 7), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_arg(gc, X64_TYPE(MM), 0), gen_arg(gc, X64_TYPE(MM), 0), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 34), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 34), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 34), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 3)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 4)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 34), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(SN(_G, U5(14)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 14), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_data(gc, X64_TYPE(M), P(NULL)), NULL));
    A(tbl_g_i(t->dt, P(vv), &kv) == TBL_STAT(OK), "inv des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 35), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 36), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(3)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 36), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 36), gen_data(gc, X64_TYPE(U3), U3(48))), gen_data(gc, X64_TYPE(I6), I6(2)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 36), gen_data(gc, X64_TYPE(U3), U3(56))), gen_data(gc, X64_TYPE(I6), I6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 36), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 36), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 36), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 35), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 36), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 37), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(3)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 37), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(4)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 37), gen_data(gc, X64_TYPE(U3), U3(48))), gen_data(gc, X64_TYPE(I6), I6(5)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 37), gen_data(gc, X64_TYPE(U3), U3(56))), gen_data(gc, X64_TYPE(I6), I6(6)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 37), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 37), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(3)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 37), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 35), gen_data(gc, X64_TYPE(U3), U3(48))), gen_tmp(gc, X64_TYPE(M), 37), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 35), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 35), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(2)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 35), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 0), gen_tmp(gc, X64_TYPE(M), 35), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 38), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 39), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 39), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 39), gen_data(gc, X64_TYPE(U3), U3(48))), gen_data(gc, X64_TYPE(I6), I6(2)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 39), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 39), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(2)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 39), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 38), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 39), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 40), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(1)), gen_data(gc, X64_TYPE(M), P(&al_vr)), gen_data(gc, X64_TYPE(M), P(NULL))), gen_data(gc, X64_TYPE(M), P(vr_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 40), gen_data(gc, X64_TYPE(U3), U3(40))), gen_data(gc, X64_TYPE(I6), I6(4)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 40), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 40), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 40), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 38), gen_data(gc, X64_TYPE(U3), U3(48))), gen_tmp(gc, X64_TYPE(M), 40), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 38), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 38), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(2)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 38), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_stkv(gc, X64_TYPE(M), 1), gen_tmp(gc, X64_TYPE(M), 38), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 41), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 0))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 41), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 16)));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 41), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 15)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 42), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 41), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 42), gen_data(gc, X64_TYPE(U3), U3(0))), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 42), gen_data(gc, X64_TYPE(U3), U3(0))), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 41)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 42), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 15), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 45), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 43, 44, 45, NULL, 10, 2, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 43), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 16), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 41)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 18)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 47), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 17), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 48), gen_idx_m(gc, X64_TYPE(M), 3, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 47)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(VR)])), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 48), gen_data(gc, X64_TYPE(U3), U3(16))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 20)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(U6), 49), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 19), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(2)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_idx_m(gc, X64_TYPE(I6), 3, gen_tmp(gc, X64_TYPE(M), 48), gen_data(gc, X64_TYPE(U3), U3(40)), gen_tmp(gc, X64_TYPE(U6), 49))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 49), gen_tmp(gc, X64_TYPE(U6), 49), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 49), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 48), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 19)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 20), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(1)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_tmp(gc, X64_TYPE(U6), 47), gen_tmp(gc, X64_TYPE(U6), 47), gen_data(gc, X64_TYPE(U6), U6(1))));
    S(gen_a(gc, GEN_OP(LT), gen_tmp(gc, X64_TYPE(U6), 47), gen_idx_m(gc, X64_TYPE(U6), 2, gen_stkv(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(16))), gen_lbl(gc, 17)));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 18), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 4, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_end)), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 50), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(MM), 1, gen_stkv(gc, X64_TYPE(M), 1))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 50), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 22)));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 50), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 21)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 51), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 50), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 51), gen_data(gc, X64_TYPE(U3), U3(0))), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 51), gen_data(gc, X64_TYPE(U3), U3(0))), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 50)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 51), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 21), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 54), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 52, 53, 54, NULL, 12, 2, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 52), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 22), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 50)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 0)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_call_m(gc, 1, gen_stkv(gc, X64_TYPE(M), 1)), gen_data(gc, X64_TYPE(M), P(vr_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(vv);
    te_f(vi);
    te_f(ee);
    te_f(ev);
    te_f(ue);
    te_f(fn);
    AE(14);
    err_p(e, true);
    A(e->m == atg_user_inv_str, "inv err str");
    A(((mc*) ((te*) e->d)->d[2].p)->d[0] == 'I', "inv err mc char");
    err_f(e);
}
