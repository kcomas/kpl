
#include "../../src/psr.h"
#include "t.h"

I;

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

const alfr pm = { .a = &malloc, .f = &free };

static tbl *mktbl(void) {
    lst *tl = lst_i(&pm, &pm, (void*) &te_f);
    te *b = te_i(10, &pm, NULL);
    tbl *t = tbl_i(&pm, &tkn_sh, &c4_eq, tl, b);
    return t;
}

static psr *bpsr(const char *pgm) {
    tkn *t = tkn_i(&pm, &pm, &tkn_entry_f, &mktbl, &tkn_df, mc_i_cstr(pgm, &pm));
    tkn_b(t);
    vr *v = vr_i(10, &pm, (void*) &te_f);
    psr *p = psr_i(&pm, &pm, &pm, &psr_entry_f, &mktbl, t, v);
    return psr_b(p);
}

static te *rpsr(psr *p) {
    psr_stat pstat;
    te *nh = te_i(3, &pm, &node_f);
    if ((pstat = psr_n(p, nh)) != PSR_STAT(END)) {
        printf("%s|", p->tt->s->d);
        printf("PSTAT: %u|", pstat);
        printf("lno:%lu,cno:%lu\n", p->tt->lno, p->tt->cno);
        te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
        te_f(nh);
        te_f(n);
        psr_f(p);
        return NULL;
    }
    te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
    nh->d[0] = P(p);
    nh->d[1] = U6(NODE_TYPE(ROOT));
    nh->d[2] = P(n);
    n->d[0] = P(nh);
    return nh;
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
        case NODE_TYPE(FLT):
            VN(v, i, vl, FLT);
            break;
        case NODE_TYPE(OP):
            VN(v, i, vl, OP);
            VV(n->d[3].p, v, i, vl);
            VV(n->d[4].p, v, i, vl);
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
        default:
            A(0, "NODE");
            break;
    }
}

void psr_verify_lst(_tests *_t, const lst *l, const node_id v[], size_t *i, size_t vl) {
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

#define LST(...) N(LST), __VA_ARGS__

#define APLY(...) N(APLY), __VA_ARGS__

#define SYM(TGT) N(SYM), TGT

T(sigma, {
    te *h = ppnode(rpsr(ppsr(bpsr("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]"))));
    V(h, {N(ROOT), OP(N(INT), LST(N(INT), OP(N(FLT), LST(N(INT), N(INT), N(INT))), OP(N(INT), N(INT)), N(INT)))});
    te_f(h);
});

T(opfn, {
    te *h = ppnode(rpsr(bpsr("+(1;2)")));
    V(h, {N(ROOT), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT))});
    te_f(h);
});

T(nl, {
    te *h = ppnode(rpsr(bpsr("{\n3.2 - 2.1\n1 Σ [1;2;3]\n}")));
    V(h, {N(ROOT), LST(OP(N(FLT), N(FLT)), OP(N(INT), LST(N(INT), N(INT), N(INT))))});
    te_f(h);
});

T(type, {
    te *h = ppnode(rpsr(bpsr("FN(I6;I6;FN(I6;I6))")));
    V(h, {N(ROOT), APLY(N(TYPE), N(TYPE), N(TYPE), APLY(N(TYPE), N(TYPE), N(TYPE)))});
    te_f(h);
});

T(sym, {
    te *h = ppnode(rpsr(bpsr("`a`b`c")));
    V(h, {N(ROOT), SYM(SYM(SYM(N(NONE))))});
    te_f(h);
});

T(op_call, {
    te *h = ppnode(rpsr(bpsr("{a:+;a(3.14;9.8)}")));
    V(h, {N(ROOT), LST(OP(N(VAR), OP(N(NONE), N(NONE))), APLY(N(VAR), N(FLT), N(FLT)))});
    te_f(h);
});

T(inv, {
    te *h = ppnode(rpsr(bpsr("+(1;2")));
    A(h == NULL, "not null");
});

T(aply_prec, {
    te *h = ppnode(rpsr(bpsr("(1+3)")));
    V(h, {N(ROOT), APLY(N(NONE), OP(N(INT), N(INT)))});
    te_f(h);
});

T(sym_add, {
    te *h = ppnode(rpsr(bpsr("+`a")));
    V(h, {N(ROOT), SYM(OP(N(NONE), N(NONE)))});
    te_f(h);
});

T(sym_lst, {
    te *h = ppnode(rpsr(bpsr("[`a;`b;`c]")));
    V(h, {N(ROOT), LST(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE)))});
    te_f(h);
});

T(var_op_op_aply, {
    te *h = ppnode(rpsr(bpsr("x:+(1;2)")));
    V(h, {N(ROOT), OP(N(VAR), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT)))});
    te_f(h);
});

T(neg, {
    te *h = ppnode(rpsr(bpsr("x:-1+2")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), OP(N(INT), N(INT))))});
    te_f(h);
});

T(op_op, {
    te *h = ppnode(rpsr(bpsr("{-+4;-(+4)}")));
    V(h, {N(ROOT), LST(OP(N(NONE), OP(N(NONE), N(INT))), APLY(OP(N(NONE), N(NONE)), OP(N(NONE), N(INT))))});
    te_f(h);
});

T(fn, {
    te *h = ppnode(rpsr(bpsr("f:FN(I6`a;I6`b;I6`c;I6)${-a+b+c}")));
    V(h, {N(ROOT), OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)), LST(OP(N(NONE), OP(N(VAR), OP(N(VAR), N(VAR)))))))});
    te_f(h);
});

T(prec, {
    te *h = ppnode(rpsr(bpsr("a:-(w) + -(x;y) + -z")));
    V(h, {N(ROOT), OP(N(VAR), OP(APLY(OP(N(NONE), N(NONE)), N(VAR)), OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(VAR)), OP(N(NONE), N(VAR)))))});
    te_f(h);
});

T(call_call_call, {
    te *h = ppnode(rpsr(bpsr("a(1;2)(3.3)()")));
    V(h, {N(ROOT), APLY(APLY(APLY(N(VAR), N(INT), N(INT)), N(FLT)))});
    te_f(h);
});

T(defer_hsh, {
    te *h = ppnode(rpsr(bpsr("d:#{1`x;(1+5-4)`y;3`z}")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), LST(SYM(N(INT)), SYM(APLY(N(NONE), OP(N(INT), OP(N(INT), N(INT))))), SYM(N(INT)))))});
    te_f(h);
});

T(value_hsh, {
    te *h = ppnode(rpsr(bpsr("[`a;`b;`c]#[1;2;3]")));
    V(h, {N(ROOT), OP(LST(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE))), LST(N(INT), N(INT), N(INT)))});
    te_f(h);
});

T(if_else_else, {
    te *h = ppnode(rpsr(bpsr("?{a?(c:2;a?c:1;c);(b+a)?2;3}")));
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
});

T(match, {
    te *h = ppnode(rpsr(bpsr("{d:UN(I6`v;FN(I6)`e)$5`v;d?{d`v`v;0`e;2}}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR),
            OP(APLY(N(TYPE),
                    SYM(N(TYPE)), SYM(APLY(N(TYPE), N(TYPE)))),
               SYM(N(INT)))),
        OP(N(VAR), LST(SYM(SYM(N(VAR))), SYM(N(INT)), N(INT)))
    )});
    te_f(h);
});

T(defer_call, {
    te *h = ppnode(rpsr(bpsr("{a:1;b:2;{a+b+c}(3`c)}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR), N(INT)),
        OP(N(VAR), N(INT)),
        APLY(LST(OP(N(VAR), OP(N(VAR), N(VAR)))), SYM(N(INT)))
    )});
    te_f(h);
});

T(nested_defer, {
    te *h = ppnode(rpsr(bpsr("{f:{a:2;x+y()};y:{a};f(1`x)}")));
    V(h, {N(ROOT), LST(
        OP(N(VAR), // f:
            LST(
                OP(N(VAR), N(INT)), // a:2
                OP(N(VAR), APLY(N(VAR))))), // x + y()
        OP(N(VAR), LST(N(VAR))), // y:{a}
        APLY(N(VAR), SYM(N(INT))) // f(1`x)
    )});
    te_f(h);
});

T(add_flt_fn, {
    te *h = ppnode(rpsr(bpsr("{f:FN(F6`x;F6`y;F6)${x-y};f(1.23;4.56)}")));
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
});

T(fib, {
    te *h = ppnode(rpsr(bpsr("$(FN(U6`n;U6);{?{(n=0)?0;(n<3)?2;S(n-1)+S(n-2)}})(35)")));
    V(h, {N(ROOT),
        APLY(APLY(OP(N(NONE), N(NONE)),
                APLY(N(TYPE), SYM(N(TYPE)), N(TYPE)),
                LST(OP(N(NONE),
                    LST(
                        OP(APLY(N(NONE), OP(N(VAR), N(INT))), N(INT)),
                        OP(APLY(N(NONE), OP(N(VAR), N(INT))), N(INT)),
                        OP(
                            APLY(N(KEY), OP(N(VAR), N(INT))),
                            APLY(N(KEY), OP(N(VAR), N(INT)))
                        )
                    )
                ))
            ), N(INT))
    });
    te_f(h);
});

T(ack, {
    te *h = ppnode(rpsr(bpsr("$(FN(U6`m;U6`n;U6);{?{?(m=0;n+1);(&(m>0;n=0))?S(m-1;1);(m>0;n>0)?S(m-1;S(m;n-1));n+1}})(3;10)")));
    V(h, {N(ROOT),
        APLY(APLY(OP(N(NONE), N(NONE)),
                APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)),
                LST(OP(N(NONE),
                    LST(
                        APLY(OP(N(NONE), N(NONE)),
                            OP(N(VAR), N(INT)),
                            OP(N(VAR), N(INT))
                        ),
                        OP(
                            APLY(N(NONE),
                            APLY(OP(N(NONE), N(NONE)),
                                OP(N(VAR), N(INT)),
                                OP(N(VAR), N(INT)))),
                            APLY(N(KEY), OP(N(VAR), N(INT)), N(INT))
                        ),
                        OP(
                            APLY(N(NONE),
                                OP(N(VAR), N(INT)), OP(N(VAR), N(INT))),
                            APLY(N(KEY), OP(N(VAR), N(INT)),
                                APLY(N(KEY), N(VAR), OP(N(VAR), N(INT))))
                        ),
                        OP(N(VAR), N(INT)))
                ))), N(INT), N(INT))});
    te_f(h);
});
