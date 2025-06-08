
#include "opt_t.h"

#define V(N) RC(); \
    fast(_t, a, &an, bopt, false); \
    ast_verify(_t, a, an, N);

T(fnadd3) {
    IC(TPGM(fnadd3));
    te *ft = TFN(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6), "c", FLG(2, LTE_FLG(A)), TS(I6)), L(1,
                    ON(TS(I6), SUB, NULL, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)),
                        ON(TS(I6), ADD, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("c", FLG(2, LTE_FLG(A)), TS(I6))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))
    )));
    V(cn);
    te_f(ft);
}

T(fnf6muli6cstdiv) {
    IC(TPGM(fnf6muli6cstdiv));
    te *ft = TFN(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            ON(te_c(ft), CST, NN(T, te_c(ft)),
                LN(LT(3, "x", FLG(0, LTE_FLG(A)), TS(F6), "y", FLG(1, LTE_FLG(A)), TS(F6), "z", FLG(0, LTE_FLG(A)), TS(U6)), L(1,
                    ON(TS(F6), DIV, ON(TS(F6), MUL, EN("x", FLG(0, LTE_FLG(A)), TS(F6)), EN("y", FLG(1, LTE_FLG(A)), TS(F6))), ON(TS(F6), CST, TN(F6), EN("z", FLG(0, LTE_FLG(A)), TS(U6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
            L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), SN(U6, U6(2)))))
    )));
    V(cn);
    te_f(ft);
}

T(apltypefn) {
    IC(TPGM(apltypefn));
    te *ft = TFN(FN, TS(I6), 2, "a", TS(I6), 0, "b", TS(I6), 1);
    V(RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(2, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6)), L(1, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)), ON(TS(I6), MUL, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("a", FLG(0, LTE_FLG(A)), TS(I6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    ))));
    te_f(ft);
}

T(facloop) {
    IC(TPGM(facloop));
    te *rn = te_c(an);
    V(RN(LN(LT(2, "v", FLG(0, LTE_FLG(L)), TS(I6), "f", FLG(1, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), L(4,
        ON(TS(I6), DFN, EN("v", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(5))),
        ON(TS(I6), DFN, EN("f", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), SN(I6, I6(1))),
        ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), GT, EN("v", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(1))))),
            LN(LT(2, "f", FLG(1, LTE_FLG(O) | LTE_FLG(E) | LTE_FLG(L)), TS(I6), "v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), L(2,
                ON(TS(I6), MULA, EN("f", FLG(1, LTE_FLG(O) | LTE_FLG(L) | LTE_FLG(E)), TS(I6)), EN("v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6))),
                ON(TS(I6), SUBA, EN("v", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("f", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)))
    ))));
    tbl *ept = rn->d[3].p;
    A(ept, "exports");
    A(ept->i->l == 1, "exports len");
    te *kv;
    mc *f = mc_i_cstr("f", &ast_am);
    A(tbl_g_i(ept, P(f), &kv) == TBL_STAT(OK), "exp not found");
    A(((te*) kv->d[2].p)->d[1].u6 == TYPE(I6), "inv exp type");
    uint32_t id = opt_exp_id(kv);
    uint16_t eid = opt_exp_eid(kv), flgs = opt_exp_flgs(kv);
    A(id == 1, "inv lid");
    A(eid == 0, "inv exp idx");
    A(flgs & LTE_FLG(L), "inv flg");
    mc_f(f);
    te_f(rn);
}

T(facrec) {
    IC(TPGM(facrec));
    te *ft = TFN(FN, TS(U6), 1, "n", TS(U6), 0);
    tbl *lta = LT(2, "n", FLG(0, LTE_FLG(A)), TS(U6), "fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    tbl *ltb = LT(2, "n", FLG(0, LTE_FLG(A) | LTE_FLG(O)), TS(U6), "fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    te *ne = EN("n", FLG(0, LTE_FLG(O) | LTE_FLG(A)), TS(U6));
    te *face = EN("fac", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    V(RN(LN(LT(1, "fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)),
            LN(tbl_c(lta), L(1, ON(TS(U6), IF,
                LN(NULL, L(1, ON(TS(BL), GT, EN("n", FLG(0, LTE_FLG(A)), TS(U6)), SN(U6, U6(1))))),
                LN(tbl_c(ltb), L(2,
                    ON(TS(U6), MUL, te_c(ne), AN(TS(U6), te_c(face), L(1, ON(TS(U6), SUB, te_c(ne), SN(U6, U6(1)))))),
                    SN(U6, U6(1))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("fac", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, SN(U6, U6(5)))))))));
    te_f(ft);
    tbl_f(lta);
    tbl_f(ltb);
    te_f(ne);
    te_f(face);
}

T(fibrec) {
    IC(TPGM(fibrec));
    te *ft = TFN(FN, TS(I6), 1, "n", TS(I6), 0);
    te *en = EN("n", FLG(0, LTE_FLG(A)), TS(I6));
    te *efib = EN("fib", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    V(RN(LN(LT(1, "fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, TFN(FN, TS(I6), 1, "n", TS(I6), 0)),
            LN(LT(2, "n", FLG(0, LTE_FLG(A)), TS(I6), "fib", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft)), L(1,
                ON(TS(I6), IF, LN(NULL, NULL), LN(NULL, L(3,
                    ON(TS(I6), IF, LN(NULL, L(1, ON(TS(BL), LTE, te_c(en), SN(I6, I6(0))))), LN(NULL, L(1, SN(I6, I6(0))))),
                    ON(TS(I6), IF, LN(NULL, L(1, ON(TS(BL), LT, te_c(en), SN(I6, I6(3))))), LN(NULL, L(1, SN(I6, I6(1))))),
                    ON(TS(I6), ADD, AN(TS(I6), te_c(efib), L(1, ON(TS(I6), SUB, te_c(en), SN(I6, I6(1))))), AN(TS(I6), te_c(efib), L(1, ON(TS(I6), SUB, te_c(en), SN(I6, I6(2)))))))))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, SN(I6, I6(8)))))
    ))));
    te_f(ft);
    te_f(en);
    te_f(efib);
}
