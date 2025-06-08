
#include "chk_t.h"

#define V(N) RC(); \
    ast_verify(_t, a, an, N);

T(fnadd3) {
    IC(TPGM(fnadd3));
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
    IC(TPGM(fnf6muli6cstdiv));
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
    IC(TPGM(apltypefn));
    te *ft = TFN(FN, TS(I6), 2, "a", TS(I6), 0, "b", TS(I6), 1);
    V(RN(AN(TS(VD), LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(2, "a", FLG(0, LTE_FLG(A)), TS(I6), "b", FLG(1, LTE_FLG(A)), TS(I6)), L(1, ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(A)), TS(I6)), ON(TS(I6), MUL, EN("b", FLG(1, LTE_FLG(A)), TS(I6)), EN("a", FLG(0, LTE_FLG(A)), TS(I6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    )), NULL)));
    te_f(ft);
}

T(facloop) {
    IC(TPGM(facloop));
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
    IC(TPGM(facrec));
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

T(fibrec) {
    IC(TPGM(fibrec));
    te *ft = TFN(FN, TS(I6), 1, "n", TS(I6), 0);
    te *en = EN("n", FLG(0, LTE_FLG(A)), TS(I6));
    te *efib = EN("fib", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft));
    V(RN(AN(TS(VD), LN(LT(1, "fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, TFN(FN, TS(I6), 1, "n", TS(I6), 0)),
            LN(LT(2, "n", FLG(0, LTE_FLG(A)), TS(I6), "fib", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft)), L(1,
                ON(TS(I6), IF, LN(NULL, NULL), LN(NULL, L(3,
                    ON(TS(I6), IF, LN(NULL, L(1, ON(TS(BL), LTE, te_c(en), SN(I6, I6(0))))), LN(NULL, L(1, SN(I6, I6(0))))),
                    ON(TS(I6), IF, LN(NULL, L(1, ON(TS(BL), LT, te_c(en), SN(I6, I6(3))))), LN(NULL, L(1, SN(I6, I6(1))))),
                    ON(TS(I6), ADD, AN(TS(I6), te_c(efib), L(1, ON(TS(I6), SUB, te_c(en), SN(I6, I6(1))))), AN(TS(I6), te_c(efib), L(1, ON(TS(I6), SUB, te_c(en), SN(I6, I6(2)))))))))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("fib", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, SN(I6, I6(8)))))
    )), NULL)));
    te_f(ft);
    te_f(en);
    te_f(efib);
}

T(ackrec) {
    IC(TPGM(ackrec));
    te *ft = TFN(FN, TS(U6), 2, "m", TS(U6), 0, "n", TS(U6), 1);
    te *em = EN("m", FLG(0, LTE_FLG(A)), TS(U6));
    te *en = EN("n", FLG(1, LTE_FLG(A)), TS(U6));
    te *eack = EN("ack", FLG(-1, LTE_FLG(O) | LTE_FLG(F)), te_c(ft));
    V(RN(AN(TS(VD), LN(LT(1, "ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(3, "m", FLG(0, LTE_FLG(A)), TS(U6), "n", FLG(1, LTE_FLG(A)), TS(U6), "ack", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft)), L(1,
            ON(TS(U6), IF, LN(NULL, NULL), LN(NULL, L(4,
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), EQ, te_c(em), SN(U6, U6(0))))),
                    LN(NULL, L(1, ON(TS(U6), ADD, te_c(en), SN(U6, U6(1)))))),
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), AND, ON(TS(BL), GT, te_c(em), SN(U6, U6(0))), ON(TS(BL), EQ, te_c(en), SN(U6, U6(0)))))),
                    LN(NULL, L(1, AN(TS(U6), te_c(eack), L(2, ON(TS(U6), SUB, te_c(em), SN(U6, U6(1))), ON(TS(U6), CST, TN(U6), SN(I6, I6(1)))))))),
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), AND, ON(TS(BL), GT, te_c(em), SN(U6, U6(0))), ON(TS(BL), GT, te_c(en), SN(U6, U6(0)))))),
                    LN(NULL, L(1, AN(TS(U6), te_c(eack), L(2, ON(TS(U6), SUB, te_c(em), SN(U6, U6(1))), AN(TS(U6), te_c(eack), L(2, te_c(em), ON(TS(U6), SUB, te_c(en), SN(U6, U6(1)))))))))),
                ON(TS(U6), ADD, te_c(en), SN(U6, U6(1)))
            )))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, ON(TS(U6), CST, TN(U6), SN(I6, I6(2))), ON(TS(U6), CST, TN(U6), SN(I6, I6(1))))))
    )), NULL)));
    te_f(ft);
    te_f(em);
    te_f(en);
    te_f(eack);
}

T(scope) {
    IC(TPGM(scope));
    tbl *args = fld_type_tbl_i(true, 1, "x", TS(I6), 0);
    tbl *scope = fld_type_tbl_i(true, 1, "s", TS(I6), 0);
    te *ft = TFS(NF, TS(VD), args, scope);
    te *fe = EN("x", FLG(-1, LTE_FLG(F)), te_c(ft));
    V(RN(AN(TS(VD), LN(LT(2, "s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6), "x", FLG(-1, LTE_FLG(F)), te_c(ft)), L(6,
        ON(TS(I6), DFN, EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
        ON(te_c(ft), DFN, te_c(fe), ON(te_c(ft), CST,
            NN(T, te_c(ft)),
            LN(LT(2, "s", FLG(0, LTE_FLG(S)), TS(I6), "x", FLG(0, LTE_FLG(A)), TS(I6)), L(1, ON(TS(I6), ADDA, EN("s", FLG(0, LTE_FLG(S)), TS(I6)), EN("x", FLG(0, LTE_FLG(A)), TS(I6)))))
        )),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(1)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(2)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(3)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)))
    )), NULL)));
    te_f(ft);
    te_f(fe);
}

T(efn) {
    IC(TPGM(efn));
    te *tfn = TFN(FN, TS(F6), 3, "z", TS(F6), 0, "y", TS(F6), 1, "x", TS(F6), 2);
    V(RN(AN(TS(VD), LN(LT(1, "addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), L(1,
        ON(te_c(tfn), DFN, EN("addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), ON(te_c(tfn), CST, NN(T, te_c(tfn)),
            LN(LT(3, "z", FLG(0, LTE_FLG(A)), TS(F6), "y", FLG(1, LTE_FLG(A)), TS(F6), "x", FLG(2, LTE_FLG(A)), TS(F6)), L(1,
                ON(TS(F6), MUL, EN("z", FLG(0, LTE_FLG(A)), TS(F6)),
                    ON(TS(F6), ADD, EN("y", FLG(1, LTE_FLG(A)), TS(F6)), EN("x", FLG(2, LTE_FLG(A)), TS(F6))))
            ))))
    )), NULL)));
    te_f(tfn);
}

T(teadd) {
    IC(TPGM(teadd));
    te *tte = type_te_i(&al_te, NULL, 3);
    te *ta = type_i(&al_te, tte, TYPE(I6));
    tte->d[2] = P(te_c(ta));
    te *tb = type_i(&al_te, tte, TYPE(I6));
    tte->d[3] = P(te_c(tb));
    te *tc = type_i(&al_te, tte, TYPE(I6));
    tte->d[4] = P(te_c(tc));
    te *ae = EN("a", FLG(0, LTE_FLG(L)), te_c(tte));
    V(RN(AN(TS(VD), LN(LT(1, "a", FLG(0, LTE_FLG(L)), te_c(tte)), L(3,
        ON(te_c(tte), DFN, te_c(ae), VN(te_c(tte), L(3, SN(I6, I6(1)), SN(I6, I6(0)), SN(I6, I6(3))))),
        ON(TS(I6), AGN, AN(TS(I6), te_c(ae), L(1, SN(I6, I6(1)))),
            ON(TS(I6), ADD, AN(TS(I6), te_c(ae), L(1, SN(I6, I6(0)))), AN(TS(I6), te_c(ae), L(1, SN(I6, I6(2)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(ae))
    )), NULL)));
    te_f(tte);
    te_f(ta);
    te_f(tb);
    te_f(tc);
    te_f(ae);
}

T(vrmul) {
    IC(TPGM(vrmul));
    te *vr = type_v_i(&al_te, NULL, TYPE(VR), type_s_i(&al_te, NULL, TYPE(F6)));
    te *ve = EN("v", FLG(0, LTE_FLG(L)), te_c(vr));
    te *ie = EN("i", FLG(0, LTE_FLG(L)), TS(F6));
    V(RN(AN(TS(VD), LN(LT(2, "v", FLG(0, LTE_FLG(L)), te_c(vr), "i", FLG(0, LTE_FLG(L)), TS(F6)), L(3,
        ON(te_c(vr), DFN, te_c(ve), ON(te_c(vr), CST, NN(T, te_c(vr)),
            VN(type_te_i_v(&al_te, NULL, 1, TS(F6)), L(1, ON(TS(F6), DFN, te_c(ie), SN(F6, F6(1.1))))))),
        ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), LT, te_c(ie), SN(F6, F6(100))))),
            LN(NULL, L(1, ON(te_c(vr), CNCTA, te_c(ve), ON(TS(F6), MULA, te_c(ie), SN(F6, F6(2))))))
        ),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(ve))
    )), NULL)));
    te_f(vr);
    te_f(ve);
    te_f(ie);
}

T(st) {
    IC(TPGM(st));
    te *st = TST(4, "a", TS(I6), "b", TS(F6), "c", TS(U6), "d", TS(SG));
    V(RN(AN(TS(VD), LN(LT(3, "a", FLG(0, LTE_FLG(L)), TS(I6), "b", FLG(0, LTE_FLG(L)), TS(F6), "s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st)), L(5,
        ON(TS(I6), DFN, EN("a", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(1))),
        ON(TS(F6), DFN, EN("b", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(2.2))),
        ON(te_c(st), DFN, EN("s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st)),
            ON(te_c(st), MTCH, LN(NULL, NULL), LN(LT(2, "a", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6), "b", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(F6)), L(4,
                ZTN("a", TS(VD), EN("a", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6))),
                ZTN("b", TS(VD), EN("b", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(F6))),
                ZTN("c", TS(VD), ON(TS(U6), CST, TN(U6), ON(TS(I6), ADD, EN("a", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(2))))),
                ZTN("d", TS(VD), ON(TS(SG), CSG, NULL, CS("Hi")))
            )))),
        ON(TS(I6), AGN, ZTN("a", TS(I6), EN("s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st))),
            ON(TS(I6), MUL, SN(I6, I6(2)), ON(TS(I6), CST, TN(I6), ZTN("c", TS(U6), EN("s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st)))
    )), NULL)));
    te_f(st);
}
