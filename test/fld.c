
#include "fld_t.h"

#define V(PGM, AST) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, a, &an); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(aplyopadd, RN(ON(NULL, ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(typetype) {
    V(typetype, RN(NN(T, TF(FN, TF(FN, TS(I6), 1, "z", TS(U6)), 2, "x", TS(I6), "y", TS(F6)))));
}

T(fnadd3) {
    V(fnadd3, RN(AN(NULL, LN(LT(1, "f"), L(2,
        ON(NULL, AGN, EN("f"),
            ON(NULL, CST,
            NN(T, TF(FN, TS(I6), 3, "a", TS(I6), "b", TS(I6), "c", TS(I6))),
            LN(LT(3, "a", "b", "c"), L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, EN("a"), ON(NULL, ADD, EN("b"), EN("c"))))))
        )),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f"), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))))), NULL)));
}
