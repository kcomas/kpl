
#include "ast_t.h"

#define V(PGM, AST) ast *a = ast_i_ast(bast); \
    te *an = NULL; \
    astb(_t, a, PGM, &an); \
    ast_verify(_t, a, an, AST)

T(aplyopadd) {
    V(TPGM(aplyopadd), RN(AN(NULL, ON(NULL, ADD, NULL, NULL), L(2, SN(I6, I6(1)), SN(I6, I6(2))))));
}

T(typetype) {
    V(TPGM(typetype), RN(AN(NULL, TN(FN), L(3, ZN("x", TN(I6)), ZN("y", TN(F6)), AN(NULL, TN(FN), L(2, ZN("z", TN(U6)), TN(I6)))))));
}

T(fnadd3) {
    V(TPGM(fnadd3), RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("f"), ON(NULL, CST,
            AN(NULL, TN(FN), L(4, ZN("a", TN(I6)), ZN("b", TN(I6)), ZN("c", TN(I6)), TN(I6))),
            LN(NULL, L(1, ON(NULL, SUB, NULL, ON(NULL, ADD, IN("a"), ON(NULL, ADD, IN("b"), IN("c"))))))
            )),
        CN(P1, AN(NULL, IN("f"), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))
    )))), NULL)));
}

T(fnf6muli6cstdiv) {
    V(TPGM(fnf6muli6cstdiv), RN(AN(NULL, LN(NULL, L(2,
            ON(NULL, DFN, IN("f"), ON(NULL, CST,
                AN(NULL, TN(FN), L(4, ZN("x", TN(F6)), ZN("y", TN(F6)), ZN("z", TN(U6)), TN(F6))),
                LN(NULL, L(1, AN(NULL, ON(NULL, DIV, NULL, NULL), L(2, ON(NULL, MUL, IN("x"), IN("y")), ON(NULL, CST, TN(F6), IN("z"))))))
                )),
            CN(P1, AN(NULL, IN("f"), L(3, SN(F6, F6(4.4)), SN(F6, F6(6.6)), ON(NULL, CST, TN(U6), SN(I6, I6(2))))
        )))), NULL)));
}

T(apltypefn) {
    V(TPGM(apltypefn), RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("f"),
        ON(NULL, CST, TN(FN), LN(NULL, L(1, ON(NULL, ADD, AN(NULL, TN(I6), L(1, IN("a"))), ON(NULL, MUL, AN(NULL, TN(I6), L(1, IN("b"))), IN("a"))))))),
        CN(P1, AN(NULL, IN("f"), L(2, SN(I6, I6(3)), SN(I6, I6(4))))))), NULL)));
}

T(facloop) {
    V(TPGM(facloop), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("v"), SN(I6, I6(5))),
        ON(NULL, DFN, CN(E, IN("f")), SN(I6, I6(1))),
        ON(NULL, LOOP,
            AN(NULL, ON(NULL, GT, NULL, NULL), L(2, IN("v"), SN(I6, I6(1)))),
            LN(NULL, L(2,
                ON(NULL, MULA, IN("f"), IN("v")),
                ON(NULL, SUBA, IN("v"), SN(I6, I6(1)))
            ))),
        CN(P1, IN("f"))
    )), NULL)));
}

T(facrec) {
    V(TPGM(facrec), RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("fac"), ON(NULL, CST, AN(NULL, TN(FN), L(2, ZN("n", TN(U6)), TN(U6))), LN(NULL, L(1,
            ON(NULL, IF,
                LN(NULL, L(1, ON(NULL, GT, IN("n"), SN(I6, I6(1))))),
                LN(NULL, L(2,
                    ON(NULL, MUL, IN("n"), AN(NULL, IN("fac"), L(1, ON(NULL, SUB, IN("n"), SN(I6, I6(1)))))),
                    ON(NULL, CST, TN(U6), SN(I6, I6(1)))
                ))
        ))))),
        CN(P1, AN(NULL, IN("fac"), L(1, ON(NULL, CST, TN(U6), SN(I6, I6(5))))))
    )), NULL)));
}

T(fibrec) {
    V(TPGM(fibrec), RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("fib"), ON(NULL, CST, AN(NULL, TN(FN), L(2, ZN("n", TN(I6)), TN(I6))), LN(NULL, L(1, ON(NULL, IF, NULL,
            LN(NULL, L(3,
                ON(NULL, IF, AN(NULL, ON(NULL, LTE, NULL, NULL), L(2, IN("n"), SN(I6, I6(0)))), SN(I6, I6(0))),
                ON(NULL, IF, AN(NULL, ON(NULL, LT, NULL, NULL), L(2, IN("n"), SN(I6, I6(3)))), SN(I6, I6(1))),
                ON(NULL, ADD,
                    AN(NULL, IN("fib"), L(1, ON(NULL, SUB, IN("n"), SN(I6, I6(1))))),
                    AN(NULL, IN("fib"), L(1, ON(NULL, SUB, IN("n"), SN(I6, I6(2))))))))))))),
        CN(P1, AN(NULL, IN("fib"), L(1, SN(I6, I6(8)))))
    )), NULL)));
}

T(ackrec) {
    V(TPGM(ackrec), RN(AN(NULL, LN(NULL, L(2,
        ON(NULL, DFN, IN("ack"), ON(NULL, CST, AN(NULL, TN(FN), L(3, ZN("m", TN(U6)), ZN("n", TN(U6)), TN(U6))), LN(NULL, L(1,
            ON(NULL, IF, NULL, LN(NULL, L(4,
                ON(NULL, IF, AN(NULL, ON(NULL, EQ, NULL, NULL), L(2, IN("m"), SN(I6, I6(0)))), ON(NULL, ADD, IN("n"), SN(I6, I6(1)))),
                ON(NULL, IF, AN(NULL, ON(NULL, AND, NULL, NULL), L(2, ON(NULL, GT, IN("m"), SN(I6, I6(0))), ON(NULL, EQ, IN("n"), SN(I6, I6(0))))), AN(NULL, IN("ack"), L(2, ON(NULL, SUB, IN("m"), SN(I6, I6(1))), ON(NULL, CST, TN(U6), SN(I6, I6(1)))))),
                ON(NULL, IF, AN(NULL, ON(NULL, AND, NULL, NULL), L(2, ON(NULL, GT, IN("m"), SN(I6, I6(0))), ON(NULL, GT, IN("n"), SN(I6, I6(0))))), AN(NULL, IN("ack"), L(2, ON(NULL, SUB, IN("m"), SN(I6, I6(1))), AN(NULL, IN("ack"), L(2, IN("m"), ON(NULL, SUB, IN("n"), SN(I6, I6(1)))))))),
                ON(NULL, ADD, IN("n"), SN(I6, I6(1)))
            )))
        )))),
        CN(P1, AN(NULL, IN("ack"), L(2, ON(NULL, CST, TN(U6), SN(I6, I6(2))), ON(NULL, CST, TN(U6), SN(I6, I6(1))))))
    )), NULL)));
}

T(scope) {
    V(TPGM(scope), RN(AN(NULL, LN(NULL, L(6,
        ON(NULL, DFN, CN(E, IN("s")), SN(I6, I6(0))),
        ON(NULL, DFN, IN("x"), ON(NULL, CST,
            AN(NULL, TN(NF), L(2, ZN("x", TN(I6)), TN(VD))),
            LN(NULL, L(1, ON(NULL, ADDA, IN("s"), IN("x"))))
        )),
        AN(NULL, IN("x"), L(1, SN(I6, I6(1)))),
        AN(NULL, IN("x"), L(1, SN(I6, I6(2)))),
        AN(NULL, IN("x"), L(1, SN(I6, I6(3)))),
        CN(P1, IN("s"))
    )), NULL)));
}

T(efn) {
    V(TPGM(efn), RN(AN(NULL, LN(NULL, L(1,
        ON(NULL, DFN, CN(E, IN("addmul")), ON(NULL, CST, TN(FN), LN(NULL, L(1,
            ON(NULL, MUL, AN(NULL, TN(F6), L(1, IN("z"))),
                ON(NULL, ADD, AN(NULL, TN(F6), L(1, IN("y"))), AN(NULL, TN(F6), L(1, IN("x")))))
        ))))
    )), NULL)));
}

T(teadd) {
    V(TPGM(teadd), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("a"), VN(NULL, L(3, SN(I6, I6(1)), SN(I6, I6(0)), SN(I6, I6(3))))),
        ON(NULL, AGN, AN(NULL, IN("a"), L(1, SN(I6, I6(1)))),
            ON(NULL, ADD, AN(NULL, IN("a"), L(1, SN(I6, I6(0)))), AN(NULL, IN("a"), L(1, SN(I6, I6(2)))))
        ),
        CN(P1, IN("a"))
    )), NULL)));
}

T(vrmul) {
    V(TPGM(vrmul), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("v"), ON(NULL, CST, TN(VR), VN(NULL, L(1, ON(NULL, DFN, IN("i"), SN(F6, F6(1.1))))))),
        ON(NULL, LOOP,
            AN(NULL, ON(NULL, LT, NULL, NULL), L(2, IN("i"), SN(I6, I6(100)))),
            ON(NULL, CNCTA, IN("v"), ON(NULL, MULA, IN("i"), SN(I6, I6(2))))
        ),
        CN(P1, IN("v"))
    )), NULL)));
}

T(st) {
    V(TPGM(st), RN(AN(NULL, LN(NULL, L(5,
        ON(NULL, DFN, IN("a"), SN(I6, I6(1))),
        ON(NULL, DFN, IN("b"), SN(F6, F6(2.2))),
        ON(NULL, DFN, CN(E, IN("s")), ON(NULL, MTCH, NULL, LN(NULL, L(4,
            ZN("a", IN("a")),
            ZN("b", IN("b")),
            ZN("c", AN(NULL, ON(NULL, CST, NULL, NULL), L(2, TN(U6), ON(NULL, ADD, IN("a"), SN(I6, I6(2)))))),
            ZN("d", CS("Hi"))
        )))),
        ON(NULL, AGN, ZN("a", IN("s")), ON(NULL, MUL, SN(I6, I6(2)), ON(NULL, CST, TN(I6), ZN("c", IN("s"))))),
        CN(P1, IN("s"))
    )), NULL)));
}

T(import) {
    V(TPGM(import), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("i"), CN(L, CS("./ex/sum.kpl"))),
        CN(P1, IN("i")),
        ON(NULL, DFN, CN(E, IN("v")), ON(NULL, ADD, SN(I6, I6(1)), ON(NULL, MUL, ZN("b", IN("i")), SN(I6, I6(2))))),
        CN(P1, IN("v"))
    )), NULL)));
}

T(lfac) {
    V(TPGM(lfac), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("l"), CN(L, CS("./ex/fac.kpl"))),
        ON(NULL, DFN, CN(E, IN("e")), AN(NULL, ZN("fac", IN("l")), L(1, SN(I6, I6(6))))),
        CN(P1, IN("e"))
    )), NULL)));
}

T(istr) {
    V(TPGM(istr), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("a"), SN(I6, I6(987))),
        ON(NULL, DFN, IN("b"), SN(F6, F6(3.14))),
        ON(NULL, DFN, IN("c"), AN(NULL, CS("a: {}, b: {}"), L(2, IN("a"), IN("b")))),
        CN(P1, IN("c"))
    )), NULL)));
}

T(fnscope) {
    V(TPGM(fnscope), RN(AN(NULL, LN(NULL, L(5,
        ON(NULL, DFN, IN("b"), AN(NULL, TN(FN), L(2, ZN("n", TN(F6)), TN(F6)))),
        ON(NULL, DFN, IN("a"), ON(NULL, CST, TN(FN), LN(NULL,
            L(1, AN(NULL, IN("b"), L(1, AN(NULL, TN(F6), L(1, IN("n"))))))))),
        ON(NULL, DFN, IN("b"), ON(NULL, CST, TN(FN), LN(NULL, L(3,
            ON(NULL, DFN, IN("v"), SN(F6, F6(4.3))),
            ON(NULL, DFN, IN("f"), ON(NULL, CST, TN(NF), LN(NULL, L(1,
                ON(NULL, SUB, AN(NULL, TN(F6), L(1, IN("n"))), IN("v")))))),
            AN(NULL, IN("f"), L(1, AN(NULL, TN(F6), L(1, IN("n")))))
        )))),
        ON(NULL, DFN, IN("c"), ON(NULL, CST, TN(FN), LN(NULL,
            L(1, AN(NULL, IN("a"), L(1, AN(NULL, TN(F6), L(1, IN("n"))))))))),
        CN(P1, AN(NULL, IN("c"), L(1, SN(F6, F6(9.6)))))
    )), NULL)));
}

T(or) {
    V(TPGM(or), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("a"), SN(I6, I6(0))),
        ON(NULL, DFN, IN("b"), SN(I6, I6(1))),
        ON(NULL, IF,
            AN(NULL, ON(NULL, OR, NULL, NULL), L(2, IN("a"), IN("b"))),
            CN(P1, AN(NULL, ON(NULL, ADD, NULL, NULL), L(2, IN("a"), IN("b")))))
    )), NULL)));
}

T(un) {
    V(TPGM(un), RN(AN(NULL, LN(NULL, L(5,
        ON(NULL, DFN, IN("u"), ZN("a", SN(I6, I6(1)))),
        ON(NULL, AGN, IN("u"), ZN("b", SN(F6, F6(2.2)))),
        CN(P1, IN("u")),
        CN(P1, ZN("b", IN("u"))),
        CN(P1, ZN("a", IN("u")))
    )), NULL)));
}

T(unmatch) {
    V(TPGM(unmatch), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("u"), ZN("a", SN(I6, I6(1)))),
        ON(NULL, AGN, IN("u"), ZN("b", SN(I6, I6(2)))),
        ON(NULL, DFN, CN(E, IN("c")), ON(NULL, MTCH, IN("u"), LN(NULL, L(3,
            ZN("a", AN(NULL, CS("a {}"), L(1, IN("a")))),
            ZN("b", AN(NULL, CS("b {}"), L(1, IN("b")))),
            CS("inv")
        )))),
        CN(P1, IN("c"))
    )), NULL)));
}

T(ns) {
    V(TPGM(ns), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("s"), CS("./ex/sum.kpl")),
        ON(NULL, DFN, IN("f"), ON(NULL, NS, NULL,
            ON(NULL, NS, IN("f"), AN(NULL, IN("rs"), L(1, IN("s")))))),
        CN(P1, ZN("v", IN("f")))
    )), NULL)));
}

T(iline) {
    V(TPGM(iline), RN(AN(NULL, LN(NULL, L(1,
        CN(P1, ZN("v",
            AN(NULL, ON(NULL, NS, NULL, NULL),
                L(2, IN("f"), ON(NULL, NS, NULL,
                    AN(NULL, IN("rs"), L(1, CS("./ex/sum.kpl"))))))))
    )), NULL)));
}

T(unsg) {
    V(TPGM(unsg), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("a"), ZN("v", SN(I6, I6(1)))),
        ON(NULL, AGN, IN("a"), ZN("s", CS("a"))),
        CN(P1, IN("a"))
    )), NULL)));
}

T(vridx) {
    V(TPGM(vridx), RN(AN(NULL, LN(NULL, L(5,
        ON(NULL, DFN, CN(E, IN("v")),
            ON(NULL, CST, TN(VR),
                VN(NULL, L(2, VN(NULL, L(2, CS("a"), CS("b"))), VN(NULL, L(2, CS("c"), CS("d"))))))),
        CN(P1, IN("v")),
        CN(P1, AN(NULL, IN("v"), L(1, ON(NULL, SUB, NULL, SN(I6, I6(1)))))),
        CN(P1, AN(NULL, ZN("v", AN(NULL, IN("v"), L(1, ON(NULL, SUB, NULL, SN(I6, I6(1)))))), L(1, SN(I6, I6(0))))),
        ON(NULL, MTCH, AN(NULL, IN("v"), L(1, SN(I6, I6(3)))), LN(NULL, L(1, ZN("e", CN(P1, IN("e"))))))
    )), NULL)));
}

T(nscope) {
    V(TPGM(nscope), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("x"), SN(F6, F6(6.6))),
        ON(NULL, DFN, IN("fa"), ON(NULL, CST, TN(NF), LN(NULL, L(2,
            ON(NULL, DFN, IN("fb"), ON(NULL, CST, TN(NF), LN(NULL, L(1,
                ON(NULL, DIVA, IN("x"), SN(F6, F6(2.0)))
            )))),
            AN(NULL, IN("fb"), NULL)
        )))),
        AN(NULL, IN("fa"), NULL),
        CN(P1, IN("x"))
    )), NULL)));
}

T(cj) {
    V(TPGM(cj), RN(AN(NULL, LN(NULL, L(5,
        ON(NULL, DFN, IN("mc"), ON(NULL, CST, TN(FN), LN(NULL, L(2,
            ON(NULL, DFN, IN("c"), SN(I6, I6(0))),
            ON(NULL, CST, TN(CJ), LN(NULL, L(1, ON(NULL, ADDA, IN("c"), SN(I6, I6(1))))))
        )))),
        ON(NULL, DFN, IN("cc"), AN(NULL, IN("mc"), NULL)),
        CN(P1, AN(NULL, IN("cc"), NULL)),
        CN(P1, AN(NULL, IN("cc"), NULL)),
        ON(NULL, DFN, CN(E, IN("e")), AN(NULL, IN("cc"), NULL))
    )), NULL)));
}

T(leap) {
    V(TPGM(leap), RN(AN(NULL, LN(NULL, L(4,
        ON(NULL, DFN, IN("y"), SN(I6, I6(2000))),
        ON(NULL, DFN, CN(E, IN("v")), ON(NULL, CST, AN(NULL, TN(VR), L(1, CN(T, IN("y")))), VN(NULL, LE()))),
        ON(NULL, LOOP, AN(NULL, ON(NULL, LTE, NULL, NULL), L(2, IN("y"), SN(I6, I6(2030)))), LN(NULL, L(2,
            ON(NULL, IF,
                    AN(NULL, ON(NULL, OR, NULL, NULL), L(2,
                    AN(NULL, ON(NULL, AND, NULL, NULL), L(2,
                            ON(NULL, EQ, SN(I6, I6(0)), ON(NULL, MOD, IN("y"), SN(I6, I6(4)))),
                            ON(NULL, NE, SN(I6, I6(0)), ON(NULL, MOD, IN("y"), SN(I6, I6(100)))))),
                        ON(NULL, EQ, SN(I6, I6(0)), ON(NULL, MOD, IN("y"), SN(I6, I6(400)))))),
                        ON(NULL, CNCTA, IN("v"), IN("y"))),
            ON(NULL, ADDA, IN("y"), SN(I6, I6(1)))))),
       CN(P1, IN("v"))
    )), NULL)));
}

T(vrup) {
    V(TPGM(vrup), RN(AN(NULL, LN(NULL, L(3,
        ON(NULL, DFN, IN("v"), ON(NULL, CST, TN(VR), VN(NULL, L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3)))))),
        ON(NULL, DFN, IN("f"), ON(NULL, CST, TN(FN), LN(NULL, L(4,
            ON(NULL, AGN,
                AN(NULL, AN(NULL, AN(NULL, TN(VR), L(1, TN(I6))), L(1, IN("a"))), L(1, SN(I6, I6(1)))),
                SN(I6, I6(5))),
            CN(P1, IN("a")),
            ON(NULL, AGN, AN(NULL, IN("a"), L(1, SN(I6, I6(5)))), SN(I6, I6(1))),
            ZN("v", SN(I6, I6(1)))
        )))),
        ON(NULL, UNER, NULL, AN(NULL, IN("f"), L(1, IN("v"))))
    )), NULL)));
}

T(stll) {
    V(TPGM(stll), RN(AN(NULL, LN(NULL, L(6,
        ON(NULL, DFN, CN(E, IN("l")),
            ON(NULL, MTCH, NULL, LN(NULL, L(2,
                ZN("i", SN(I6, I6(0))),
                ZN("n", ZN("y", TN(VD))))))),
        ON(NULL, AGN, ZN("z", ZN("n", IN("l"))), CN(T, IN("l"))),
        ON(NULL, DFN, IN("a"), ON(NULL, CST, TN(NF), LN(NULL, L(2,
            ON(NULL, DFN, IN("b"),
                ON(NULL, CST, TN(FN), LN(NULL, L(1,
                    ON(NULL, MTCH, ZN("n", AN(NULL, CN(T, IN("l")), L(1, IN("x")))), LN(NULL, L(2,
                        ZN("z", AN(NULL, IN("b"), L(1, IN("z")))),
                        ZN("y", IN("x")))))
                )))),
            ON(NULL, AGN, ZN("n", AN(NULL, IN("b"), L(1, IN("l")))),
                ON(NULL, CST, CN(T, IN("l")), ON(NULL, MTCH, NULL, LN(NULL, L(2,
                    ZN("i", AN(NULL, TN(I6), L(1, IN("i")))),
                    ZN("n", ZN("y", TN(VD)))
        ))))))))),
        ON(NULL, DFN, IN("i"), SN(I6, I6(0))),
        ON(NULL, LOOP,
            AN(NULL, ON(NULL, LTE, NULL, NULL), L(2, IN("i"), SN(I6, I6(5)))),
            AN(NULL, IN("a"), L(1, ON(NULL, ADDA, IN("i"), SN(I6, I6(1)))))),
        CN(P1, IN("a"))
    )), NULL)));
}
