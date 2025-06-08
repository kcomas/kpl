
#include "fld_t.h"

#define V(PGM, AST) ast *a = ast_i_ast(bast); \
    te *an = NULL; \
    astb(_t, a, PGM, &an); \
    fast(_t, a, &an, bfld, true); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(aplyopadd, RN(ON(NULL, ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(typetype) {
    V(typetype, RN(NN(T, TF(FN, TF(FN, TS(I6), 1, "z", TS(U6), 0), 2, "x", TS(I6), 0, "y", TS(F6), 0))));
}

T(fnadd3) {
    V(fnadd3, RN(AN(NULL, LN(LT(1, "f", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("f", U6(0), NULL),
            ON(NULL, CST,
            NN(T, TF(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2)),
            LN(LT(3, "a", U6(0), NULL, "b", U6(0), NULL,  "c", U6(0), NULL), L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, EN("a", U6(0), NULL), ON(NULL, ADD, EN("b", U6(0), NULL), EN("c", U6(0), NULL))))))
        )),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f", U6(0), NULL), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))))), NULL)));
}

T(fnf6muli6cstdiv) {
    V(fnf6muli6cstdiv, RN(AN(NULL,
        LN(LT(1, "f", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("f", U6(0), NULL), ON(NULL, CST, NN(T, TF(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0)), LN(LT(3, "x", U6(0), NULL, "y", U6(0), NULL, "z", U6(0), NULL), L(1, ON(NULL, DIV,
            ON(NULL, MUL, EN("x", U6(0), NULL), EN("y", U6(0), NULL)),
            ON(NULL, CST, TN(F6), EN("z", U6(0), NULL))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f", U6(0), NULL), L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(NULL, CST, TN(U6), SN(I6, I6(2)))))))),
    NULL)));
}
