
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
    te *ft = TFN(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2);
    te *cn = RN(AN(TS(VD), LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6), "c", FLG(2, LTE_FLG(A)), TS(I6)), L(1,
                    ON(TS(I6), SUB, NULL, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)),
                        ON(TS(I6), ADD, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("c", FLG(2, LTE_FLG(A)), TS(I6))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )), NULL));
    V(cn);
    te_f(ft);
}

T(fnf6muli6cstdiv) {
    IC(fnf6muli6cstdiv);
    te *ft = TFN(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0);
    te *cn = RN(AN(TS(VD), LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "x", FLG(0, LTE_FLG(A)), TS(F6), "y", FLG(1, LTE_FLG(A)), TS(F6), "z", FLG(0, LTE_FLG(A)), TS(U6)), L(1,
                    ON(TS(F6), DIV, ON(TS(F6), MUL, EN("x", FLG(0, LTE_FLG(A)), TS(F6)), EN("y", FLG(1, LTE_FLG(A)), TS(F6))), ON(TS(F6), CST, TN(F6), EN("z", FLG(0, LTE_FLG(A)), TS(U6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(TS(U6), CST, TN(U6), SN(I6, I6(2))))))
    )), NULL));
    V(cn);
    te_f(ft);
}

T(apltypefn) {
    IC(apltypefn);
    te *ft = TFN(FN, TS(I6), 2, "a", TS(I6), 0, "b", TS(I6), 1);
    V(RN(AN(TS(VD), LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(2, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6)), L(1, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)), ON(TS(I6), MUL, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("a", FLG(0, LTE_FLG(A)), TS(I6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    )), NULL)));
    te_f(ft);
}

T(facloop) {
    IC(facloop);
    V(RN(AN(TS(VD), LN(LT(2, "v", FLG(0, LTE_FLG(L)), TS(I6), "f", FLG(1, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), L(4,
        ON(TS(I6), DFN, EN("v", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(5))),
        ON(TS(I6), DFN, EN("f", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), SN(I6, I6(1))),
        ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), GT, EN("v", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(1))))),
            LN(LT(2, "f", FLG(0, LTE_FLG(O) | LTE_FLG(L) | LTE_FLG(E)), TS(I6), "v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), L(2,
                ON(TS(I6), MULA, EN("f", FLG(0, LTE_FLG(O) | LTE_FLG(L) | LTE_FLG(E)), TS(I6)), EN("v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6))),
                ON(TS(I6), SUBA, EN("v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("f", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)))
    )), NULL)));
}

T(facrec) {
    IC(facrec);
    te *ft = TFN(FN, TS(U6), 1, "n", TS(U6), 0);
    tbl *lta = LT(2, "n", FLG(0, LTE_FLG(A)), TS(U6), "fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    tbl *ltb = LT(2, "n", FLG(0, LTE_FLG(A) | LTE_FLG(O)), TS(U6), "fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    te *ne = EN("n", FLG(0, LTE_FLG(O) | LTE_FLG(A)), TS(U6));
    te *face = EN("fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    V(RN(AN(TS(VD), LN(LT(1, "fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)),
            LN(tbl_c(lta), L(1, ON(TS(U6), IF,
                LN(NULL, L(1, ON(TS(BL), GT, EN("n", FLG(0, LTE_FLG(A)), TS(U6)), SN(U6, U6(1))))),
                LN(tbl_c(ltb), L(2,
                    ON(TS(U6), MUL, te_c(ne), AN(TS(U6), te_c(face), L(1, ON(TS(U6), SUB, te_c(ne), SN(U6, U6(1)))))),
                    ON(TS(U6), CST, TN(U6), SN(I6, I6(1)))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, ON(TS(U6), CST, TN(U6), SN(I6, I6(5)))))))),
    NULL)));
    te_f(ft);
    tbl_f(lta);
    tbl_f(ltb);
    te_f(ne);
    te_f(face);
}
