
#include "psr_t.h"

tbl *psr_mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tkn_sh, &c4_eq, tl, b);
    return t;
}

void psr_entry_free(void *p) {
    te *t = (te*) p;
    te_f(t->d[3].p);
    tbl_f(t->d[6].p);
    free(t);
}

static te *node_i(psr *const p, node_type nt, size_t size) {
    te *n = te_i(size, p->pa, &node_f);
    n->d[0] = U6(nt);
    un m;
    vr_sb(p->ts, &m);
    n->d[1] = P(m.p);
    return n;
}

psr_stat psr_val_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(VAL), 2);
    return PSR_STAT(OK);
}

psr_stat psr_val_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if (nh->d[1].p) ((te*) nh->d[1].p)->d[3].p = n;
    else if (nh->d[2].p) return PSR_STAT(INV);
    else nh->d[2].p = n;
    return PSR_STAT(OK);
}

psr_stat psr_op_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(OP), 4);
    return PSR_STAT(OK);
}

psr_stat psr_op_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if (!nh->d[0].p) nh->d[0].p = n;
    if (nh->d[1].p && nh->d[2].p) return PSR_STAT(INV);
    if (nh->d[2].p) {
        n->d[2].p = nh->d[2].p;
        nh->d[2].p = NULL;
    } else {
        n->d[2].p = ((te*) nh->d[1].p)->d[3].p;
        ((te*) nh->d[1].p)->d[3].p = n;
    }
    nh->d[1].p = n;
    return PSR_STAT(OK);
}

void node_p(const te *const n, const mc *const s, size_t idnt) {
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    printf("type:%lu,", n->d[0].u6);
    tkn_m_p(n->d[1].p, s);
    putchar('\n');
    switch (n->d[0].u6) {
        case NODE_TYPE(VAL):
            break;
        case NODE_TYPE(OP):
            node_p(n->d[2].p, s, idnt + 1);
            node_p(n->d[3].p, s, idnt + 1);
            break;
    }
}

void node_f(void *p) {
    te *n = (te*) p;
    switch (n->d[0].u6) {
        case NODE_TYPE(VAL):
            break;
        case NODE_TYPE(OP):
            node_f(n->d[2].p);
            node_f(n->d[3].p);
            break;
    }
    te_f(n->d[1].p);
    free(n);
}
