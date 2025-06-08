
#include "psr_t.h"

psr *psr_b(const char *const pgm) {
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_f, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    vr *v = vr_i(10, &malloc, (void*) &te_f, &free);
    psr *p = psr_i(&malloc, &free, &psr_entry_f, &psr_mktbl, t, v);
    te *lst_stp = te_i(1, &malloc, &free);
    lst_stp->d[0] = U6(tkn_a(t, TCUST(RS), "]", &tkn_ft));
    te *blk_stp = te_i(1, &malloc, &free);
    blk_stp->d[0] = U6(tkn_a(t, TCUST(RB), "}", &tkn_ft));
    te *aply_stp = te_i(1, &malloc, &free);
    aply_stp->d[0] = U6(tkn_a(t, TCUST(RP), ")", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, tkn_a(t, TCUST(SEMI), ";", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, tkn_a(t, TCUST(NL), "\n", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RS));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RB));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RP));
    psr_a(p, PCUST(INT), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_int_i, 1, TCUST(NUM));
    psr_a(p, PCUST(VAR), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_var_i, 1, TCUST(VAR));
    psr_a(p, PCUST(TYPE), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_type_i, 1, tkn_a(t, TOKEN(UN), "I6", &tkn_ft));
    psr_a(p, PCUST(TYPE), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_type_i, 1, tkn_a(t, TOKEN(UN), "F6", &tkn_ft));
    psr_a(p, PCUST(TYPE), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_type_i, 1, tkn_a(t, TOKEN(UN), "FN", &tkn_ft));
    psr_a(p, PCUST(TYPE), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_type_i, 1, tkn_a(t, TOKEN(UN), "DL", &tkn_ft));
    psr_a(p, PCUST(TYPE), PSR_MODE(ONCE), NULL, NULL, &psr_val_m, &psr_type_i, 1, tkn_a(t, TOKEN(UN), "UN", &tkn_ft));
    psr_a(p, PCUST(SUM), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "Σ", &tkn_ft));
    psr_a(p, PCUST(SUB), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "-", &tkn_ft));
    psr_a(p, PCUST(ADD), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "+", &tkn_ft));
    psr_a(p, PCUST(EFN), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "##", &tkn_ft));
    psr_a(p, PCUST(MH), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "#!", &tkn_ft));
    psr_a(p, PCUST(AGN), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), ":", &tkn_ft));
    psr_a(p, PCUST(CST), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "$", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, TCUST(WS));
    psr_a(p, PCUST(FLT), PSR_MODE(ONCE), NULL, NULL,  &psr_val_m, &psr_flt_i, 3, TCUST(NUM), tkn_a(t, TOKEN(UN), ".", &tkn_ft), TCUST(NUM));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), lst_stp, &psr_lst_e, &psr_val_m, &psr_lst_i, 1, tkn_a(t, TCUST(LS), "[", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), blk_stp, &psr_lst_e, &psr_val_m, &psr_lst_i, 1, tkn_a(t, TCUST(LB), "{", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), aply_stp, &psr_aply_e, &psr_aply_m, &psr_aply_i, 1, tkn_a(t, TCUST(LP), "(", &tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, &psr_sym_m, &psr_sym_i, 1, TCUST(SYM));
    psr_a(p, PCUST(IF), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "#?", &tkn_ft));
    psr_a(p, PCUST(MTCH), PSR_MODE(ONCE), NULL, NULL, &psr_op_m, &psr_op_i, 1, tkn_a(t, TOKEN(UN), "#=", &tkn_ft));
    return p;
}

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

void psr_entry_f(void *p) {
    te *t = (te*) p;
    te_f(t->d[3].p);
    tbl_f(t->d[7].p);
    free(t);
}

static te *node_i(psr *const p, node_type nt, size_t size) {
    te *n = te_i(size, p->pa, &node_f);
    n->d[1] = U6(nt);
    un m;
    vr_sb(p->ts, &m);
    n->d[2] = P(m.p);
    return n;
}

psr_stat psr_var_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(VAR), 3);
    return PSR_STAT(OK);
}

psr_stat psr_type_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(TYPE), 3);
    return PSR_STAT(OK);
}

psr_stat psr_int_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(INT), 3);
    return PSR_STAT(OK);
}

psr_stat psr_flt_i(psr *const p, te **n) {
    *n = te_i(5, p->pa, &node_f);
    (*n)->d[1] = U6(NODE_TYPE(FLT));
    un m;
    for (size_t i = 4; i > 1; i--) {
        vr_sb(p->ts, &m);
        (*n)->d[i] = P(m.p);
    }
    return PSR_STAT(OK);
}

psr_stat psr_val_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if (nh->d[1].p) {
        ((te*) nh->d[1].p)->d[4] = P(n);
        n->d[0] = nh->d[1];
    }
    else if (nh->d[2].p) return PSR_STAT(INV);
    else nh->d[2] = P(n);
    return PSR_STAT(OK);
}

psr_stat psr_op_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(OP), 5);
    return PSR_STAT(OK);
}

psr_stat psr_op_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if (nh->d[1].p && nh->d[2].p) return PSR_STAT(INV);
    if (!nh->d[1].p && !nh->d[2].p) {
        nh->d[0] = nh->d[1] = P(n);
        return PSR_STAT(OK);
    }
    if (!nh->d[0].p) nh->d[0] = P(n);
    if (nh->d[2].p) {
        n->d[3] = nh->d[2];
        ((te*) n->d[3].p)->d[0] = P(n);
        nh->d[2] = P(NULL);
    } else {
        n->d[3] = ((te*) nh->d[1].p)->d[4];
        ((te*) nh->d[1].p)->d[4] = P(n);
        n->d[0] = nh->d[1];
    }
    nh->d[1].p = n;
    return PSR_STAT(OK);
}

psr_stat psr_lst_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(LST), 4);
    (*n)->d[3].p = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    return PSR_STAT(OK);
}

psr_stat psr_lst_e(psr *const p, te *const e, te *const n) {
    (void) p;
    n->d[0] = P(e);
    lst_ab(e->d[3].p, P(n));
    return PSR_STAT(OK);
}

psr_stat psr_aply_i(psr *const p, te **n) {
    (void) p;
    *n = node_i(p, NODE_TYPE(APLY), 5);
    (*n)->d[4].p = lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
    return PSR_STAT(OK);
}

psr_stat psr_aply_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if ((nh->d[1].p && nh->d[2].p) || (!nh->d[1].p && !nh->d[2].p)) return PSR_STAT(INV);
    if (nh->d[1].p && ((te*) nh->d[1].p)->d[3].p && ((te*) nh->d[1].p)->d[4].p) {
        n->d[3] = ((te*) nh->d[1].p)->d[4];
        ((te*) nh->d[1].p)->d[4] = P(n);
        n->d[0] = nh->d[1];
    } else if (nh->d[1].p && !((te*) nh->d[1].p)->d[3].p && ((te*) nh->d[1].p)->d[4].p) {
        n->d[0] = ((te*) ((te*) nh->d[1].p)->d[4].p)->d[0];
        n->d[3] = ((te*) nh->d[1].p)->d[4];
        ((te*) nh->d[1].p)->d[4] = ((te*) ((te*) nh->d[1].p)->d[4].p)->d[0] = P(n);
    } else if (nh->d[1].p) {
        if (((te*) nh->d[1].p)->d[3].p || ((te*) nh->d[1].p)->d[4].p) return PSR_STAT(INV);
        n->d[3] = nh->d[1];
        if (((te*) nh->d[1].p)->d[0].p) {
            nh->d[1] = n->d[0] = ((te*) nh->d[1].p)->d[0];
            ((te*) n->d[3].p)->d[0] = ((te*) n->d[0].p)->d[4] = P(n);
        } else {
            ((te*) nh->d[1].p)->d[0] = nh->d[2] = P(n);
            nh->d[0] = nh->d[1] = P(NULL);
        }
    } else {
        n->d[0] = ((te*) nh->d[2].p)->d[0];
        ((te*) nh->d[2].p)->d[0] = P(n);
        n->d[3] = nh->d[2];
        nh->d[1] = P(NULL);
        nh->d[2] = P(n);
    }
    return PSR_STAT(OK);
}

psr_stat psr_aply_e(psr *const p, te *const e, te *const n) {
    (void) p;
    n->d[0] = P(e);
    lst_ab(e->d[4].p, P(n));
    return PSR_STAT(OK);
}

psr_stat psr_sym_i(psr *const p, te **n) {
    *n = node_i(p, NODE_TYPE(SYM), 4);
    return PSR_STAT(OK);
}

psr_stat psr_sym_m(psr *const p, te *const nh, te *const n) {
    (void) p;
    if ((nh->d[1].p && nh->d[2].p) || (nh->d[1].p && !((te*) nh->d[1].p)->d[4].p)) return PSR_STAT(INV);
    if (!nh->d[1].p && !nh->d[2].p) {
        nh->d[2].p = n;
        return PSR_STAT(OK);
    }
    if (nh->d[1].p) {
        n->d[3] = ((te*) nh->d[1].p)->d[4];
        ((te*) nh->d[1].p)->d[4] = P(n);
        n->d[0] = nh->d[1];
    } else {
        n->d[3] = nh->d[2];
        ((te*) n->d[3].p)->d[0] = P(n);
        nh->d[2] = P(n);
    }
    return PSR_STAT(OK);
}

static const mc *node_root_mc(const te *const n) {
    if (n->d[1].u6 != NODE_TYPE(ROOT)) return node_root_mc(n->d[0].p);
    return ((psr*) n->d[0].p)->tt->s;
}

void node_p(const te *const n, size_t idnt) {
    te *h;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    printf("(type:%lu", n->d[1].u6);
    switch (n->d[1].u6) {
        case NODE_TYPE(ROOT):
            break;
        case NODE_TYPE(TYPE):
        case NODE_TYPE(VAR):
        case NODE_TYPE(INT):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            printf("|)");
            break;
        case NODE_TYPE(FLT):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            putchar(',');
            tkn_m_p(n->d[3].p, node_root_mc(n->d[0].p));
            putchar(',');
            tkn_m_p(n->d[4].p, node_root_mc(n->d[0].p));
            printf("|)");
            break;
        case NODE_TYPE(OP):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            putchar('|');
            if (n->d[4].p) putchar('\n');
            if (n->d[3].p) {
                node_p(n->d[3].p, idnt + 1);
                putchar('\n');
            }
            if (n->d[4].p) node_p(n->d[4].p, idnt + 1);
            putchar(')');
            break;
        case NODE_TYPE(LST):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            printf("|\n");
            h = ((lst*) n->d[3].p)->h;
            while (h) {
                node_p(h->d[0].p, idnt + 1);
                h = h->d[2].p;
                if (h) putchar('\n');
            }
            putchar(')');
            break;
       case NODE_TYPE(APLY):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            printf("|\n");
            node_p(n->d[3].p, idnt + 1);
            h = ((lst*) n->d[4].p)->h;
            if (h) putchar('\n');
            while (h) {
                node_p(h->d[0].p, idnt + 1);
                h = h->d[2].p;
                if (h) putchar('\n');
            }
            putchar(')');
            break;
       case NODE_TYPE(SYM):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            printf("|");
            if (n->d[3].p) {
                putchar('\n');
                node_p(n->d[3].p, idnt + 1);
            }
            putchar(')');
            break;
    }
}

void node_f(void *p) {
    if (!p) return;
    te *n = (te*) p;
    te_f(n->d[2].p);
    switch (n->d[1].u6) {
        case NODE_TYPE(ROOT):
            psr_f(n->d[0].p);
            break;
        case NODE_TYPE(TYPE):
        case NODE_TYPE(VAR):
        case NODE_TYPE(INT):
            break;
        case NODE_TYPE(FLT):
            te_f(n->d[3].p);
            te_f(n->d[4].p);
            break;
        case NODE_TYPE(OP):
            node_f(n->d[3].p);
            node_f(n->d[4].p);
            break;
        case NODE_TYPE(LST):
            lst_f(n->d[3].p);
            break;
        case NODE_TYPE(APLY):
            node_f(n->d[3].p);
            lst_f(n->d[4].p);
            break;
        case NODE_TYPE(SYM):
            node_f(n->d[3].p);
            break;
    }
    free(n);
}
