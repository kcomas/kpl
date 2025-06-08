
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
        psr_f(p);
        nh->d[1] = P(NULL);
        te_f(nh);
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
    te_f(ppnode(rpsr(psr_b("+(1;2"))));
    te_f(ppnode(rpsr(psr_b("f:FN(I6;I6;I6;I6)${a;b;c}#{a+b+c}"))));
    return 0;
}
