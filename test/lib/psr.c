
#include "psr_t.h"

I;

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

extern const alfr pm;

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

T(sigma, {
    te *h = ppnode(rpsr(ppsr(psr_b("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]"))));
    V(h, {N(ROOT), OP(N(INT), LST(N(INT), OP(N(FLT), LST(N(INT), N(INT), N(INT))), OP(N(INT), N(INT)), N(INT)))});
    te_f(h);
});

T(opfn, {
    te *h = ppnode(rpsr(psr_b("+(1;2)")));
    V(h, {N(ROOT), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT))});
    te_f(h);
});

T(nl, {
    te *h = ppnode(rpsr(psr_b("{\n3.2 - 2.1\n1 Σ [1;2;3]\n}")));
    V(h, {N(ROOT), LST(OP(N(FLT), N(FLT)), OP(N(INT), LST(N(INT), N(INT), N(INT))))});
    te_f(h);
});

T(type, {
    te *h = ppnode(rpsr(psr_b("FN(I6;I6;FN(I6;I6))")));
    V(h, {N(ROOT), APLY(N(TYPE), N(TYPE), N(TYPE), APLY(N(TYPE), N(TYPE), N(TYPE)))});
    te_f(h);
});

T(sym, {
    te *h = ppnode(rpsr(psr_b("`a`b`c")));
    V(h, {N(ROOT), SYM(SYM(SYM(N(NONE))))});
    te_f(h);
});

T(op_call, {
    te *h = ppnode(rpsr(psr_b("{a:+;a(3.14;9.8)}")));
    V(h, {N(ROOT), LST(OP(N(VAR), OP(N(NONE), N(NONE))), APLY(N(VAR), N(FLT), N(FLT)))});
    te_f(h);
});

T(inv, {
    te *h = ppnode(rpsr(psr_b("+(1;2")));
    A(h == NULL, "not null");
});

T(aply_prec, {
    te *h = ppnode(rpsr(psr_b("(1+3)")));
    V(h, {N(ROOT), APLY(N(NONE), OP(N(INT), N(INT)))});
    te_f(h);
});

T(sym_add, {
    te *h = ppnode(rpsr(psr_b("+`a")));
    V(h, {N(ROOT), SYM(OP(N(NONE), N(NONE)))});
    te_f(h);
});

T(sym_lst, {
    te *h = ppnode(rpsr(psr_b("[`a;`b;`c]")));
    V(h, {N(ROOT), LST(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE)))});
    te_f(h);
});

T(var_op_op_aply, {
    te *h = ppnode(rpsr(psr_b("x:+(1;2)")));
    V(h, {N(ROOT), OP(N(VAR), APLY(OP(N(NONE), N(NONE)), N(INT), N(INT)))});
    te_f(h);
});

T(neg, {
    te *h = ppnode(rpsr(psr_b("x:-1+2")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), OP(N(INT), N(INT))))});
    te_f(h);
});

T(op_op, {
    te *h = ppnode(rpsr(psr_b("{-+4;-(+4)}")));
    V(h, {N(ROOT), LST(OP(N(NONE), OP(N(NONE), N(INT))), APLY(OP(N(NONE), N(NONE)), OP(N(NONE), N(INT))))});
    te_f(h);
});

T(fn, {
    te *h = ppnode(rpsr(psr_b("f:FN(I6`a;I6`b;I6`c;I6)${-a+b+c}")));
    V(h, {N(ROOT), OP(N(VAR), OP(APLY(N(TYPE), SYM(N(TYPE)), SYM(N(TYPE)), SYM(N(TYPE)), N(TYPE)), LST(OP(N(NONE), OP(N(VAR), OP(N(VAR), N(VAR)))))))});
    te_f(h);
});

T(prec, {
    te *h = ppnode(rpsr(psr_b("a:-(w) + -(x;y) + -z")));
    V(h, {N(ROOT), OP(N(VAR), OP(APLY(OP(N(NONE), N(NONE)), N(VAR)), OP(APLY(OP(N(NONE), N(NONE)), N(VAR), N(VAR)), OP(N(NONE), N(VAR)))))});
    te_f(h);
});

T(call_call_call, {
    te *h = ppnode(rpsr(psr_b("a(1;2)(3.3)()")));
    V(h, {N(ROOT), APLY(APLY(APLY(N(VAR), N(INT), N(INT)), N(FLT)))});
    te_f(h);
});

T(defer_hsh, {
    te *h = ppnode(rpsr(psr_b("d:#{1`x;(1+5-4)`y;3`z}")));
    V(h, {N(ROOT), OP(N(VAR), OP(N(NONE), LST(SYM(N(INT)), SYM(APLY(N(NONE), OP(N(INT), OP(N(INT), N(INT))))), SYM(N(INT)))))});
    te_f(h);
});

T(value_hsh, {
    te *h = ppnode(rpsr(psr_b("[`a;`b;`c]#[1;2;3]")));
    V(h, {N(ROOT), OP(LST(SYM(N(NONE)), SYM(N(NONE)), SYM(N(NONE))), LST(N(INT), N(INT), N(INT)))});
    te_f(h);
});

T(if_else_else, {
    te *h = ppnode(rpsr(psr_b("?{a?(c:2;a?c:1;c);(b+a)?2;3}")));
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

/*
T(psr, {
    te_f(ppnode(rpsr(psr_b("{d:UN(I6`v;FN(I6)`e)$5`v;d?{d`v`v;0`e;2}}"))));
    te_f(ppnode(rpsr(psr_b("{a:1;b:2;{a+b+c}(3`c)}"))));
    te_f(ppnode(rpsr(psr_b("{f:{a:2:x+y()};y:{a};f(1`x)}"))));
});
*/

T(add_flt_fn, {
    te *h = ppnode(rpsr(psr_b("{f:FN(F6`x;F6`y;F6)${x-y};f(1.23;4.56)}")));
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
    te *h = ppnode(rpsr(psr_b("$(FN(U6`n;U6);{?{(n=0)?0;(n<3)?2;S(n-1)+S(n-2)}})(35)")));
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
    te *h = ppnode(rpsr(psr_b("$(FN(U6`m;U6`n;U6);{?{?(m=0;n+1);(&(m>0;n=0))?S(m-1;1);(m>0;n>0)?S(m-1;S(m;n-1));n+1}})(3;10)")));
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
