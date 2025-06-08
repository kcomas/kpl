
#include "ast_t.h"

#define V(PGM, AST) ast *a = ast_i_ast(bast); \
    te *an = NULL; \
    astb(_t, a, PGM, &an); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(aplyopadd, RN(AN(NULL, ON(NULL, ADD, NULL, NULL), L(2, SN(I6, I6(1)), SN(I6, I6(2))))));
}

T(typetype) {
    V(typetype, RN(AN(NULL, TN(FN), L(3, ZN("x", TN(I6)), ZN("y", TN(F6)), AN(NULL, TN(FN), L(2, ZN("z", TN(U6)), TN(I6)))))));
}

T(fnadd3) {
    V(fnadd3, RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("f"), ON(NULL, CST,
            AN(NULL, TN(FN), L(4, ZN("a", TN(I6)), ZN("b", TN(I6)), ZN("c", TN(I6)), TN(I6))),
            LN(NULL, L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, IN("a"), ON(NULL, ADD, IN("b"), IN("c"))))))
            )),
        CN(P1, AN(NULL, IN("f"), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))
    )))), NULL)));
}

T(fnf6muli6cstdiv) {
    V(fnf6muli6cstdiv, RN(AN(NULL, LN(NULL, L(2,
            ON(NULL, DFN, IN("f"), ON(NULL, CST,
                AN(NULL, TN(FN), L(4, ZN("x", TN(F6)), ZN("y", TN(F6)), ZN("z", TN(U6)), TN(F6))),
                LN(NULL, L(1, AN(NULL, ON(NULL, DIV, NULL, NULL), L(2, ON(NULL, MUL, IN("x"), IN("y")), ON(NULL, CST, TN(F6), IN("z"))))))
                )),
            CN(P1, AN(NULL, IN("f"), L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(NULL, CST, TN(U6), SN(I6, I6(2))))
        )))), NULL)));
}

T(apltypefn) {
    V(apltypefn, RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("f"),
        ON(NULL, CST, TN(FN), LN(NULL, L(1, ON(NULL, ADD, AN(NULL, TN(I6), L(1, IN("a"))), ON(NULL, MUL, AN(NULL, TN(I6), L(1, IN("b"))), IN("a"))))))),
        CN(P1, AN(NULL, IN("f"), L(2, SN(I6, I6(3)), SN(I6, I6(4))))))), NULL)));
}
