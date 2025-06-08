
#include "../src/chk.h"
#include "fld_t.h"

static tbl *cti(void) {
    lst *tl = lst_i(&ast_am, &ast_am, (void*) te_f);
    te *b = te_i(10, &ast_am, NULL);
    return tbl_i(&ast_am, tbl_no_hsh, tbl_un_eq, tl, b);
}

#define I ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, fnadd3, &an); \
    fast(_t, a, &an); \
    chk *c = chk_i(&ast_am, &ast_am, cti, a); \
    chk_b(c);

#define V(N) te *e = NULL; \
    chk_stat stat = chk_n(c, an, &e); \
    chk_f(c); \
    if (e) { \
        ast_p(e, 0); \
        putchar('\n'); \
    } \
    A(stat == CHK_STAT(OK), "chk_n"); \
    ast_verify(_t, a, an, N);

T(fnadd3) {
    I;
    printf("BEFORE\n");
    chk_p(c->bt, 0);
    printf("AFTER\n");
    chk_p(c->at, 0);
    printf("------\n");
    te *ft = TF(FN, TS(I6), 3, "a", TS(I6), "b", TS(I6), "c", TS(I6));
    te *cn = RN(AN(TS(VD), LN(LT(1, "f", te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "a", TS(I6), "b", TS(I6), "c", TS(I6)), L(1,
                    ON(TS(I6), SUB, NULL, ON(TS(I6), ADD, EN("a", TS(I6)),
                        ON(TS(I6), ADD, EN("b", TS(I6)), EN("c", TS(I6))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )), NULL));
    V(cn);
    te_f(ft);
}
