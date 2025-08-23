
#include "psr.h"

void psr_rn_f(void *p) {
    te *n = p;
    psr_f(n->d[0].p);
    te_f(n->d[2].p);
    n->af->f(n);
}

te *psr_g_rn(psr *p, te *nh) {
    nh->tf = psr_rn_f;
    te *n = nh->d[0].p ? nh->d[0].p : nh->d[2].p;
    nh->d[0] = P(p);
    nh->d[1] = U6(NODE_TYPE(ROOT));
    nh->d[2] = P(n);
    n->d[0] = P(nh);
    return nh;
}

void psr_n_err_f(void *d) {
    te *nh = d;
    if (!nh->d[0].p && !nh->d[2].p) te_f(nh->d[1].p);
    else {
        if (nh->d[0].p == nh->d[2].p) nh->d[2] = P(NULL);
        te_f(nh->d[0].p);
        te_f(nh->d[2].p);
    }
    nh->af->f(nh);
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

void psr_entry_f(void *p) {
    te *t = p;
    te_f(t->d[3].p);
    tbl_f(t->d[7].p);
    t->af->f(t);
}

typedef void node_f(te *n);

static te *node_i(psr *p, node_id nt, size_t size, node_f f) {
    te *n = te_i(size, p->ta, (void*) f);
    n->d[1] = U6(nt);
    un m;
    vr_sb(p->ts, &m);
    n->d[2] = P(m.p);
    return n;
}

static void p_vtkris_f(te *n) {
    te_f(n->d[2].p);
    n->af->f(n);
}

psr_stat psr_var_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(VAR), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

psr_stat psr_type_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(TYPE), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

psr_stat psr_key_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(KEY), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

psr_stat psr_reg_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(REG), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

psr_stat psr_int_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(INT), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

psr_stat psr_str_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(STR), 3, p_vtkris_f);
    return PSR_STAT(OK);
}

static void p_flt_f(void *p) {
    te *n = p;
    te_f(n->d[2].p);
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    n->af->f(n);
}

psr_stat psr_flt_i(psr *p, te **n, err **e) {
    (void) e;
    *n = te_i(5, p->ta, p_flt_f);
    (*n)->d[1] = U6(NODE_TYPE(FLT));
    un m;
    for (size_t i = 4; i > 1; i--) {
        vr_sb(p->ts, &m);
        (*n)->d[i] = P(m.p);
    }
    return PSR_STAT(OK);
}

psr_stat psr_val_m(psr *p, te *restrict nh, te *restrict n, err **e) {
    (void) p;
    if (nh->d[1].p && nh->d[2].p) return psr_err(p, e, "value op");
    if (nh->d[1].p) {
        if (((te*) nh->d[1].p)->d[4].p) return psr_err(p, e, "psr value value");
        ((te*) nh->d[1].p)->d[4] = P(n);
        n->d[0] = nh->d[1];
    } else nh->d[2] = P(n);
    return PSR_STAT(OK);
}

static void p_op_f(te *n) {
    te_f(n->d[2].p);
    te_f(n->d[3].p);
    te_f(n->d[4].p);
    n->af->f(n);
}

psr_stat psr_op_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(OP), 5, p_op_f);
    return PSR_STAT(OK);
}

psr_stat psr_op_m(psr *p, te *restrict nh, te *restrict n, err **e) {
    (void) p;
    if (nh->d[1].p && nh->d[2].p) return psr_err(p, e, "psr op op");
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

static void p_vl_f(te *n) {
    te_f(n->d[2].p);
    lst_f(n->d[3].p);
    n->af->f(n);
}

psr_stat psr_vec_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(VEC), 4, p_vl_f);
    (*n)->d[3].p = lst_i(p->la, p->ta, (void*) te_f);
    return PSR_STAT(OK);
}

psr_stat psr_lst_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(LST), 4, p_vl_f);
    (*n)->d[3].p = lst_i(p->la, p->ta, (void*) te_f);
    return PSR_STAT(OK);
}

psr_stat psr_lst_e(psr *p, te *restrict en, te *restrict n, err **e) {
    (void) p;
    (void) e;
    n->d[0] = P(en);
    lst_ab(en->d[3].p, P(n));
    return PSR_STAT(OK);
}

static void p_a_f(te *n) {
    te_f(n->d[2].p);
    te_f(n->d[3].p);
    lst_f(n->d[4].p);
    n->af->f(n);
}

psr_stat psr_aply_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(APLY), 5, p_a_f);
    (*n)->d[4].p = lst_i(p->la, p->ta, (void*) te_f);
    return PSR_STAT(OK);
}

psr_stat psr_aply_m(psr *p, te *restrict nh, te *restrict n, err **e) {
    (void) p;
    (void) e;
    if (!nh->d[1].p && !nh->d[2].p) nh->d[2] = P(n);
    else if (nh->d[1].p && ((te*) nh->d[1].p)->d[4].p) {
        n->d[0] = ((te*) ((te*) nh->d[1].p)->d[4].p)->d[0];
        n->d[3] = ((te*) nh->d[1].p)->d[4];
        ((te*) nh->d[1].p)->d[4] = ((te*) ((te*) nh->d[1].p)->d[4].p)->d[0] = P(n);
    } else if (nh->d[1].p && ((te*) nh->d[1].p)->d[3].p && !((te*) nh->d[1].p)->d[4].p) {
        n->d[0] = nh->d[1];
        ((te*) nh->d[1].p)->d[4] = P(n);
    } else if (nh->d[1].p) {
        n->d[3] = nh->d[1];
        if (((te*) nh->d[1].p)->d[0].p) {
            nh->d[1] = n->d[0] = ((te*) nh->d[1].p)->d[0];
            ((te*) n->d[3].p)->d[0] = ((te*) n->d[0].p)->d[4] = P(n);
        } else {
            ((te*) nh->d[1].p)->d[0] = nh->d[2] = P(n);
            nh->d[0] = nh->d[1] = P(NULL);
        }
    } else {
        n->d[3] = nh->d[2];
        if (((te*) nh->d[2].p)->d[0].p) n->d[0] = ((te*) nh->d[2].p)->d[0];
        ((te*) nh->d[2].p)->d[0] = P(n);
        nh->d[1] = P(NULL);
        nh->d[2] = P(n);
    }
    return PSR_STAT(OK);
}

psr_stat psr_aply_e(psr *p, te *restrict en, te *restrict n, err **e) {
    (void) p;
    (void) e;
    n->d[0] = P(en);
    lst_ab(en->d[4].p, P(n));
    return PSR_STAT(OK);
}

static void p_sc_i(te *n) {
    te_f(n->d[2].p);
    te_f(n->d[3].p);
    n->af->f(n);
}

psr_stat psr_sym_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(SYM), 4, p_sc_i);
    return PSR_STAT(OK);
}

psr_stat psr_cmd_i(psr *p, te **n, err **e) {
    (void) e;
    *n = node_i(p, NODE_TYPE(CMD), 4, p_sc_i);
    return PSR_STAT(OK);
}

te *node_g_root(te *n) {
    while (n->d[1].u6 != NODE_TYPE(ROOT)) n = n->d[0].p;
    return n;
}

const mc *node_root_mc(te *n) {
    n = node_g_root(n);
    return ((psr*) n->d[0].p)->tt->s;
}

mc *node_root_fname(te *n) {
    n = node_g_root(n);
    return ((psr*) n->d[0].p)->fname;
}

void node_p(const te *n, size_t idnt) {
    te *h;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    printf("(nid:%lu", n->d[1].u6);
    switch (n->d[1].u6) {
        case NODE_TYPE(ROOT):
            break;
        case NODE_TYPE(VAR):
        case NODE_TYPE(TYPE):
        case NODE_TYPE(KEY):
        case NODE_TYPE(REG):
        case NODE_TYPE(INT):
        case NODE_TYPE(STR):
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
        case NODE_TYPE(VEC):
        case NODE_TYPE(LST):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            h = ((lst*) n->d[3].p)->h;
            if (h) printf("|\n");
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
            printf("|");
            if (n->d[3].p) {
                putchar('\n');
                node_p(n->d[3].p, idnt + 1);
            }
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
       case NODE_TYPE(CMD):
            putchar('|');
            tkn_m_p(n->d[2].p, node_root_mc(n->d[0].p));
            printf("|");
            if (n->d[3].p) {
                putchar('\n');
                node_p(n->d[3].p, idnt + 1);
            }
            putchar(')');
            break;
      default:
        printf("INV");
        break;
    }
}

static un node_tkn_g_s_e(const te *n) {
    un se = U6(0);
    if (!n) return se;
    switch (n->d[1].u6) {
        case NODE_TYPE(ROOT):
            break;
        case NODE_TYPE(FLT):
            se = u5_s_o(se, TKN_S, tkn_m_g_s(n->d[2].p));
            se = u5_s_o(se, TKN_E, tkn_m_g_e(n->d[4].p));
            break;
        default:
            se = u5_s_o(se, TKN_S, tkn_m_g_s(n->d[2].p));
            se = u5_s_o(se, TKN_E, tkn_m_g_e(n->d[2].p));
            break;
    }
    return se;
}

static void node_tkn_mm(const te *n, uint32_t *s, uint32_t *e) {
    if (!n) return;
    te *h;
    switch (n->d[1].u6) {
        case NODE_TYPE(OP):
            node_tkn_mm(n->d[3].p, s, e);
            node_tkn_mm(n->d[4].p, s, e);
            break;
        case NODE_TYPE(VEC):
        case NODE_TYPE(LST):
            h = ((lst*) n->d[3].p)->h;
            while (h) {
                node_tkn_mm(h->d[0].p, s, e);
                h = h->d[2].p;
            }
            break;
        case NODE_TYPE(APLY):
            node_tkn_mm(n->d[3].p, s, e);
            h = ((lst*) n->d[4].p)->h;
            while (h) {
                node_tkn_mm(h->d[0].p, s, e);
                h = h->d[2].p;
            }
            break;
        case NODE_TYPE(SYM):
        case NODE_TYPE(CMD):
            node_tkn_mm(n->d[3].p, s, e);
            break;
        default:
            break;
    }
    un se = node_tkn_g_s_e(n);
    if (!se.u6) return;
    if (u5_g_o(se, TKN_S) < *s) *s = u5_g_o(se, TKN_S);
    if (u5_g_o(se, TKN_E) > *e) *e = u5_g_o(se, TKN_E);
}

psr_stat node_tkn_s_e(const te *n, uint32_t *s, uint32_t *e) {
    un se = node_tkn_g_s_e(n);
    if (!se.u6) return PSR_STAT(INV);
    *s = u5_g_o(se, TKN_S);
    *e = u5_g_o(se, TKN_E);
    node_tkn_mm(n, s, e);
    return PSR_STAT(OK);
}

psr *psr_b(psr *p) {
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, TCUST(WS));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, TCUST(TAB));
    // end stmt
    te *vec_stp = te_i(2, p->ta, NULL);
    vec_stp->d[0] = U4(tkn_a(p->tt, TCUST(RS), "]", tkn_ft));
    te *lst_stp = te_i(2, p->ta, NULL);
    lst_stp->d[0] = U4(tkn_a(p->tt, TCUST(RB), "}", tkn_ft));
    te *aply_stp = te_i(2, p->ta, NULL);
    aply_stp->d[0] = U4(tkn_a(p->tt, TCUST(RP), ")", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(SEMI));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(NL));
    // lists
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), vec_stp, psr_lst_e, psr_val_m, psr_vec_i, 1, tkn_a(p->tt, TCUST(LS), "[", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RS));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), lst_stp, psr_lst_e, psr_val_m, psr_lst_i, 1, tkn_a(p->tt, TCUST(LB), "{", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RB));
    psr_a(p, PARSER(UN), PSR_MODE(LOOP), aply_stp, psr_aply_e, psr_aply_m, psr_aply_i, 1, tkn_a(p->tt, TCUST(LP), "(", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(STOP), NULL, NULL, NULL, NULL, 1, TCUST(RP));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_sym_i, 1, TCUST(SYM));
    // value
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_var_i, 1, TCUST(VAR));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_str_i, 1, TCUST(STR));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_int_i, 1, TCUST(NUM));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL,  psr_val_m, psr_flt_i, 3, TCUST(NUM), TCUST(DOT), TCUST(NUM));
    // type
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(VD), "VD", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(I6), "I6", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(U6), "U6", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(F6), "F6", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(SG), "SG", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(FN), "FN", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(NF), "NF", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(CJ), "CJ", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(ST), "ST", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(UN), "UN", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_type_i, 1, tkn_a(p->tt, TCUST(VR), "VR", tkn_ft));
    // key
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_key_i, 1, tkn_a(p->tt, TCUST(A), "A", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_key_i, 1, tkn_a(p->tt, TOKEN(UN), "S", tkn_ft));
    // regex
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_val_m, psr_reg_i, 1, TCUST(REGT));
    // op
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, TCUST(DOT));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(AGN), ":", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(DFN), "::", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(CST), "$", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(IF), "?", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(LOOP), "@", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(MTCH), "#", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(ADD), "+", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(ADDA), "+:", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(SUB), "-", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(SUBA), "-:", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(MUL), "*", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(MULA), "*:", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(DIV), "/", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(DIVA), "/:", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(MOD), "%", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(NOT), "!", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(EQ), "=", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(NE), "!=", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(GT), ">", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(GTE), ">=", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(LT), "<", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(LTE), "<=", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(AND), "&", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(OR), "|", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(CNCT), ",", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(CNCTA), ",:", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(p->tt, TCUST(UNER), "'", tkn_ft));
    // cmd
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_cmd_i, 1, tkn_a(p->tt, TCUST(E), "\\e", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_cmd_i, 1, tkn_a(p->tt, TCUST(L), "\\l", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_cmd_i, 1, tkn_a(p->tt, TCUST(T), "\\t", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_cmd_i, 1, tkn_a(p->tt, TCUST(RT), "\\rt", tkn_ft));
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_aply_m, psr_cmd_i, 1, tkn_a(p->tt, TCUST(P1), "\\p1", tkn_ft));
    // misc
    psr_a(p, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, NULL, NULL, 1, tkn_a(p->tt, TCUST(CMT), "//", tkn_cmt));
    return p;
}
