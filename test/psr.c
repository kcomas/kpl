
#include "psr_t.h"

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

static te *ppnode(te *h) {
    if (!h) return NULL;
    printf("%s\n", ((psr*) h->d[0].p)->tt->s->d);
    node_p(h->d[2].p, 0);
    putchar('\n');
    return h;
}

void psr_verify_lst(_tests *_t, const lst *l, const node_id v[], size_t *i, size_t vl);

#define VV(N, VA, I, VL) psr_verify(_t, N, VA, I, VL); \
    if (_t->m) return

#define VN(V, I, VL, NT) A(V[*I] == NODE_TYPE(NT), #NT); \
    A(++*I <= VL, "verify")

#define VL(L, V, I, VL) psr_verify_lst(_t, L, V, I, VL); \
    A(*I <= VL, "verify_lst")

void psr_verify(_tests *_t, const te *n, const node_id v[], size_t *i, size_t vl) {
    E();
    if (!n) {
        VN(v, i, vl, NONE);
        return;
    }
    switch (n->d[1].u6) {
        case NODE_TYPE(ROOT):
            VN(v, i, vl, ROOT);
            VV(n->d[2].p, v, i, vl);
            break;
        case NODE_TYPE(VAR):
            VN(v, i, vl, VAR);
            break;
        case NODE_TYPE(TYPE):
            VN(v, i, vl, TYPE);
            break;
        case NODE_TYPE(KEY):
            VN(v, i, vl, KEY);
            break;
        case NODE_TYPE(INT):
            VN(v, i, vl, INT);
            break;
        case NODE_TYPE(STR):
            VN(v, i, vl, STR);
            break;
        case NODE_TYPE(FLT):
            VN(v, i, vl, FLT);
            break;
        case NODE_TYPE(OP):
            VN(v, i, vl, OP);
            VV(n->d[3].p, v, i, vl);
            VV(n->d[4].p, v, i, vl);
            break;
        case NODE_TYPE(VEC):
            VN(v, i, vl, VEC);
            VL(n->d[3].p, v, i, vl);
            break;
        case NODE_TYPE(LST):
            VN(v, i, vl, LST);
            VL(n->d[3].p, v, i, vl);
            break;
        case NODE_TYPE(APLY):
            VN(v, i, vl, APLY);
            VV(n->d[3].p, v, i, vl);
            VL(n->d[4].p, v, i, vl);
            break;
        case NODE_TYPE(SYM):
            VN(v, i, vl, SYM);
            VV(n->d[3].p, v, i, vl);
            break;
        case NODE_TYPE(CMD):
            VN(v, i, vl, CMD);
            VV(n->d[3].p, v, i, vl);
            break;
        default:
            A(0, "NODE");
            break;
    }
}

void psr_verify_lst(_tests *_t, const lst *l, const node_id v[], size_t *i, size_t vl) {
    E();
    if (!l) return;
    te *h = l->h;
    while (h) {
        VV(h->d[0].p, v, i, vl);
        h = h->d[2].p;
    }
}

#define VS(VA) (sizeof(VA) / sizeof(VA[0]))

#define V(H, ...)  const node_id v[] = __VA_ARGS__; \
    size_t i = 0; \
    psr_verify(_t, H, v, &i, VS(v)); \
    A(i == VS(v), "vl")

#define N(X) NODE_TYPE(X)

#define OP(L, R) N(OP), L, R

#define VEC(...) N(VEC), __VA_ARGS__

#define LST(...) N(LST), __VA_ARGS__

#define APLY(...) N(APLY), __VA_ARGS__

#define SYM(TGT) N(SYM), TGT

#define CMD(TGT) N(CMD), TGT

T(sigma) {
    te *h = ppnode(psr_r(ppsr(bpsr("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]"))));
    V(h, {N(ROOT), OP(N(INT), VEC(N(INT), OP(N(FLT), VEC(N(INT), N(INT), N(INT))), OP(N(INT), N(INT)), N(INT)))});
    te_f(h);
}

T(aplyopadd) {
    te *h = ppnode(psr_r(bpsr(TPGM(aplyopadd))));
    V(h, {N(ROOT), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT))});
    te_f(h);
}

T(nl) {
    te *h = ppnode(psr_r(bpsr("{\n3.2 - 2.1\n1 Σ [1;2;3]\n}")));
    V(h, {N(ROOT), LST(OP(N(FLT), N(FLT)), OP(N(INT), VEC(N(INT), N(INT), N(INT))))});
    te_f(h);
}

T(typetype) {
    te *h = ppnode(psr_r(bpsr(TPGM(typetype))));
    V(h, {N(ROOT), APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), APLY(N(TYPE), SYM(N(TYPE)), N(TYPE)))});
    te_f(h);
}

T(sym) {
    te *h = ppnode(psr_r(bpsr("`a`b`c")));
    V(h, {N(ROOT), SYM(SYM(SYM(N(NONE))))});
    te_f(h);
}

T(op_call) {
    te *h = ppnode(psr_r(bpsr("{a:+;a(3.14;9.8)}")));
    V(h, {N(ROOT), LST(OP(N(VAR), OP(N(NONE), N(NONE))), APLY(N(VAR), N(FLT), N(FLT)))});
    te_f(h);
}

T(inv) {
    te *h = ppnode(psr_r(bpsr("+(1;2")));
    A(h == NULL, "not null");
}

T(aply_prec) {
    te *h = ppnode(psr_r(bpsr("(1+3)")));
    V(h, {N(ROOT), APLY(N(NONE), OP(N(INT), N(INT)))});
    te_f(h);
}

T(sym_add) {
    te *h = ppnode(psr_r(bpsr("+`a")));
    V(h, {N(ROOT), SYM(OP(N(NONE), N(NONE)))});
    te_f(h);
}

T(sym_lst) {
    te *h = ppnode(psr_r(bpsr("[`a;`b;`c]")));
    V(h, {N(ROOT), VEC(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE)))});
    te_f(h);
}

T(var_op_op_aply) {
    te *h = ppnode(psr_r(bpsr("x:+(1;2)")));
    V(h, {N(ROOT), OP(N(VAR), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT)))});
    te_f(h);
}

T(neg) {
    te *h = ppnode(psr_r(bpsr("x:-1+2")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), OP(N(INT), N(INT))))});
    te_f(h);
}

T(op_op) {
    te *h = ppnode(psr_r(bpsr("{-+4;-(+4)}")));
    V(h, {N(ROOT), LST(OP(N(NONE), OP(N(NONE), N(INT))), APLY(OP(N(NONE), N(NONE)), OP(N(NONE), N(INT))))});
    te_f(h);
}

T(fnadd3) {
    te *h = ppnode(psr_r(bpsr(TPGM(fnadd3))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)), LST(OP(N(NONE), OP(N(VAR), OP(N(VAR), N(VAR))))))),
        CMD(APLY(N(VAR), N(INT), N(INT), N(INT)))
        ))});
    te_f(h);
}

T(fnf6muli6cstdiv) {
    te *h = ppnode(psr_r(bpsr(TPGM(fnf6muli6cstdiv))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)),
            LST(
                APLY(OP(N(NONE), N(NONE)), OP(N(VAR), N(VAR)), OP(N(TYPE), N(VAR)))
            ))),
        CMD(APLY(N(VAR), N(FLT), N(FLT), OP(N(TYPE), N(INT))))
        ))});
    te_f(h);
}

T(apltypefn) {
    te *h = ppnode(psr_r(bpsr(TPGM(apltypefn))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(N(TYPE), LST(
            OP(APLY(N(TYPE), N(VAR)), OP(APLY(N(TYPE), N(VAR)), N(VAR)))))),
            CMD(APLY(N(VAR), N(INT), N(INT)))
        ))});
    te_f(h);
}

T(facloop) {
    te *h = ppnode(psr_r(bpsr(TPGM(facloop))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(INT)),
        OP(CMD(N(VAR)), N(INT)),
        OP(
            APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)),
            LST(
                OP(N(VAR), N(VAR)),
                OP(N(VAR), N(INT))
            )
        ),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(facrec) {
    te *h = ppnode(psr_r(bpsr(TPGM(facrec))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), N(TYPE)), LST(
            OP(
                LST(OP(N(VAR), N(INT))),
                LST(
                    OP(N(VAR), APLY(N(VAR), OP(N(VAR), N(INT)))),
                    OP(N(TYPE), N(INT))
                )
            )
        ))),
        CMD(APLY(N(VAR), OP(N(TYPE), N(INT))))
    ))});
    te_f(h);
}

T(fibrec) {
    te *h = ppnode(psr_r(bpsr(TPGM(fibrec))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), N(TYPE)), LST(
            OP(N(NONE), LST(
                OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), N(INT)),
                OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), N(INT)),
                OP(APLY(N(VAR), OP(N(VAR), N(INT))), APLY(N(VAR), OP(N(VAR), N(INT))))
            ))
        ))),
        CMD(APLY(N(VAR), N(INT)))
    ))});
    te_f(h);
}

T(ackrec) {
    te *h = ppnode(psr_r(bpsr(TPGM(ackrec))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)), LST(
            OP(N(NONE), LST(
                OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), OP(N(VAR), N(INT))),
                OP(APLY(OP(N(NONE), N(NONE)), OP(N(VAR), N(INT)), OP(N(VAR), N(INT))), APLY(N(VAR), OP(N(VAR), N(INT)), OP(N(TYPE), N(INT)))),
                OP(APLY(OP(N(NONE), N(NONE)), OP(N(VAR), N(INT)), OP(N(VAR), N(INT))), APLY(N(VAR), OP(N(VAR), N(INT)), APLY(N(VAR), N(VAR), OP(N(VAR), N(INT))))),
                OP(N(VAR), N(INT))
            ))
        ))),
        CMD(APLY(N(VAR), OP(N(TYPE), N(INT)), OP(N(TYPE), N(INT))))
    ))});
    te_f(h);
}

T(prec) {
    te *h = ppnode(psr_r(bpsr("a:-(w) + -(x;y) + -z")));
    V(h, {N(ROOT), OP(N(VAR), OP(APLY(OP(N(NONE), N(NONE)), N(VAR)), OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(VAR)), OP(N(NONE), N(VAR)))))});
    te_f(h);
}

T(call_call_call) {
    te *h = ppnode(psr_r(bpsr("a(1;2)(3.3)()")));
    V(h, {N(ROOT), APLY(APLY(APLY(N(VAR), N(INT), N(INT)), N(FLT)))});
    te_f(h);
}

T(defer_hsh) {
    te *h = ppnode(psr_r(bpsr("d:#{1`x;(1+5-4)`y;3`z}")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), LST(SYM(N(INT)), SYM(APLY(N(NONE), OP(N(INT), OP(N(INT), N(INT))))), SYM(N(INT)))))});
    te_f(h);
}

T(value_hsh) {
    te *h = ppnode(psr_r(bpsr("[`a;`b;`c]#[1;2;3]")));
    V(h, {N(ROOT), OP(VEC(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE))), VEC(N(INT), N(INT), N(INT)))});
    te_f(h);
}

T(if_else_else) {
    te *h = ppnode(psr_r(bpsr("?{a?(c:2;a?c:1;c);(b+a)?2;3}")));
    V(h, {N(ROOT),
        OP(
            N(NONE), // ?
            LST( // {
                OP( // ?
                    N(VAR), //a
                    APLY(N(NONE),
                        OP(N(VAR), N(INT)), // c:2
                        OP(N(VAR), OP(N(VAR), N(INT))), // a?c:1
                        N(VAR) // c
                    )
                ),
                OP( // ?
                    APLY(N(NONE),
                        OP(N(VAR), N(VAR)) // b + a
                    ),
                    N(INT) // 2
                ),
                N(INT) // 3
            )
        )
    });
    te_f(h);
}

T(match) {
    te *h = ppnode(psr_r(bpsr("{d:UN(I6`v;FN(I6)`e)$5`v;d?{d`v`v;0`e;2}}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR),
            OP(APLY(N(TYPE),
                    SYM(N(TYPE)), SYM(APLY(N(TYPE), N(TYPE)))),
               SYM(N(INT)))),
        OP(N(VAR), LST(SYM(SYM(N(VAR))), SYM(N(INT)), N(INT)))
    )});
    te_f(h);
}

T(defer_call) {
    te *h = ppnode(psr_r(bpsr("{a:1;b:2;{a+b+c}(3`c)}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR), N(INT)),
        OP(N(VAR), N(INT)),
        APLY(LST(OP(N(VAR), OP(N(VAR), N(VAR)))), SYM(N(INT)))
    )});
    te_f(h);
}

T(nested_defer) {
    te *h = ppnode(psr_r(bpsr("{f:{a:2;x+y()};y:{a};f(1`x)}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR), // f:
            LST(
                OP(N(VAR), N(INT)), // a:2
                OP(N(VAR), APLY(N(VAR))))), // x + y()
        OP(N(VAR), LST(N(VAR))), // y:{a}
        APLY(N(VAR), SYM(N(INT))) // f(1`x)
    )});
    te_f(h);
}

T(add_flt_fn) {
    te *h = ppnode(psr_r(bpsr("{f:FN(F6`x;F6`y;F6)${x-y};f(1.23;4.56)}")));
    V(h, {N(ROOT),
        LST(
            OP(N(VAR),
                OP(
                    APLY(N(TYPE),
                        SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)
                    ),
                    LST(OP(N(VAR), N(VAR)))
                )
            ),
            APLY(N(VAR), N(FLT), N(FLT))
         )
    });
    te_f(h);
}

T(scope) {
    te *h = ppnode(psr_r(bpsr(TPGM(scope))));
    V(h, {N(ROOT), APLY(LST(
        OP(CMD(N(VAR)), N(INT)),
        OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), N(TYPE)), LST(OP(N(VAR), N(VAR))))),
        APLY(N(VAR), N(INT)),
        APLY(N(VAR), N(INT)),
        APLY(N(VAR), N(INT)),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(efn) {
    te *h = ppnode(psr_r(bpsr(TPGM(efn))));
    V(h, {N(ROOT), APLY(LST(
        OP(CMD(N(VAR)), OP(N(TYPE), LST(
            OP(APLY(N(TYPE), N(VAR)), OP(APLY(N(TYPE), N(VAR)), APLY(N(TYPE), N(VAR))))
        )))
    ))});
    te_f(h);
}

T(teadd) {
    te *h = ppnode(psr_r(bpsr(TPGM(teadd))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), VEC(N(INT), N(INT), N(INT))),
        OP(APLY(N(VAR), N(INT)), OP(APLY(N(VAR), N(INT)), APLY(N(VAR), N(INT)))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(vrmul) {
    te *h = ppnode(psr_r(bpsr(TPGM(vrmul))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(N(TYPE), VEC(OP(N(VAR), N(FLT))))),
        OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), OP(N(VAR), OP(N(VAR), N(INT)))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(st) {
    te *h = ppnode(psr_r(bpsr(TPGM(st))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(INT)),
        OP(N(VAR), N(FLT)),
        OP(CMD(N(VAR)), OP(N(NONE), LST(
            SYM(N(VAR)),
            SYM(N(VAR)),
            SYM(APLY(OP(N(NONE), N(NONE)), N(TYPE), OP(N(VAR), N(INT)))),
            SYM(N(STR))
        ))),
        OP(SYM(N(VAR)), OP(N(INT), OP(N(TYPE), SYM(N(VAR))))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(import) {
    te *h = ppnode(psr_r(bpsr(TPGM(import))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), CMD(N(STR))),
        CMD(N(VAR)),
        OP(CMD(N(VAR)), OP(N(INT), OP(SYM(N(VAR)), N(INT)))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(lfac) {
    te *h = ppnode(psr_r(bpsr(TPGM(lfac))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), CMD(N(STR))),
        OP(CMD(N(VAR)), APLY(SYM(N(VAR)), N(INT))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(istr) {
    te *h = ppnode(psr_r(bpsr(TPGM(istr))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(INT)),
        OP(N(VAR), N(FLT)),
        OP(N(VAR), APLY(N(STR), N(VAR), N(VAR))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(fnscope) {
    te *h = ppnode(psr_r(bpsr(TPGM(fnscope))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), APLY(N(TYPE), SYM(N(TYPE)), N(TYPE))),
        OP(N(VAR), OP(N(TYPE), LST(APLY(N(VAR), APLY(N(TYPE), N(VAR)))))),
        OP(N(VAR), OP(N(TYPE), LST(
            OP(N(VAR), N(FLT)),
            OP(N(VAR), OP(N(TYPE), LST(OP(APLY(N(TYPE), N(VAR)), N(VAR))))),
            APLY(N(VAR), APLY(N(TYPE), N(VAR)))
        ))),
        OP(N(VAR), OP(N(TYPE), LST(APLY(N(VAR), APLY(N(TYPE), N(VAR)))))),
        CMD(APLY(N(VAR), N(FLT)))
    ))});
    te_f(h);
}

T(or) {
    te *h = ppnode(psr_r(bpsr(TPGM(or))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(INT)),
        OP(N(VAR), N(INT)),
        OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(VAR)), CMD(APLY(OP(N(NONE), N(NONE)), N(VAR), N(VAR))))
    ))});
    te_f(h);
}

T(un) {
    te *h = ppnode(psr_r(bpsr(TPGM(un))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), SYM(N(INT))),
        OP(N(VAR), SYM(N(FLT))),
        CMD(N(VAR)),
        CMD(SYM(N(VAR))),
        CMD(SYM(N(VAR)))
    ))});
    te_f(h);
}

T(unmatch) {
    te *h = ppnode(psr_r(bpsr(TPGM(unmatch))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), SYM(N(INT))),
        OP(N(VAR), SYM(N(INT))),
        OP(CMD(N(VAR)), OP(N(VAR), LST(
            SYM(APLY(N(STR), N(VAR))),
            SYM(APLY(N(STR), N(VAR))),
            N(STR)
        ))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(ns) {
    te *h = ppnode(psr_r(bpsr(TPGM(ns))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(STR)),
        OP(N(VAR), OP(N(NONE), OP(N(VAR), APLY(N(VAR), N(VAR))))),
        CMD(SYM(N(VAR)))
    ))});
    te_f(h);
}

T(iline) {
    te *h = ppnode(psr_r(bpsr(TPGM(iline))));
    V(h, {N(ROOT), APLY(LST(
        CMD(SYM(APLY(OP(N(NONE), N(NONE)), N(VAR), OP(N(NONE), APLY(N(VAR), N(STR))))))
    ))});
    te_f(h);
}

T(unsg) {
    te *h = ppnode(psr_r(bpsr(TPGM(unsg))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), SYM(N(INT))),
        OP(N(VAR), SYM(N(STR))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(vridx) {
    te *h = ppnode(psr_r(bpsr(TPGM(vridx))));
    V(h, {N(ROOT), APLY(LST(
        OP(CMD(N(VAR)), OP(N(TYPE), VEC(VEC(N(STR), N(STR)), VEC(N(STR), N(STR))))),
        CMD(N(VAR)),
        CMD(APLY(N(VAR), OP(N(NONE), N(INT)))),
        CMD(APLY(SYM(APLY(N(VAR), OP(N(NONE), N(INT)))), N(INT))),
        OP(APLY(N(VAR), N(INT)), LST(SYM(CMD(N(VAR)))))
    ))});
    te_f(h);
}

T(nscope) {
    te *h = ppnode(psr_r(bpsr(TPGM(nscope))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(FLT)),
        OP(N(VAR), OP(N(TYPE), LST(
            OP(N(VAR), OP(N(TYPE), LST(OP(N(VAR), N(FLT))))),
            APLY(N(VAR))
        ))),
        APLY(N(VAR)),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(cj) {
    te *h = ppnode(psr_r(bpsr(TPGM(cj))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(N(TYPE), LST(
            OP(N(VAR), N(INT)),
            OP(N(TYPE), LST(OP(N(VAR), N(INT))))
        ))),
        OP(N(VAR), APLY(N(VAR))),
        CMD(APLY(N(VAR))),
        CMD(APLY(N(VAR))),
        OP(CMD(N(VAR)), APLY(N(VAR)))
    ))});
    te_f(h);
}

T(leap) {
    te *h = ppnode(psr_r(bpsr(TPGM(leap))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), N(INT)),
        OP(CMD(N(VAR)), OP(APLY(N(TYPE), CMD(N(VAR))), N(VEC))),
        OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), LST(// @
                OP(APLY(OP(N(NONE), N(NONE)),
                    APLY(OP(N(NONE), N(NONE)),
                        OP(N(INT), OP(N(VAR), N(INT))),
                        OP(N(INT), OP(N(VAR), N(INT)))),
                        OP(N(INT), OP(N(VAR), N(INT)))),
                        OP(N(VAR), N(VAR))),
                OP(N(VAR), N(INT)))),
        CMD(N(VAR))
    ))});
    te_f(h);
}

T(vrup) {
    te *h = ppnode(psr_r(bpsr(TPGM(vrup))));
    V(h, {N(ROOT), APLY(LST(
        OP(N(VAR), OP(N(TYPE), VEC(N(INT), N(INT), N(INT)))),
        OP(N(VAR), OP(N(TYPE), LST(
            OP(APLY(APLY(APLY(N(TYPE), N(TYPE)), N(VAR)), N(INT)), N(INT)),
            CMD(N(VAR)),
            OP(APLY(N(VAR), N(INT)), N(INT)),
            SYM(N(INT))
        ))),
        OP(N(NONE), APLY(N(VAR), N(VAR)))
    ))});
    te_f(h);
}

T(stll) {
    te *h = ppnode(psr_r(bpsr(TPGM(stll))));
    V(h, {N(ROOT), APLY(LST(
        OP(CMD(N(VAR)), OP(
            APLY(N(TYPE), SYM(N(TYPE)), SYM(APLY(N(TYPE), SYM(N(TYPE)), SYM(CMD(N(VAR)))))),
            OP(N(NONE), LST(SYM(N(INT)), SYM(SYM(N(NONE))))))),
        CMD(N(VAR)),
        OP(N(VAR), OP(N(TYPE), LST(
            CMD(APLY(N(STR), APLY(N(TYPE), N(VAR)))),
            OP(N(VAR), OP(APLY(N(TYPE), SYM(CMD(N(VAR))), CMD(N(VAR))), LST(
                CMD(N(STR)),
                OP(SYM(N(VAR)), LST(SYM(APLY(N(VAR), N(VAR))), SYM(N(VAR)))),
                OP(SYM(SYM(APLY(N(VAR), N(VAR)))), OP(CMD(N(VAR)), OP(N(NONE), LST(
                    SYM(N(VAR)),
                    SYM(SYM(N(NONE)))
                ))))
            )))
        ))),
        OP(N(VAR), N(INT)),
        OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(INT)), APLY(N(VAR), OP(N(VAR), N(INT)))),
        CMD(N(VAR))
    ))});
    te_f(h);
}
