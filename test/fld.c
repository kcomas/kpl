
#include "fld_t.h"

#define V(PGM, AST) ast *a = ast_i_ast(bast); \
    te *an = NULL; \
    astb(_t, a, PGM, &an); \
    fast(_t, a, &an, bfld, true); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(TPGM(aplyopadd), RN(ON(NULL, ADD, SN(I6, I6(1)), SN(I6, I6(2)))));
}

T(typetype) {
    V(TPGM(typetype), RN(NN(T, TFN(FN, TFN(FN, TS(I6), 1, "z", TS(U6), 0), 2, "x", TS(I6), 0, "y", TS(F6), 0))));
}

T(fnadd3) {
    V(TPGM(fnadd3), RN(AN(NULL, LN(LT(1, "f", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("f", U6(0), NULL),
            ON(NULL, CST,
            NN(T, TFN(FN, TS(I6), 3, "a", TS(I6), 0, "b", TS(I6), 1, "c", TS(I6), 2)),
            LN(LT(3, "a", U6(0), NULL, "b", U6(0), NULL,  "c", U6(0), NULL), L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, EN("a", U6(0), NULL), ON(NULL, ADD, EN("b", U6(0), NULL), EN("c", U6(0), NULL))))))
        )),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f", U6(0), NULL), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))))), NULL)));
}

T(fnf6muli6cstdiv) {
    V(TPGM(fnf6muli6cstdiv), RN(AN(NULL,
        LN(LT(1, "f", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("f", U6(0), NULL), ON(NULL, CST, NN(T, TFN(FN, TS(F6), 3, "x", TS(F6), 0, "y", TS(F6), 1, "z", TS(U6), 0)), LN(LT(3, "x", U6(0), NULL, "y", U6(0), NULL, "z", U6(0), NULL), L(1, ON(NULL, DIV,
            ON(NULL, MUL, EN("x", U6(0), NULL), EN("y", U6(0), NULL)),
            ON(NULL, CST, TN(F6), EN("z", U6(0), NULL))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f", U6(0), NULL), L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(NULL, CST, TN(U6), SN(I6, I6(2)))))))),
    NULL)));
}

T(apltypefn) {
    V(TPGM(apltypefn), RN(AN(NULL, LN(LT(1, "f", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("f", U6(0), NULL), ON(NULL, CST, NN(T, TF(FN, NULL)), LN(LT(2, "a", U6(0), TS(I6), "b", U6(0), TS(I6)), L(1, ON(NULL, ADD, EN("a", U6(0), TS(I6)), ON(NULL, MUL, EN("b", U6(0), TS(I6)), EN("a", U6(0), TS(I6)))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("f", U6(0), NULL), L(2, SN(I6, I6(3)), SN(I6, I6(4)))))
    )), NULL)));
}

T(facloop) {
    V(TPGM(facloop), RN(AN(NULL, LN(LT(2, "v", U6(0), NULL, "f", LTE_FLG(E), NULL), L(4,
        ON(NULL, DFN, EN("v", U6(0), NULL), SN(I6, I6(5))),
        ON(NULL, DFN, EN("f", U6(LTE_FLG(E)), NULL), SN(I6, I6(1))),
        ON(NULL, LOOP,
            LN(NULL, L(1, ON(NULL, GT, EN("v", U6(0), NULL), SN(I6, I6(1))))),
            LN(LT(2, "f", U6(0), NULL, "v", U6(0), NULL), L(2,
                ON(NULL, MULA, EN("f", U6(0), NULL), EN("v", U6(0), NULL)),
                ON(NULL, SUBA, EN("v", U6(0), NULL), SN(I6, I6(1)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("f", U6(LTE_FLG(E)), NULL))
    )), NULL)));
}

T(facrec) {
    V(TPGM(facrec), RN(AN(NULL, LN(LT(1, "fac", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("fac", U6(0), NULL), ON(NULL, CST, NN(T, TFN(FN, TS(U6), 1, "n", TS(U6), 0)),
            LN(LT(2, "n", U6(0), NULL, "fac", U6(0), NULL), L(1, ON(NULL, IF,
                LN(NULL, L(1, ON(NULL, GT, EN("n", U6(0), NULL), SN(I6, I6(1))))),
                LN(LT(2, "n", U6(0), NULL, "fac", U6(0), NULL), L(2,
                    ON(NULL, MUL, EN("n", U6(0), NULL), AN(NULL, EN("fac", U6(0), NULL), L(1, ON(NULL, SUB, EN("n", U6(0), NULL), SN(I6, I6(1)))))),
                    ON(NULL, CST, TN(U6), SN(I6, I6(1)))))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("fac", U6(0), NULL), L(1, ON(NULL, CST, TN(U6), SN(I6, I6(5)))))))),
    NULL)));
}

T(fibrec) {
    V(TPGM(fibrec), RN(AN(NULL, LN(LT(1, "fib", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("fib", U6(0), NULL), ON(NULL, CST, NN(T, TFN(FN, TS(I6), 1, "n", TS(I6), 0)),
            LN(LT(2, "n", U6(0), NULL, "fib", U6(0), NULL), L(1,
                ON(NULL, IF, LN(NULL, NULL), LN(NULL, L(3,
                    ON(NULL, IF, LN(NULL, L(1, ON(NULL, LTE, EN("n", U6(0), NULL), SN(I6, I6(0))))), LN(NULL, L(1, SN(I6, I6(0))))),
                    ON(NULL, IF, LN(NULL, L(1, ON(NULL, LT, EN("n", U6(0), NULL), SN(I6, I6(3))))), LN(NULL, L(1, SN(I6, I6(1))))),
                    ON(NULL, ADD, AN(NULL, EN("fib", U6(0), NULL), L(1, ON(NULL, SUB, EN("n", U6(0), NULL), SN(I6, I6(1))))), AN(NULL, EN("fib", U6(0), NULL), L(1, ON(NULL, SUB, EN("n", U6(0), NULL), SN(I6, I6(2)))))))))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("fib", U6(0), NULL), L(1, SN(I6, I6(8)))))
    )), NULL)));
}

T(ackrec) {
    V(TPGM(ackrec), RN(AN(NULL, LN(LT(1, "ack", U6(0), NULL), L(2,
        ON(NULL, DFN, EN("ack", U6(0), NULL), ON(NULL, CST, NN(T, TFN(FN, TS(U6), 2, "m", TS(U6), 0, "n", TS(U6), 1)), LN(LT(3, "m", U6(0), NULL, "n", U6(0), NULL, "ack", U6(0), NULL), L(1,
            ON(NULL, IF, LN(NULL, NULL), LN(NULL, L(4,
                ON(NULL, IF, LN(NULL, L(1, ON(NULL, EQ, EN("m", U6(0), NULL), SN(I6, I6(0))))), LN(NULL, L(1, ON(NULL, ADD, EN("n", U6(0), NULL), SN(I6, I6(1)))))),
                ON(NULL, IF, LN(NULL, L(1, ON(NULL, AND, ON(NULL, GT, EN("m", U6(0), NULL), SN(I6, I6(0))), ON(NULL, EQ, EN("n", U6(0), NULL), SN(I6, I6(0)))))), LN(NULL, L(1, AN(NULL, EN("ack", U6(0), NULL), L(2, ON(NULL, SUB, EN("m", U6(0), NULL), SN(I6, I6(1))), ON(NULL, CST, TN(U6), SN(I6, I6(1)))))))),
                ON(NULL, IF, LN(NULL, L(1, ON(NULL, AND, ON(NULL, GT, EN("m", U6(0), NULL), SN(I6, I6(0))), ON(NULL, GT, EN("n", U6(0), NULL), SN(I6, I6(0)))))), LN(NULL, L(1, AN(NULL, EN("ack", U6(0), NULL), L(2, ON(NULL, SUB, EN("m", U6(0), NULL), SN(I6, I6(1))), AN(NULL, EN("ack", U6(0), NULL), L(2, EN("m", U6(0), NULL), ON(NULL, SUB, EN("n", U6(0), NULL), SN(I6, I6(1)))))))))),
                ON(NULL, ADD, EN("n", U6(0), NULL), SN(I6, I6(1)))
            )))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(NULL, EN("ack", U6(0), NULL), L(2, ON(NULL, CST, TN(U6), SN(I6, I6(2))), ON(NULL, CST, TN(U6), SN(I6, I6(1))))))
    )), NULL)));
}

T(scope) {
    V(TPGM(scope), RN(AN(NULL, LN(LT(2, "s", FLG(0, LTE_FLG(E)), NULL, "x", U6(0), NULL), L(6,
        ON(NULL, DFN, EN("s", FLG(0, LTE_FLG(E)), NULL), SN(I6, I6(0))),
        ON(NULL, DFN, EN("x", U6(0), NULL), ON(NULL, CST,
            NN(T, TFN(NF, TS(VD), 1, "x", TS(I6), 0)),
            LN(LT(2, "s", U6(0), NULL, "x", U6(0), NULL), L(1, ON(NULL, ADDA, EN("s", U6(0), NULL), EN("x", U6(0), NULL))))
        )),
        AN(NULL, EN("x", U6(0), NULL), L(1, SN(I6, I6(1)))),
        AN(NULL, EN("x", U6(0), NULL), L(1, SN(I6, I6(2)))),
        AN(NULL, EN("x", U6(0), NULL), L(1, SN(I6, I6(3)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(0, LTE_FLG(E)), NULL))
    )), NULL)));
}

T(efn) {
    V(TPGM(efn), RN(AN(NULL, LN(LT(1, "addmul", FLG(0, LTE_FLG(E)), NULL), L(1,
        ON(NULL, DFN, EN("addmul", FLG(0, LTE_FLG(E)), NULL), ON(NULL, CST, TN(FN),
            LN(LT(3, "z", U6(0), TS(F6), "y", U6(0), TS(F6), "x", U6(0), TS(F6)), L(1,
                ON(NULL, MUL, EN("z", U6(0), TS(F6)),
                    ON(NULL, ADD, EN("y", U6(0), TS(F6)), EN("x", U6(0), TS(F6))))
            ))))
    )), NULL)));
}

T(teadd) {
    V(TPGM(teadd), RN(AN(NULL, LN(LT(1, "a", U6(0), NULL), L(3,
        ON(NULL, DFN, EN("a", U6(0), NULL), VN(NULL, L(3, SN(I6, I6(1)), SN(I6, I6(0)), SN(I6, I6(3))))),
        ON(NULL, AGN, AN(NULL, EN("a", U6(0), NULL), L(1, SN(I6, I6(1)))),
            ON(NULL, ADD, AN(NULL, EN("a", U6(0), NULL), L(1, SN(I6, I6(0)))), AN(NULL, EN("a", U6(0), NULL), L(1, SN(I6, I6(2)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("a", U6(0), NULL))
    )), NULL)));
}

T(lfile) {
    mc *f = mc_i_cstr("./ex/sum.kpl", &al_mc);
    tbl *et = NULL;
    err *e = z(f, &et, 0);
    A(!e, "lfile error");
    mc_f(f);
    A(et->i->l == 1, "inv tbl len");
    f = mc_i_cstr("b", &al_mc);
    te *kv;
    A(tbl_g_i(et, P(f), &kv) == TBL_STAT(OK), "export not found");
    mc_f(f);
    A(kv->d[2].p && ((te*) kv->d[2].p)->d[1].u4 == TYPE(I6), "inv export type");
    A(kv->d[1].i6 == 5050, "inv export value");
    tbl_f(et);
}

T(vrmul) {
    V(TPGM(vrmul), RN(AN(NULL, LN(LT(2, "v", U6(0), NULL, "i", U6(0), NULL), L(3,
        ON(NULL, DFN, EN("v", U6(0), NULL), ON(NULL, CST, TN(VR),
            VN(NULL, L(1, ON(NULL, DFN, EN("i", U6(0), NULL), SN(F6, F6(1.1))))))),
        ON(NULL, LOOP,
            LN(NULL, L(1, ON(NULL, LT, EN("i", U6(0), NULL), SN(I6, I6(100))))),
            LN(NULL, L(1, ON(NULL, CNCTA, EN("v", U6(0), NULL), ON(NULL, MULA, EN("i", U6(0), NULL), SN(I6, I6(2))))))
        ),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("v", U6(0), NULL))
    )), NULL)));
}

T(st) {
    V(TPGM(st), RN(AN(NULL, LN(LT(3, "a", U6(0), NULL, "b", U6(0), NULL, "s", FLG(0, LTE_FLG(E)), NULL), L(5,
        ON(NULL, DFN, EN("a", U6(0), NULL), SN(I6, I6(1))),
        ON(NULL, DFN, EN("b", U6(0), NULL), SN(F6, F6(2.2))),
        ON(NULL, DFN, EN("s", FLG(0, LTE_FLG(E)), NULL),
            ON(NULL, MTCH, LN(NULL, NULL), LN(LT(2, "a", U6(0), NULL, "b", U6(0), NULL), L(4,
                ZN("a", EN("a", U6(0), NULL)),
                ZN("b", EN("b", U6(0), NULL)),
                ZN("c", ON(NULL, CST, TN(U6), ON(NULL, ADD, EN("a", U6(0), NULL), SN(I6, I6(2))))),
                ZN("d", ON(TS(SG), CSG, NULL, CS("Hi")))
            )))),
        ON(NULL, AGN, ZN("a", EN("s", FLG(0, LTE_FLG(E)), NULL)),
            ON(NULL, MUL, SN(I6, I6(2)), ON(NULL, CST, TN(I6), ZN("c", EN("s", FLG(0, LTE_FLG(E)), NULL))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(0, LTE_FLG(E)), NULL))
    )), NULL)));
}

T(import) {
    V(TPGM(import), RN(AN(NULL, LN(LT(2, "i", U6(0), NULL, "v", FLG(0, LTE_FLG(E)), NULL), L(4,
        ON(NULL, DFN, EN("i", U6(0), NULL), SET(1, "b", P(NULL), TS(I6))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("i", U6(0), NULL)),
        ON(NULL, DFN, EN("v", FLG(0, LTE_FLG(E)), NULL),
            ON(NULL, ADD, SN(I6, I6(1)), ON(NULL, MUL, ZN("b", EN("i", U6(0), NULL)), SN(I6, I6(2))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("v", FLG(0, LTE_FLG(E)), NULL))
    )), NULL)));
}

T(lfac) {
    V(TPGM(lfac), RN(AN(NULL, LN(LT(2, "l", U6(0), NULL, "e", FLG(0, LTE_FLG(E)), NULL), L(3,
        ON(NULL, DFN, EN("l", U6(0), NULL), SET(1, "fac", P(NULL), TFN(FN, TS(I6), 1, "n", TS(I6), 0))),
        ON(NULL, DFN, EN("e", FLG(0, LTE_FLG(E)), NULL), AN(NULL, ZN("fac", EN("l", U6(0), NULL)), L(1, SN(I6, I6(6))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("e", FLG(0, LTE_FLG(E)), NULL))
    )), NULL)));
}

T(istr) {
    V(TPGM(istr), RN(AN(NULL, LN(LT(3, "a", U6(0), NULL, "b", U6(0), NULL, "c", U6(0), NULL), L(4,
        ON(NULL, DFN, EN("a", U6(0), NULL), SN(I6, I6(987))),
        ON(NULL, DFN, EN("b", U6(0), NULL), SN(F6, F6(3.14))),
        ON(NULL, DFN, EN("c", U6(0), NULL), AN(NULL, CS("a: {}, b: {}"), L(2, EN("a", U6(0), NULL), EN("b", U6(0), NULL)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("c", U6(0), NULL))
    )), NULL)));
}

T(fnscope) {
    V(TPGM(fnscope), RN(AN(NULL, LN(LT(3, "a", U6(0), NULL, "b", U6(0), NULL, "c", U6(0), NULL), L(4,
        ON(NULL, DFN, EN("a", U6(0), NULL), ON(NULL, CST, TN(FN),
            LN(LT(2, "b", U6(0), NULL, "n", U6(0), TS(F6)),
                L(1, AN(NULL, EN("b", U6(0), NULL), L(1, EN("n", U6(0), TS(F6)))))))),
        ON(NULL, DFN, EN("b", U6(0), NULL), ON(NULL, CST, TN(FN),
            LN(LT(3, "v", U6(0), NULL, "f", U6(0), NULL, "n", U6(0), TS(F6)), L(3,
                ON(NULL, DFN, EN("v", U6(0), NULL), SN(F6, F6(4.3))),
                ON(NULL, DFN, EN("f", U6(0), NULL), ON(NULL, CST, TN(NF),
                    LN(LT(2, "n", U6(0), TS(F6), "v", U6(0), NULL),
                        L(1, ON(NULL, SUB, EN("n", U6(0), TS(F6)), EN("v", U6(0), NULL)))))),
                AN(NULL, EN("f", U6(0), NULL), L(1, EN("n", U6(0), TS(F6)))))))),
        ON(NULL, DFN, EN("c", U6(0), NULL), ON(NULL, CST, TN(FN),
            LN(LT(2, "a", U6(0), NULL, "n", U6(0), TS(F6)), L(1,
                AN(NULL, EN("a", U6(0), NULL), L(1, EN("n", U6(0), TS(F6)))))))),
        AN(NULL, EN("c", U6(0), NULL), L(1, SN(F6, F6(9.6))))
    )), NULL)));
}
