
#include "../src/atg_x64.h"
#include "chk_t.h"
#include "gen_t.h"

extern const alfr am;

T(fnadd3) {
    IC(fnadd3);
    RC();
    fast(_t, a, &an, opt_b, false);
    atg *t = atg_i(&ast_am, &ast_am, &ast_am, &ast_am, cti, lst_i(&am, &am, NULL), ali(), gen_b(gen_i(&am, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst())), as_b(as_i(&am, &am, &am, as_arg_tbl, as_op_tbl(AS_X64(_END)), as_mklst())));
    atg_b(t);
    atg_tbl_p(t->ot, AST_CLS(O), 0);
    A(atg_q(t, &an, atg_x64_enq) == ATG_STAT(OK) && t->q->l == 2, "atg_q");
    gen *g;
    te *e = NULL;
    atg_stat stat = atg_qn(t, &g, a, &e);
    if (e) {
        ast_p(e, 0);
        putchar('\n');
    }
    A(stat == ATG_STAT(OK), "atg_qn");
    ast_p(an, 0);
    putchar('\n');
    gen_p(g, NULL);
    te *ft = TF(FN, TS(I6), 3, "a", TS(I6), "b", TS(I6), "c", TS(I6));
    te *cn = RN(LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)), SN(_G, P(NULL))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )));
    A(ast_eq(an, cn), "ast_eq");
    te_f(ft);
    te_f(cn);
    cn = NULL;
    // TODO test gen
    te_f(an);
    atg_f(t);
    ast_f(a);
}
