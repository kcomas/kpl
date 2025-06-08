
#include "psr_t.h"

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

static te *rpsr(psr *p) {
    psr_stat pstat;
    te *nh = te_i(3, &malloc, &node_f);
    if ((pstat = psr_n(p, nh)) != PSR_STAT(END)) {
        printf("%s|", p->tt->s->d);
        printf("PSTAT: %u|", pstat);
        printf("lno:%lu,cno:%lu\n", p->tt->lno, p->tt->cno);
        te_f(nh);
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

int main(void) {
    te_f(ppnode(rpsr(ppsr(psr_b("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]")))));
    te_f(ppnode(rpsr(psr_b("{\n3.2 - 2.1\n1 Σ [1;2;3]\n}"))));
    te_f(ppnode(rpsr(psr_b("FN(I6;I6;FN(I6;I6))"))));
    te_f(ppnode(rpsr(psr_b("+(1;2)"))));
    te_f(ppnode(rpsr(psr_b("`a`b`c"))));
    te_f(ppnode(rpsr(psr_b("{a:+;a(3.14;9.8)}"))));
    te_f(ppnode(rpsr(psr_b("(1+3)"))));
    te_f(ppnode(rpsr(psr_b("+`a"))));
    te_f(ppnode(rpsr(psr_b("[`a;`b;`c]"))));
    te_f(ppnode(rpsr(psr_b("x:+(1;2)"))));
    te_f(ppnode(rpsr(psr_b("x:-1+2"))));
    te_f(ppnode(rpsr(psr_b("{-+4;-(+4)}"))));
    //te_f(ppnode(rpsr(psr_b("+(1;2"))));
    te_f(ppnode(rpsr(psr_b("f:FN(I6`a;I6`b;I6`c;I6)${-a+b+c}"))));
    te_f(ppnode(rpsr(psr_b("a:-(w) + -(x;y) + -z"))));
    te_f(ppnode(rpsr(psr_b("a(1;2)(3)()"))));
    te_f(ppnode(rpsr(psr_b("d:#!{1`x;(1+5-4)`y;3`z}"))));
    te_f(ppnode(rpsr(psr_b("[`a;`b;`c]#![1;2;3]"))));
    te_f(ppnode(rpsr(psr_b("?{a?(c:2;a?c:1;c);(b+a)?2;3}"))));
    te_f(ppnode(rpsr(psr_b("{d:UN(I6`v;FN(I6)`e)$5`v;d?{d`v`v;0`e;2}}"))));
    te_f(ppnode(rpsr(psr_b("{a:1;b:2;{a+b+c}(3`c)}"))));
    te_f(ppnode(rpsr(psr_b("{f:FN(F6`x;F6`y;F6)${x-y};f(1.23;4.56)}"))));
    te_f(ppnode(rpsr(psr_b("{f:{a:2:x+y()};y:{a};f(1`x)}"))));
    te_f(ppnode(rpsr(psr_b("FN(U6`n;U6)${?{(n=0)?0;(n<3)?2;S(n-1)+S(n-2)}}(35)"))));
    te_f(ppnode(rpsr(psr_b("FN(U6`m;U6`n;U6)${?{?(m=0;n+1);(&(m>0;n=0))?S(m-1;1);(m>0;n>0)?S(m-1;S(m;n-1));n+1}}(3;10)"))));
    return 0;
}
