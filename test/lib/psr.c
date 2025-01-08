
#include "psr_t.h"

static psr *ppsr(psr *p) {
    tkn_p(p->tt->t, 0);
    psr_p(p->pt, 0);
    return p;
}

static te *rpsr(psr *p) {
    psr_stat pstat;
    te *nh = te_i(3, &malloc, &node_f);
    if ((pstat = psr_n(p, nh)) != PSR_STAT(END)) exit(pstat);
    te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
    nh->d[0] = P(p);
    nh->d[1] = U6(NODE_TYPE(ROOT));
    nh->d[2] = P(n);
    n->d[0] = P(nh);
    return nh;
}

static te *ppnode(te *h) {
    printf("%s\n", ((psr*) h->d[0].p)->tt->s->d);
    node_p(h->d[2].p, 0);
    putchar('\n');
    return h;
}

int main(void) {
    te_f(ppnode(rpsr(ppsr(psr_b("0 Σ [12;5.4 Σ [1;2;3];5 - 4;15]")))));
    te_f(ppnode(rpsr(psr_b("{\n3.2 - 2.1\n1 Σ [1;2;3]\n}"))));
    te_f(ppnode(rpsr(psr_b("{a;b;c}#{a+b+c}"))));
    return 0;
}
