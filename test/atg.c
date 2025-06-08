
#include "../src/atg_x64.h"
#include "chk_t.h"
#include "gen_t.h"

extern const alfr am;

T(fnadd3) {
    IC(fnadd3);
    RC();
    fast(_t, a, &an, opt_b, false);
    atg *t = atg_b(atg_i(&ast_am, &ast_am, cti, lst_i(&am, &am, NULL), ali(), gen_b(gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst())), as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst()))));
    gen_st *st = gen_st_i(&am, &am, gen_op_tbl(20), gen_op_tbl(20), vr_i(16, &am, NULL), vr_i(16, &am, NULL));
    atg_tbl_p(t->ot, AST_CLS(O), 0);
    A(atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->q->l == 2, "atg_q");
    gen *g;
    te *ae = NULL, *ce = NULL;
    atg_stat astat = atg_qn(t, &g, a, &ae);
    if (ae) {
        ast_p(ae, 0);
        putchar('\n');
    }
    A(astat == ATG_STAT(OK), "atg_qn");
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
    gen *gc = gen_b(gen_i(&am, &am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst()));
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
    gen_stat gstat = gen_n(g, sc, t->a, &ce);
    if (ce) {
        printf("CODE ERROR %p\n", ce);
    }
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    sc = NULL;
    gen_f(g);
    g = gc = NULL;
    astat = atg_qn(t, &g, a, &ae);
    if (ae) {
        ast_p(ae, 0);
        putchar('\n');
    }
    A(astat == ATG_STAT(OK), "atg_qn");
    ast_p(an, 0);
    putchar('\n');
    gen_p(g, NULL);
    A(ast_eq(an, cn), "ast_eq");
    gen_st_f(st);
    atg_f(t);
    ast_f(a);
    te_f(an);
}
