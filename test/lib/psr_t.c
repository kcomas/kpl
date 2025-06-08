
#include "psr_t.h"

void psr_p(tbl *t, size_t idnt) {
    te *h = t->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        printf("tid:%lu,pid:%lu,mode:%lu\n", kv->d[0].u6, kv->d[1].u6, kv->d[2].u6);
        psr_p(kv->d[7].p, idnt + 1);
        h = h->d[2].p;
    }
}

tbl *psr_mktbl(void) {
    lst *tl = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &tkn_sh, &c4_eq, tl, b);
    return t;
}

void psr_entry_free(void *p) {
    te *t = (te*) p;
    te_f(t->d[3].p);
    tbl_f(t->d[7].p);
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

psr_stat psr_int_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(INT), 2);
    return PSR_STAT(OK);
}

psr_stat psr_flt_i(psr *const p, te **n) {
    *n = te_i(4, p->pa, &node_f);
    (*n)->d[0] = U6(NODE_TYPE(FLT));
    un m;
    for (size_t i = 3; i > 0; i--) {
        vr_sb(p->ts, &m);
        (*n)->d[i] = P(m.p);
    }
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

psr_stat psr_lst_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(LST), 3);
    (*n)->d[2].p = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    return PSR_STAT(OK);
}

psr_stat psr_lst_e(psr *const p, te *const e, te *const n) {
    (void) p;
    lst_ab(e->d[2].p, P(n));
    return PSR_STAT(OK);
}

void node_p(const te *const n, const mc *const s, size_t idnt) {
    te *h;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    printf("(type:%lu", n->d[0].u6);
    switch (n->d[0].u6) {
        case NODE_TYPE(INT):
            putchar('|');
            tkn_m_p(n->d[1].p, s);
            printf("|)");
            break;
        case NODE_TYPE(FLT):
            putchar('|');
            tkn_m_p(n->d[1].p, s);
            putchar(',');
            tkn_m_p(n->d[2].p, s);
            putchar(',');
            tkn_m_p(n->d[3].p, s);
            printf("|)");
            break;
        case NODE_TYPE(OP):
            putchar('|');
            tkn_m_p(n->d[1].p, s);
            printf("|\n");
            node_p(n->d[2].p, s, idnt + 1);
            putchar('\n');
            node_p(n->d[3].p, s, idnt + 1);
            putchar(')');
            break;
        case NODE_TYPE(LST):
            putchar('|');
            tkn_m_p(n->d[1].p, s);
            printf("|\n");
            h = ((lst*) n->d[2].p)->h;
            while (h) {
                node_p(h->d[0].p, s, idnt + 1);
                h = h->d[2].p;
                if (h) putchar('\n');
            }
            putchar(')');
            break;
    }
}

void node_f(void *p) {
    te *n = (te*) p;
    switch (n->d[0].u6) {
        case NODE_TYPE(INT):
            te_f(n->d[1].p);
            break;
        case NODE_TYPE(FLT):
            te_f(n->d[1].p);
            te_f(n->d[2].p);
            te_f(n->d[3].p);
            break;
        case NODE_TYPE(OP):
            te_f(n->d[1].p);
            node_f(n->d[2].p);
            node_f(n->d[3].p);
            break;
        case NODE_TYPE(LST):
            te_f(n->d[1].p);
            lst_f(n->d[2].p);
            break;
    }
    free(n);
}
