
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
    mc *f = mc_i_cstr("f", &al_mc);
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

T(ackrec) {
    IC(TPGM(ackrec));
    te *ft = TFN(FN, TS(U6), 2, "m", TS(U6), 0, "n", TS(U6), 1);
    te *em = EN("m", FLG(0, LTE_FLG(A)), TS(U6));
    te *en = EN("n", FLG(1, LTE_FLG(A)), TS(U6));
    te *eack = EN("ack", FLG(-1, LTE_FLG(O) | LTE_FLG(F)), te_c(ft));
    V(RN(LN(LT(1, "ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2,
        ON(te_c(ft), DFN, EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(3, "m", FLG(0, LTE_FLG(A)), TS(U6), "n", FLG(1, LTE_FLG(A)), TS(U6), "ack", FLG(-1, LTE_FLG(F) | LTE_FLG(O)), te_c(ft)), L(1,
            ON(TS(U6), IF, LN(NULL, NULL), LN(NULL, L(4,
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), EQ, te_c(em), SN(U6, U6(0))))),
                    LN(NULL, L(1, ON(TS(U6), ADD, te_c(en), SN(U6, U6(1)))))),
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), AND, LN(NULL, L(1, ON(TS(BL), GT, te_c(em), SN(U6, U6(0))))), LN(NULL, L(1, ON(TS(BL), EQ, te_c(en), SN(U6, U6(0)))))))),
                    LN(NULL, L(1, AN(TS(U6), te_c(eack), L(2, ON(TS(U6), SUB, te_c(em), SN(U6, U6(1))), SN(U6, U6(1))))))),
                ON(TS(U6), IF,
                    LN(NULL, L(1, ON(TS(BL), AND, LN(NULL, L(1, ON(TS(BL), GT, te_c(em), SN(U6, U6(0))))), LN(NULL, L(1, ON(TS(BL), GT, te_c(en), SN(U6, U6(0)))))))),
                    LN(NULL, L(1, AN(TS(U6), te_c(eack), L(2, ON(TS(U6), SUB, te_c(em), SN(U6, U6(1))), AN(TS(U6), te_c(eack), L(2, te_c(em), ON(TS(U6), SUB, te_c(en), SN(U6, U6(1)))))))))),
                ON(TS(U6), ADD, te_c(en), SN(U6, U6(1)))
            )))
        )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(U6), EN("ack", FLG(-1, LTE_FLG(F)), te_c(ft)), L(2, SN(U6, U6(2)), SN(U6, U6(1)))))
    ))));
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
    V(RN(LN(LT(2, "s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6), "x", FLG(-1, LTE_FLG(F)), te_c(ft)), L(6,
        ON(TS(I6), DFN, EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
        ON(te_c(ft), DFN, te_c(fe), ON(te_c(ft), CST,
            NN(T, te_c(ft)),
            LN(LT(2, "s", FLG(0, LTE_FLG(S)), TS(I6), "x", FLG(0, LTE_FLG(A)), TS(I6)), L(1, ON(TS(I6), ADDA, EN("s", FLG(0, LTE_FLG(S)), TS(I6)), EN("x", FLG(0, LTE_FLG(A)), TS(I6)))))
        )),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(1)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(2)))),
        AN(TS(VD), te_c(fe), L(1, SN(I6, I6(3)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("s", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)))
    ))));
    te_f(ft);
    te_f(fe);
}

T(efn) {
    IC(TPGM(efn));
    te *tfn = TFN(FN, TS(F6), 3, "z", TS(F6), 0, "y", TS(F6), 1, "x", TS(F6), 2);
    V(RN(LN(LT(1, "addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), L(1,
        ON(te_c(tfn), DFN, EN("addmul", FLG(-1, LTE_FLG(E) | LTE_FLG(F)), te_c(tfn)), ON(te_c(tfn), CST, NN(T, te_c(tfn)),
            LN(LT(3, "z", FLG(0, LTE_FLG(A)), TS(F6), "y", FLG(1, LTE_FLG(A)), TS(F6), "x", FLG(2, LTE_FLG(A)), TS(F6)), L(1,
                ON(TS(F6), MUL, EN("z", FLG(0, LTE_FLG(A)), TS(F6)),
                    ON(TS(F6), ADD, EN("y", FLG(1, LTE_FLG(A)), TS(F6)), EN("x", FLG(2, LTE_FLG(A)), TS(F6))))
            ))))
    ))));
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
    V(RN(LN(LT(1, "a", FLG(0, LTE_FLG(L)), te_c(tte)), L(3,
        ON(te_c(tte), DFN, te_c(ae), VN(te_c(tte), L(3, SN(I6, I6(1)), SN(I6, I6(0)), SN(I6, I6(3))))),
        ON(TS(I6), AGN, AN(TS(I6), te_c(ae), L(1, SN(I6, I6(1)))),
            ON(TS(I6), ADD, AN(TS(I6), te_c(ae), L(1, SN(I6, I6(0)))), AN(TS(I6), te_c(ae), L(1, SN(I6, I6(2)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(ae))
    ))));
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
    V(RN(LN(LT(2, "v", FLG(0, LTE_FLG(L)), te_c(vr), "i", FLG(0, LTE_FLG(L)), TS(F6)), L(3,
        ON(te_c(vr), DFN, te_c(ve), VN(te_c(vr), L(1, ON(TS(F6), DFN, te_c(ie), SN(F6, F6(1.1)))))),
        ON(TS(VD), LOOP,
            LN(NULL, L(1, ON(TS(BL), LT, te_c(ie), SN(F6, F6(100))))),
            LN(NULL, L(1, ON(te_c(vr), CNCTA, te_c(ve), ON(TS(F6), MULA, te_c(ie), SN(F6, F6(2))))))
        ),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(ve))
    ))));
    te_f(vr);
    te_f(ve);
    te_f(ie);
}

T(st) {
    IC(TPGM(st));
    te *st = TH(ST, 4, "a", TS(I6), "b", TS(F6), "c", TS(U6), "d", TS(SG));
    V(RN(LN(LT(3, "a", FLG(0, LTE_FLG(L)), TS(I6), "b", FLG(0, LTE_FLG(L)), TS(F6), "s", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(st)), L(5,
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
    ))));
    te_f(st);
}

T(import) {
    IC(TPGM(import));
    te *et = TH(ET, 1, "b", TS(I6));
    te *etn = EN("i", FLG(0, LTE_FLG(D)), te_c(et));
    V(RN(LN(LT(1, "v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), L(4,
        ON(te_c(et), NOP, te_c(etn), SET(1, "b", P(NULL), TS(I6))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(etn)),
        ON(TS(I6), DFN, EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), SN(I6, I6(10101))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)))
    ))));
    te_f(et);
    te_f(etn);
}

T(lfac) {
    IC(TPGM(lfac));
    te *fn = TFN(FN, TS(I6), 1, "n", TS(I6), 0);
    te *et = TH(ET, 1, "fac", te_c(fn));
    V(RN(LN(LT(1, "e", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), L(3,
        ON(te_c(et), NOP, EN("l", FLG(0, LTE_FLG(D)), te_c(et)), SET(1, "fac", P(NULL), te_c(fn))),
        ON(TS(I6), DFN, EN("e", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)), AN(TS(I6), NN(S, te_c(fn)), L(1, SN(I6, I6(6))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("e", FLG(0, LTE_FLG(E) | LTE_FLG(L)), TS(I6)))
    ))));
    te_f(fn);
    te_f(et);
}

T(istr) {
    IC(TPGM(istr));
    V(RN(LN(LT(3, "a", FLG(0, LTE_FLG(L)), TS(I6), "b", FLG(0, LTE_FLG(L)), TS(F6), "c", FLG(1, LTE_FLG(L)), TS(SG)), L(4,
        ON(TS(I6), DFN, EN("a", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(987))),
        ON(TS(F6), DFN, EN("b", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(3.14))),
        ON(TS(SG), DFN, EN("c", FLG(1, LTE_FLG(L)), TS(SG)), AN(TS(SG), CS("a: %ld, b: %lf"), L(2, EN("a", FLG(0, LTE_FLG(L)), TS(I6)), EN("b", FLG(0, LTE_FLG(L)), TS(F6))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("c", FLG(1, LTE_FLG(L)), TS(SG)))
    ))));
}

T(fnscope) {
    IC(TPGM(fnscope));
    te *fn = TFN(FN, TS(F6), 1, "n", TS(F6), 0);
    tbl *args = fld_type_tbl_i(true, 1, "n", TS(F6), 0);
    tbl *scope = fld_type_tbl_i(true, 1, "v", TS(F6), 0);
    te *ft = TFS(NF, TS(F6), args, scope);
    V(RN(LN(LT(3, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "a", FLG(-1, LTE_FLG(F)), te_c(fn), "c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(5,
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), NN(T, te_c(fn))),
        ON(te_c(fn), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(1, "n", FLG(0, LTE_FLG(A)), TS(F6)), L(1,
                AN(TS(F6), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
        ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
            LN(LT(3, "v", FLG(0, LTE_FLG(L)), TS(F6), "f", FLG(-3, LTE_FLG(F)), te_c(ft), "n", FLG(0, LTE_FLG(A)), TS(F6)), L(3,
                ON(TS(F6), DFN, EN("v", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(4.3))),
                ON(te_c(ft), DFN, EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), ON(te_c(ft), CST, NN(T, te_c(ft)),
                    LN(LT(2, "n", FLG(0, LTE_FLG(A)), TS(F6), "v", FLG(0, LTE_FLG(S)), TS(F6)), L(1,
                        ON(TS(F6), SUB, EN("n", FLG(0, LTE_FLG(A)), TS(F6)), EN("v", FLG(0, LTE_FLG(S)), TS(F6)))))
                )),
                AN(TS(F6), EN("f", FLG(-3, LTE_FLG(F)), te_c(ft)), L(1, EN("n", FLG(0, LTE_FLG(A)), TS(F6))))
            )))),
            ON(te_c(fn), DFN, EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
                LN(LT(1, "n", FLG(0, LTE_FLG(A)), TS(F6)),
                    L(1, AN(TS(F6), EN("a", FLG(-1, LTE_FLG(F)), te_c(fn)), L(1,
                        EN("n", FLG(0, LTE_FLG(A)), TS(F6)))))))),
            ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(F6), EN("c", FLG(-4, LTE_FLG(F)), te_c(fn)), L(1,
                SN(F6, F6(9.6)))))
    ))));
    te_f(fn);
    te_f(ft);
}

T(or) {
    IC(TPGM(or));
    te *ea = EN("a", FLG(0, LTE_FLG(L)), TS(I6)), *eb = EN("b", FLG(1, LTE_FLG(L)), TS(I6));
    V(RN(LN(LT(2, "a", FLG(0, LTE_FLG(L)), TS(I6), "b", FLG(1, LTE_FLG(L)), TS(I6)), L(3,
        ON(TS(I6), DFN, te_c(ea), SN(I6, I6(0))),
        ON(TS(I6), DFN, te_c(eb), SN(I6, I6(1))),
        ON(TS(VD), IF,
            LN(NULL, L(1, ON(TS(BL), OR, LN(NULL, L(1, ON(TS(BL), NE, te_c(ea), SN(I6, I6(0))))), LN(NULL, L(1, ON(TS(BL), NE, te_c(eb), SN(I6, I6(0)))))))),
            LN(NULL, L(1, ON(TS(VD), DUMP, SN(U5, U5(1)), ON(TS(I6), ADD, te_c(ea), te_c(eb))))))
    ))));
    te_f(ea);
    te_f(eb);
}

T(un) {
    IC(TPGM(un));
    te *u = TH(UN, 2, "a", TS(I6), "b", TS(F6));
    te *eu = EN("u", FLG(0, LTE_FLG(L)), te_c(u));
    V(RN(LN(LT(1, "u", FLG(0, LTE_FLG(L)), te_c(u)), L(5,
        ON(te_c(u), DFN, te_c(eu), ZTN("a", TS(VD), SN(I6, I6(1)))),
        ON(te_c(u), AGN, te_c(eu), ZTN("b", TS(VD), SN(F6, F6(2.2)))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), te_c(eu)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), ZTN("b", TS(F6), te_c(eu))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), ZTN("a", TS(I6), te_c(eu)))
    ))));
    te_f(u);
    te_f(eu);
}

T(unmatch) {
    IC(TPGM(unmatch));
    te *u = TH(UN, 2, "a", TS(I6), "b", TS(I6));
    V(RN(LN(LT(2, "u", FLG(0, LTE_FLG(L)), te_c(u), "c", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(SG)), L(4,
        ON(te_c(u), DFN, EN("u", FLG(0, LTE_FLG(L)), te_c(u)), ZTN("a", TS(VD), SN(I6, I6(1)))),
        ON(te_c(u), AGN, EN("u", FLG(0, LTE_FLG(L)), te_c(u)), ZTN("b", TS(VD), SN(I6, I6(2)))),
        ON(TS(SG), DFN, EN("c", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(SG)), ON(TS(SG), MTCH,
            LN(NULL, L(1, EN("u", FLG(0, LTE_FLG(L)), te_c(u)))),
            LN(LT(2, "a", FLG(-1, LTE_FLG(Y)), TS(I6), "b", FLG(-2, LTE_FLG(Y)), TS(I6)), L(3,
                ZTN("a", TS(VD), AN(TS(SG), CS("a %ld"), L(1, EN("a", FLG(-1, LTE_FLG(Y)), TS(I6))))),
                ZTN("b", TS(VD), AN(TS(SG), CS("b %ld"), L(1, EN("b", FLG(-2, LTE_FLG(Y)), TS(I6))))),
                ON(TS(SG), CSG, NULL, CS("inv"))
            )))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("c", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(SG)))
    ))));
    te_f(u);
}

T(ns) {
    IC(TPGM(ns));
    te *u = TH(UN, 2, "e", TV(ER, TS(SG)), "v", TS(SG));
    V(RN(LN(LT(2, "s", FLG(0, LTE_FLG(L)), TS(SG), "f", FLG(1, LTE_FLG(L)), te_c(u)), L(3,
        ON(TS(SG), DFN, EN("s", FLG(0, LTE_FLG(L)), TS(SG)), ON(TS(SG), CSG, NULL, CS("./ex/sum.kpl"))),
        ON(te_c(u), DFN, EN("f", FLG(1, LTE_FLG(L)), te_c(u)), AN(te_c(u), NNV(S, TFN(FP, te_c(u), 1, "s", TS(SG), 0), P(NULL)), L(1, EN("s", FLG(0, LTE_FLG(L)), TS(SG))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), ZTN("v", TS(SG), EN("f", FLG(1, LTE_FLG(L)), te_c(u))))
    ))));
    te_f(u);
}

T(iline) {
    IC(TPGM(iline));
    te *u = TH(UN, 2, "e", TV(ER, TS(SG)), "v", TS(SG));
    V(RN(LN(LT(2, "0", FLG(0, LTE_FLG(L)), TS(SG), "1", FLG(1, LTE_FLG(L)), te_c(u)), L(1,
        ON(TS(VD), DUMP, SN(U5, U5(1)), ZTN("v", TS(SG),
            ON(te_c(u), DFN, EN("1", FLG(1, LTE_FLG(L)), te_c(u)),
                AN(te_c(u),
                    NNV(S, TFN(FP, te_c(u), 1, "s", TS(SG), 0), P(NULL)),
                    L(1, ON(TS(SG), DFN, EN("0", FLG(0, LTE_FLG(L)), TS(SG)),
                        ON(TS(SG), CSG, NULL, CS("./ex/sum.kpl"))))))))
    ))));
    te_f(u);
}

T(unsg) {
    IC(TPGM(unsg));
    te *u = TH(UN, 2, "s", TS(SG), "v", TS(I6));
    un f = FLG(0, LTE_FLG(L));
    V(RN(LN(LT(1, "a", f, te_c(u)), L(3,
        ON(te_c(u), DFN, EN("a", f, te_c(u)), ZTN("v", TS(VD), SN(I6, I6(1)))),
        ON(te_c(u), AGN, EN("a", f, te_c(u)), ZTN("s", TS(VD), ON(TS(SG), CSG, NULL, CS("a")))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("a", f, te_c(u)))
    ))));
    te_f(u);
}

T(vridx) {
    IC(TPGM(vridx));
    te *tsg = type_te_i_v(&al_te, NULL, 2, TS(SG), TS(SG));
    te *vr = TV(VR, te_c(tsg));
    te *u = TH(UN, 2, "e", chk_rt_err_t(&al_te), "v", te_c(tsg));
    V(RN(LN(LT(4, "v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr),"0", FLG(1, LTE_FLG(L)), te_c(u), "1", FLG(2, LTE_FLG(L)), te_c(u), "2", FLG(3, LTE_FLG(L)), te_c(u)), L(5,
        ON(te_c(vr), DFN, EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr)),
            VN(te_c(vr), L(2,
                VN(te_c(tsg),
                    L(2, ON(TS(SG), CSG, NULL, CS("a")), ON(TS(SG), CSG, NULL, CS("b")))),
                VN(te_c(tsg),
                    L(2, ON(TS(SG), CSG, NULL, CS("c")), ON(TS(SG), CSG, NULL, CS("d"))))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr))),
        ON(TS(VD), DUMP, SN(U5, U5(1)),
            ON(te_c(u), DFN, EN("0", FLG(1, LTE_FLG(L)), te_c(u)), AN(te_c(u), EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr)), L(1, SN(I6, I6(-1)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(SG), ZTN("v", te_c(tsg),
            ON(te_c(u), DFN, EN("1", FLG(2, LTE_FLG(L)), te_c(u)),
                AN(te_c(u), EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr)), L(1, SN(I6, I6(-1)))))),
                    L(1, SN(I6, I6(0))))),
        ON(TS(VD), MTCH,
            LN(NULL, L(1, ON(te_c(u), DFN, EN("2", FLG(3, LTE_FLG(L)), te_c(u)), AN(te_c(u), EN("v", FLG(0, LTE_FLG(E) | LTE_FLG(L)), te_c(vr)), L(1, SN(I6, I6(3))))))),
            LN(LT(1, "e", FLG(-1, LTE_FLG(Y)), chk_rt_err_t(&al_te)), L(1,
                ZTN("e", TS(VD), ON(TS(VD), DUMP, SN(U5, U5(1)), EN("e", FLG(-1, LTE_FLG(Y)), chk_rt_err_t(&al_te)))))))
    ))));
    te_f(tsg);
    te_f(vr);
    te_f(u);
}

T(nscope) {
    IC(TPGM(nscope));
    tbl *args = fld_type_tbl_i(true, 0);
    tbl *fbs = fld_type_tbl_i(true, 1, "x", TS(F6), 0);
    te *fb = TFS(NF, TS(F6), args, fbs);
    V(RN(LN(LT(2, "x", FLG(0, LTE_FLG(L)), TS(F6), "fa", FLG(-1, LTE_FLG(F)), te_c(fb)), L(4,
        ON(TS(F6), DFN, EN("x", FLG(0, LTE_FLG(L)), TS(F6)), SN(F6, F6(6.6))),
        ON(te_c(fb), DFN, EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), ON(te_c(fb), CST, NN(T, te_c(fb)),
            LN(LT(1, "fb", FLG(-2, LTE_FLG(F)), te_c(fb)), L(2,
                ON(te_c(fb), DFN, EN("fb", FLG(-2, LTE_FLG(F)), te_c(fb)), ON(te_c(fb), CST, NN(T, te_c(fb)), LN(LT(1, "x", FLG(0, LTE_FLG(S)), TS(F6)), L(1,
                    ON(TS(F6), DIVA, EN("x", FLG(0, LTE_FLG(S)), TS(F6)), SN(F6, F6(2.0)))
                )))),
                AN(TS(F6), EN("fb", FLG(-2, LTE_FLG(F)), te_c(fb)), NULL)
        )))),
        AN(TS(F6), EN("fa", FLG(-1, LTE_FLG(F)), te_c(fb)), NULL),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("x", FLG(0, LTE_FLG(L)), TS(F6)))
    ))));
    te_f(fb);
}

T(cj) {
    IC(TPGM(cj));
    tbl *args = fld_type_tbl_i(true, 0);
    tbl *scope = fld_type_tbl_i(true, 1, "c", TS(I6), 0);
    te *nf = TFS(NF, TS(I6), args, scope);
    te *cj = TV(CJ, te_c(nf));
    te *fn = type_f_i(&al_te, NULL, TYPE(FN), te_c(cj), tbl_c(args), NULL);
    V(RN(LN(LT(3, "mc", FLG(-1, LTE_FLG(F)), te_c(fn), "cc", FLG(0, LTE_FLG(L)), te_c(cj), "e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), L(5,
        ON(te_c(fn), DFN, EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)),
            ON(te_c(fn), CST, NN(T, te_c(fn)), LN(LT(2, "c", FLG(0, LTE_FLG(L)), TS(I6), "-1", FLG(-2, LTE_FLG(F)), te_c(nf)), L(2,
                ON(TS(I6), DFN, EN("c", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
                ON(te_c(cj), CST, NN(T, te_c(cj)),
                    ON(te_c(nf), DFN, EN("-1", FLG(-2, LTE_FLG(F)), te_c(nf)),
                        ON(te_c(nf), CST, NN(T, te_c(nf)), LN(LT(1, "c", FLG(0, LTE_FLG(S)), TS(I6)), L(1,
                            ON(TS(I6), ADDA, EN("c", FLG(0, LTE_FLG(S)), TS(I6)), SN(I6, I6(1)))
                        )))))
            )))),
        ON(te_c(cj), DFN, EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), AN(te_c(cj), EN("mc", FLG(-1, LTE_FLG(F)), te_c(fn)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(VD), DUMP, SN(U5, U5(1)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL)),
        ON(TS(I6), DFN, EN("e", FLG(1, LTE_FLG(L) | LTE_FLG(E)), TS(I6)), AN(TS(I6), EN("cc", FLG(0, LTE_FLG(L)), te_c(cj)), NULL))
    ))));
    te_f(nf);
    te_f(cj);
    te_f(fn);
}

T(leap) {
    IC(TPGM(leap));
    te *vt = TV(VR, TS(I6));
    V(RN(LN(LT(2, "y", FLG(0, LTE_FLG(L)), TS(I6), "v", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(vt)), L(4,
        ON(TS(I6), DFN, EN("y", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(2000))),
        ON(te_c(vt), DFN, EN("v", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(vt)), VN(te_c(vt), LE())),
        ON(TS(VD), LOOP, LN(NULL, L(1, ON(TS(BL), LTE, EN("y", FLG(0, LTE_FLG(L)), TS(I6)), SN(I6, I6(2030))))), LN(LT(2, "y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6), "v", FLG(1, LTE_FLG(O) | LTE_FLG(L) | LTE_FLG(E)), te_c(vt)), L(2,
            ON(TS(VD), IF,
                LN(NULL, L(1, ON(TS(BL), OR,
                    LN(NULL, L(1, ON(TS(BL), AND,
                        LN(NULL, L(1, ON(TS(BL), EQ, SN(I6, I6(0)), ON(TS(I6), MOD, EN("y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(4)))))),
                        LN(NULL, L(1, ON(TS(BL), NE, SN(I6, I6(0)), ON(TS(I6), MOD, EN("y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(100))))))))),
                    LN(NULL, L(1, ON(TS(BL), EQ, SN(I6, I6(0)),
                        ON(TS(I6), MOD, EN("y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(400))))))))),
                LN(NULL, L(1, ON(te_c(vt), CNCTA, EN("v", FLG(1, LTE_FLG(O) | LTE_FLG(L) | LTE_FLG(E)), te_c(vt)), EN("y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)))))),
            ON(TS(I6), ADDA, EN("y", FLG(0, LTE_FLG(O) | LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), EN("v", FLG(1, LTE_FLG(E) | LTE_FLG(L)), te_c(vt)))
    ))));
    te_f(vt);
}

T(vrup) {
    IC(TPGM(vrup));
    te *vt = TV(VR, TS(I6));
    te *ue = chk_rt_err_t(&al_te);
    te *ut = TH(UN, 2, "e", te_c(ue), "v", TS(I6));
    te *ft = TFN(FN, te_c(ut), 1, "a", te_c(vt), 0);
    V(RN(LN(LT(2, "v", FLG(0, LTE_FLG(L)), te_c(vt), "f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(3,
        ON(te_c(vt), DFN, EN("v", FLG(0, LTE_FLG(L)), te_c(vt)), VN(te_c(vt), L(3, SN(I6, I6(1)), SN(I6, I6(2)), SN(I6, I6(3))))),
           ON(te_c(ft), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)),
                ON(te_c(ft), CST, NN(T, te_c(ft)), LN(LT(1, "a", FLG(0, LTE_FLG(A)), te_c(vt)), L(4,
                    ON(TS(I6), AGN, AN(te_c(ut), EN("a", FLG(0, LTE_FLG(A)), te_c(vt)), L(1, SN(I6, I6(1)))), SN(I6, I6(5))),
                    ON(TS(VD), DUMP, SN(U5, U5(1)), EN("a", FLG(0, LTE_FLG(A)), te_c(vt))),
                    ON(TS(I6), AGN, AN(te_c(ut), EN("a", FLG(0, LTE_FLG(A)), te_c(vt)), L(1, SN(I6, I6(5)))), SN(I6, I6(1))),
                    ZTN("v", TS(VD), SN(I6, I6(1)))
                )))),
           ON(TS(I6), UNER, NULL, AN(te_c(ut), EN("f", FLG(-1, LTE_FLG(F)), te_c(ft)), L(1, EN("v", FLG(0, LTE_FLG(L)), te_c(vt)))))
    ))));
    te_f(vt);
    te_f(ue);
    te_f(ut);
    te_f(ft);
}

T(stll) {
    IC(TPGM(stll));
    te *stut = TH(ST, 2, "i", TS(I6), "n", TH(UN, 1, "y", TS(VD)));
    te *ut = TH(UN, 2, "y", TS(VD), "z", TRF());
    te *st = TH(ST, 2, "i", TS(I6), "n", te_c(ut));
    tbl *args = fld_type_tbl_i(true, 1, "i", TS(I6), 0);
    tbl *scope = fld_type_tbl_i(true, 1, "l", te_c(st), 0);
    te *nf = TFS(NF, te_c(st), args, scope);
    te *fn = TFN(FN, te_c(st), 1, "x", te_c(st), 0);
    V(RN(LN(LT(3, "l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(st), "a", FLG(-1, LTE_FLG(F)), te_c(nf), "i", FLG(1, LTE_FLG(L)), TS(I6)), L(5,
        ON(te_c(st), DFN, EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(st)),
            ON(te_c(st), CST, NN(T, te_c(st)), ON(te_c(stut), MTCH, LN(NULL, NULL), LN(NULL, L(2,
                ZTN("i", TS(VD), SN(I6, I6(0))),
                ZTN("n", TS(VD), ZTN("y", TH(UN, 1, "y", TS(VD)), NULL))
            ))))),
        ON(te_c(nf), DFN, EN("a", FLG(-1, LTE_FLG(F)), te_c(nf)), ON(te_c(nf), CST, NN(T, te_c(nf)),
            LN(LT(4, "b", FLG(-2, LTE_FLG(F)), te_c(fn), "l", FLG(0, LTE_FLG(S)), te_c(st), "i", FLG(0, LTE_FLG(A)), TS(I6), "0", FLG(0, LTE_FLG(L)), te_c(st)), L(2,
                ON(te_c(fn), DFN, EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), ON(te_c(fn), CST, NN(T, te_c(fn)),
                    LN(LT(1, "x", FLG(0, LTE_FLG(A)), te_c(st)), L(1,
                        ON(te_c(st), MTCH,
                            LN(NULL, L(1,
                                ZTN("n", te_c(ut), EN("x", FLG(0, LTE_FLG(A)), te_c(st)))
                            )),
                            LN(LT(1, "z", FLG(-1, LTE_FLG(Y)), te_c(st)), L(2,
                                ZTN("z", TS(VD),
                                    AN(te_c(st), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1,
                                        EN("z", FLG(-1, LTE_FLG(Y)), te_c(st))
                                    ))),
                                ZTN("y", TS(VD), EN("x", FLG(0, LTE_FLG(A)), te_c(st)))
                            )))
                    )))),
                ON(te_c(st), AGN,
                    ZTN("z", te_c(st), ZTN("n", te_c(ut),
                        ON(te_c(st), DFN,
                            EN("0", FLG(0, LTE_FLG(L)), te_c(st)),
                            AN(te_c(st), EN("b", FLG(-2, LTE_FLG(F)), te_c(fn)), L(1,
                                EN("l", FLG(0, LTE_FLG(S)), te_c(st))
                            ))))),
                        ON(te_c(st), CST, NN(T, te_c(st)), ON(te_c(stut), MTCH, LN(NULL, NULL),
                            LN(LT(1, "i", FLG(0, LTE_FLG(A)), TS(I6)), L(2,
                                ZTN("i", TS(VD), EN("i", FLG(0, LTE_FLG(A)), TS(I6))),
                                ZTN("n", TS(VD), ZTN("y", TH(UN, 1, "y", TS(VD)), NULL))
                            )))))
            )))),
            ON(TS(I6), DFN, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(0))),
            ON(TS(VD), LOOP,
                LN(NULL, L(1,
                    ON(TS(BL), LTE, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(5)))
                )),
                LN(NULL, L(1,
                    AN(te_c(st), EN("a", FLG(-1, LTE_FLG(F)), te_c(nf)), L(1,
                        ON(TS(I6), ADDA, EN("i", FLG(1, LTE_FLG(L)), TS(I6)), SN(I6, I6(1)))
                    ))
                ))),
            ON(TS(VD), DUMP, SN(U5, U5(1)), EN("l", FLG(0, LTE_FLG(L) | LTE_FLG(E)), te_c(st)))
    ))));
    te_f(stut);
    te_f(ut);
    te_f(st);
    te_f(nf);
    te_f(fn);
}
