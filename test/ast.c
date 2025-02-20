
#include "ast_t.h"

T(ast_aplyopadd) {
    V(aplyopadd, RN(AN(P(NULL), ON(P(NULL), ADD, NULL, NULL), L(2, SN(I6, I6(1)), SN(I6, I6(2))))));
}

T(ast_typetype) {
    V(typetype, RN(AN(P(NULL), TN(FN), L(3, ZN("x", TN(I6)), ZN("y", TN(F6)), AN(P(NULL), TN(FN), L(2, ZN("z", TN(U6)), TN(I6)))))));
}

T(ast_fnadd3) {
    V(fnadd3, RN(LN(L(2,
        ON(P(NULL), AGN, IN("f"), ON(P(NULL), CST,
            AN(P(NULL), TN(FN), L(4, ZN("a", TN(I6)), ZN("b", TN(I6)), ZN("c", TN(I6)), TN(I6))),
            LN(L(1, ON(P(NULL), SUB, NULL, ON(P(NULL), ADD, IN("a"), ON(P(NULL), ADD, IN("b"), IN("c"))))))
            )),
        AN(P(NULL), IN("f"), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))
    )))));
}
