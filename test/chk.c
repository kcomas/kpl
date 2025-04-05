
#include "chk_t.h"

#define V(N) RC(); \
    ast_verify(_t, a, an, N);

T(fnadd3) {
    IC(fnadd3);
    printf("BEFORE\n");
    chk_p(c->bt, 0);
    printf("AFTER\n");
    chk_p(c->at, 0);
    printf("------\n");
    te *ft = TF(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2);
    te *cn = RN(AN(TS(VD), LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6), "c", FLG(2, LTE_FLG(A)), TS(I6)), L(1,
                    ON(TS(I6), SUB, NULL, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)),
                        ON(TS(I6), ADD, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("c", FLG(2, LTE_FLG(A)), TS(I6))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )), NULL));
    V(cn);
    te_f(ft);
}

T(fnf6muli6cstdiv) {
    IC(fnf6muli6cstdiv);
    te *ft = TF(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0);
    te *cn = RN(AN(TS(VD), LN(LT(1, "f", FLG(0, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "x", FLG(0, LTE_FLG(A)), TS(F6), "y", FLG(1, LTE_FLG(A)), TS(F6), "z", FLG(0, LTE_FLG(A)), TS(U6)), L(1,
                    ON(TS(F6), DIV, ON(TS(F6), MUL, EN("x", FLG(0, LTE_FLG(A)), TS(F6)), EN("y", FLG(1, LTE_FLG(A)), TS(F6))), ON(TS(F6), CST, TN(F6), EN("z", FLG(0, LTE_FLG(A)), TS(U6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("f", FLG(0, LTE_FLG(F)), te_c(ft)),
            L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(TS(U6), CST, TN(U6), SN(I6, I6(2))))))
    )), NULL));
    V(cn);
    te_f(ft);
}
