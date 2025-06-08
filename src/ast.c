#include "ast.h"

const char *ast_oc_str(oc o) {
    static const char *ocs[] = {
        "_START",
        "DFN",
        "AGN",
        "CST",
        "LOOP",
        "IF",
        "ADD",
        "ADDA",
        "SUB",
        "SUBA",
        "MUL",
        "MULA",
        "DIV",
        "EQ",
        "GT",
        "GTE",
        "LT",
        "LTE",
        "AND",
        "DUMP",
        "_END"
    };
    const char *s = "OCINV";
    if (o > OC(_START) && o < OC(_END)) s = ocs[o];
    return s;
}

const char *ast_cc_str(cc c) {
    static const char *ccs[] = {
        "_START",
        "E",
        "P1",
        "_END"
    };
    const char *s = "CCINV";
    if (c > CC(_START) && c < CC(_END)) s = ccs[c];
    return s;
}

static void lst_tbl_e_f(void *p) {
    te *lte = p;
    mc_f(lte->d[0].p);
    te_f(lte->d[2].p);
    te_f(lte->d[3].p);
    lte->af->f(lte);
}

te *ast_lst_tbl_e_i(const ast *a, mc *s, un flgs, te *t) {
    te *lte = te_i(4, a->ta, lst_tbl_e_f);
    lte->d[0] = P(s);
    lte->d[1] = flgs;
    lte->d[2] = P(t);
    return lte;
}

uint32_t ast_lst_tbl_e_g_f(te *ent) {
    return u5_g_o(ent->d[1], 0);
}

void ast_lst_tbl_e_s_f(te *ent, uint32_t flg) {
    ent->d[1] = u5_s_o(ent->d[1], 0, flg | ast_lst_tbl_e_g_f(ent));
}

uint32_t ast_lst_tbl_e_g_i(te *ent) {
    return u5_g_o(ent->d[1], 1);
}

void ast_lst_tbl_e_s_i(te *ent, uint32_t id) {
    ent->d[1] = u5_s_o(ent->d[1], 1, id);
}

void node_err_p(void *d) {
    uint32_t s = 0, e = 0;
    if (node_tkn_s_e(d, &s, &e) != PSR_STAT(OK)) {
        printf("\e[1;91mCANNOT PRINT STR FROM AST\e[0m\n");
        return;
    }
    const mc *str = node_root_mc(d);
    te *tkn = ((te*) d)->d[2].p;
    printf("\e[1m%d::%d\e[0m ", tkn_m_g_l(tkn), tkn_m_g_c(tkn));
    for (uint32_t i = s; i < e; i++) putchar(str->d[i]);
}

void node_err_f(void *d) {
    te_f(node_g_root(d));
}

void ast_err_p(void *d) {
    te *t = d;
    node_err_p(t->d[1].p);
    putchar('\n');
    ast_p(t, 0);
}

void ast_err_f(void *d) {
    te_f(ast_g_root(d));
}

static ast_stat ast_err(const ast *a, te *pn, err **e, const char *m) {
    te_c(node_g_root(pn));
    *e = err_i(a->ea, node_err_p, node_err_f, pn, m);
    return AST_STAT(INV);
}

static ast_stat ast_root(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    te **an = (te**) vn;
    *an = ast_an_i(a, pan, pn, AST_CLS(R), P(NULL));
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, *an, pn->d[2].p, &(*an)->d[4].p, e);
}

static ast_stat ast_var(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    te **an = (te**) vn;
    mc *v;
    if (tkn_g_mc(pn->d[2].p, node_root_mc(pn), 0, a->ma, &v) != TKN_STAT(OK)) return ast_err(a, pn, e, "ast var");
    *an = ast_an_i(a, pan, pn, AST_CLS(I), P(v));
    return AST_STAT(OK);
}

static ast_stat ast_type(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    te **an = (te**) vn;
    uint16_t tid;
    if ((stat = ast_t_n(a, pn->d[2].p, &tid)) != AST_STAT(OK)) return ast_err(a, pn, e, "ast type");
    *an = ast_an_i(a, pan, pn, AST_CLS(T), P(type_i(a->ta, NULL, tid)));
    return AST_STAT(OK);
}

static ast_stat ast_int(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    te **an = (te**) vn;
    int64_t i = 0;
    if (tkn_g_i6(pn->d[2].p, node_root_mc(pn), &i) != TKN_STAT(OK)) return ast_err(a, pn, e, "ast int");
    *an = ast_an_i(a, pan, pn, AST_CLS(S), P(type_i(a->ta, NULL, TYPE(I6))), I6(i));
    return AST_STAT(OK);
}

static ast_stat ast_flt(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    te **an = (te**) vn;
    double d = 0;
    if (tkn_g_f6(pn->d[2].p, pn->d[3].p, pn->d[4].p, node_root_mc(pn), &d) != TKN_STAT(OK)) return ast_err(a, pn, e, "ast flt");
    *an = ast_an_i(a, pan, pn, AST_CLS(S), P(type_i(a->ta, NULL, TYPE(F6))), F6(d));
    return AST_STAT(OK);
}

static ast_stat ast_op(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    te **an = (te**) vn;
    uint16_t oid;
    if ((stat = ast_t_n(a, pn->d[2].p, &oid)) != AST_STAT(OK)) return ast_err(a, pn, e, "ast op");
    *an = ast_an_i(a, pan, pn, AST_CLS(O), P(NULL), U4(oid), NULL, NULL);
    if (pn->d[3].p && (stat = ast_n(a, *an, pn->d[3].p, &(*an)->d[5].p, e)) != AST_STAT(OK)) {
        te_f(*an);
        return stat;
    }
    if (!pn->d[4].p) return AST_STAT(OK);
    return ast_n(a, *an, pn->d[4].p, &(*an)->d[6].p, e);
}

static ast_stat ast_lst(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, pan, pn, AST_CLS(L), P(NULL), NULL);
    if (!pn->d[3].p) return AST_STAT(OK);
    lst *ll = a->ali();
    (*an)->d[4] = P(ll);
    te *h = ((lst*) pn->d[3].p)->h;
    while (h) {
        if ((stat = ast_n(a, *an, h->d[0].p, &ln, e)) != AST_STAT(OK)) return stat;
        lst_ab(ll, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

static ast_stat ast_aply(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, pan, pn, AST_CLS(A), P(NULL), NULL, NULL);
    if (pn->d[3].p && (stat = ast_n(a, *an, pn->d[3].p, &(*an)->d[4].p, e)) != AST_STAT(OK)) return stat;
    if (!pn->d[4].p) return AST_STAT(OK);
    lst *pl = pn->d[4].p;
    if (pl->l == 0) return AST_STAT(OK);
    lst *al = a->ali();
    (*an)->d[5] = P(al);
    te *h = pl->h;
    while (h) {
        if ((stat = ast_n(a, *an, h->d[0].p, &ln, e)) != AST_STAT(OK)) return stat;
        lst_ab(al, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

static ast_stat ast_z(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    te **an = (te**) vn;
    mc *v;
    if (tkn_g_mc(pn->d[2].p, node_root_mc(pn), 1, a->ma, &v) != TKN_STAT(OK)) return ast_err(a, pn, e, "ast z");
    *an = ast_an_i(a, pan, pn, AST_CLS(Z), P(type_i(a->ta, NULL, TYPE(SL))), NULL, v);
    if (!pn->d[3].p) return AST_STAT(OK);
    return ast_n(a, *an, pn->d[3].p, &(*an)->d[4].p, e);
}

static ast_stat ast_cmd(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e) {
    ast_stat stat;
    te **an = (te**) vn;
    uint16_t cid;
    if ((stat = ast_t_n(a, pn->d[2].p, &cid)) != AST_STAT(OK)) return ast_err(a, pn, e, "ast cmd");
    *an = ast_an_i(a, pan, pn, AST_CLS(C), U4(cid), NULL);
    if (!pn->d[3].p) return AST_STAT(OK);
    return ast_n(a, *an, pn->d[3].p, &(*an)->d[4].p, e);
}

static ast *ast_tkn(ast *a) {
    // types
    ast_t_a(a, TCUST(VD), TYPE(VD));
    ast_t_a(a, TCUST(I6), TYPE(I6));
    ast_t_a(a, TCUST(U6), TYPE(U6));
    ast_t_a(a, TCUST(F6), TYPE(F6));
    ast_t_a(a, TCUST(FN), TYPE(FN));
    ast_t_a(a, TCUST(NF), TYPE(NF));
    ast_t_a(a, TCUST(UN), TYPE(UN));
    // ops
    ast_t_a(a, TCUST(DFN), OC(DFN));
    ast_t_a(a, TCUST(AGN), OC(AGN));
    ast_t_a(a, TCUST(CST), OC(CST));
    ast_t_a(a, TCUST(LOOP), OC(LOOP));
    ast_t_a(a, TCUST(IF), OC(IF));
    ast_t_a(a, TCUST(ADD), OC(ADD));
    ast_t_a(a, TCUST(ADDA), OC(ADDA));
    ast_t_a(a, TCUST(SUB), OC(SUB));
    ast_t_a(a, TCUST(SUBA), OC(SUBA));
    ast_t_a(a, TCUST(MUL), OC(MUL));
    ast_t_a(a, TCUST(MULA), OC(MULA));
    ast_t_a(a, TCUST(DIV), OC(DIV));
    ast_t_a(a, TCUST(EQ), OC(EQ));
    ast_t_a(a, TCUST(GT), OC(GT));
    ast_t_a(a, TCUST(GTE), OC(GTE));
    ast_t_a(a, TCUST(LT), OC(LT));
    ast_t_a(a, TCUST(LTE), OC(LTE));
    ast_t_a(a, TCUST(AND), OC(AND));
    // cmds
    ast_t_a(a, TCUST(E), CC(E));
    ast_t_a(a, TCUST(P1), CC(P1));
    return a;
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), ast_root);
    ast_a(a, NODE_TYPE(VAR), ast_var);
    ast_a(a, NODE_TYPE(TYPE), ast_type);
    ast_a(a, NODE_TYPE(INT), ast_int);
    ast_a(a, NODE_TYPE(FLT), ast_flt);
    ast_a(a, NODE_TYPE(OP), ast_op);
    ast_a(a, NODE_TYPE(LST), ast_lst);
    ast_a(a, NODE_TYPE(APLY), ast_aply);
    ast_a(a, NODE_TYPE(SYM), ast_z);
    ast_a(a, NODE_TYPE(CMD), ast_cmd);
    return ast_tkn(a);
}

static void lst_tbl_e_p(te *lte) {
    static const char *lbl_flag_str[] = {
        "E",
        "A",
        "L",
        "T",
        "F",
        "O",
        "S"
    };
    type_p(lte->d[2].p);
    printf("`%s", (char*) ((mc*) lte->d[0].p)->d);
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs) {
        printf("<%d|", id);
        for (int32_t i = LTE_FLGS - 1; i > -1; i--) {
            if (flgs & ((uint32_t) 1 << i)) printf("%s", lbl_flag_str[i]);
        }
        printf(">");
    }
    // TODO ast node
}

void ast_p(const te *an, size_t idnt) {
    if (!an) return;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    switch (an->d[2].u4) {
        case AST_CLS(R):
            ast_p(an->d[4].p, idnt);
            break;
        case AST_CLS(T):
            printf("(T ");
            type_p(an->d[3].p);
            putchar(')');
            break;
        case AST_CLS(E):
            printf("(E ");
            lst_tbl_e_p(an->d[3].p);
            putchar(')');
            break;
        case AST_CLS(I):
            printf("(I %s)", (char*) ((mc*) an->d[3].p)->d);
            break;
        case AST_CLS(S):
            printf("(S ");
            type_p(an->d[3].p);
            switch (((te*) an->d[3].p)->d[1].u4) {
                case TYPE(_G):
                case TYPE(U5):
                    printf(" %u", an->d[4].i5);
                    break;
                case TYPE(I5):
                    printf(" %d", an->d[4].i5);
                    break;
                case TYPE(U6):
                    printf(" %lu", an->d[4].i6);
                    break;
                case TYPE(I6):
                    printf(" %ld", an->d[4].i6);
                    break;
                case TYPE(F5):
                    printf(" %f", an->d[4].f5);
                    break;
                case TYPE(F6):
                    printf(" %lf", an->d[4].f6);
                    break;
                default:
                    printf("INV S");
                    break;
            }
            putchar(')');
            break;
        case AST_CLS(V):
            break;
        case AST_CLS(O):
            printf("(O ");
            type_p(an->d[3].p);
            printf(" [%s]", ast_oc_str(an->d[4].u4));
            if (an->d[5].p) {
                putchar('\n');
                ast_p(an->d[5].p, idnt + 1);
            }
            if (an->d[6].p) {
                putchar('\n');
                ast_p(an->d[6].p, idnt + 1);
            }
            putchar(')');
            break;
        case AST_CLS(Z):
            printf("(Z ");
            type_p(an->d[3].p);
            printf(" `%s", (char*) ((mc*) an->d[5].p)->d);
            if (an->d[4].p) {
                putchar('\n');
                ast_p(an->d[4].p, idnt + 1);
            }
            putchar(')');
            break;
        case AST_CLS(A):
            printf("(A ");
            type_p(an->d[3].p);
            if (an->d[4].p) {
                putchar('\n');
                ast_p(an->d[4].p, idnt + 1);
            }
            if (an->d[5].p) {
                te *h = ((lst*) an->d[5].p)->h;
                while (h) {
                    putchar('\n');
                    ast_p(h->d[0].p, idnt + 2);
                    h = h->d[2].p;
                }
            }
            putchar(')');
            break;
        case AST_CLS(L):
            printf("(L ");
            if (an->d[3].p) {
                putchar('|');
                te *h = ((tbl*) an->d[3].p)->i->h;
                while (h) {
                    lst_tbl_e_p(h->d[0].p);
                    h = h->d[2].p;
                    if (h) putchar(' ');
                }
                putchar('|');
            }
            if (an->d[4].p) {
                te *h = ((lst*) an->d[4].p)->h;
                while (h) {
                    putchar('\n');
                    ast_p(h->d[0].p, idnt + 1);
                    h = h->d[2].p;
                }
            }
            putchar(')');
            break;
        case AST_CLS(C):
            printf("(C [%s]", ast_cc_str(an->d[3].u4));
            if (an->d[4].p) {
                putchar('\n');
                ast_p(an->d[4].p, idnt + 1);
            }
            putchar(')');
            break;
        default:
            printf("INV CLS");
            break;
    }

}
static bool lst_tbl_e_eq(const te *restrict ea, const te *restrict eb) {
    return mc_eq(ea->d[0].p, eb->d[0].p) && ea->d[1].u6 == eb->d[1].u6 && type_eq(ea->d[2].p, eb->d[2].p) && ast_eq(ea->d[3].p, eb->d[3].p);
}

static bool ast_v_eq(const te *restrict t, const te *restrict a, const te *restrict b) {
    if (t->d[1].u4 == TYPE(_G)) return a->d[4].u5 == b->d[4].u5;
    if (t->d[1].u4 == TYPE(VD)) return true;
    if (t->d[1].u4 >= TYPE(I3) && t->d[1].u4 <= TYPE(F6)) return a->d[4].u6 == b->d[4].u6;
    if (t->d[1].u4 == TYPE(C4)) return c4_eq(a->d[4], b->d[4]);
    return false;
}

static bool ast_un_eq(un a, un b) {
    return ast_eq(a.p, b.p);
}

bool ast_eq(const te *restrict a, const te *restrict b) {
    if (!a && !b) return true;
    if (!a || !b || a->d[2].u4 != b->d[2].u4) return false;
    switch (a->d[2].u4) {
        case AST_CLS(R):
            // TODO compare tbls
            return ast_eq(a->d[4].p, b->d[4].p);
        case AST_CLS(T):
            return type_eq(a->d[3].p, b->d[3].p);
        case AST_CLS(E):
            return lst_tbl_e_eq(a->d[3].p, b->d[3].p);
        case AST_CLS(I):
            return mc_eq(a->d[3].p, b->d[3].p);
        case AST_CLS(S):
            return type_eq(a->d[3].p, b->d[3].p) && ast_v_eq(a->d[3].p, a, b);
        case AST_CLS(V):
            // TODO
            break;
        case AST_CLS(O):
            return type_eq(a->d[3].p, b->d[3].p) && a->d[4].u4 == b->d[4].u4 && ast_eq(a->d[5].p, b->d[5].p) && ast_eq(a->d[6].p, b->d[6].p);
        case AST_CLS(Z):
            return type_eq(a->d[3].p, b->d[3].p) && ast_eq(a->d[4].p, b->d[4].p) && mc_eq(a->d[5].p, b->d[5].p);
        case AST_CLS(A):
            return type_eq(a->d[3].p, b->d[3].p) && ast_eq(a->d[4].p, b->d[4].p) && lst_eq(a->d[5].p, b->d[5].p, ast_un_eq);
        case AST_CLS(L):
            return tbl_eq(a->d[3].p, b->d[3].p, lst_tbl_e_eq) && lst_eq(a->d[4].p, b->d[4].p, ast_un_eq);
        case AST_CLS(C):
            return a->d[3].u4 == b->d[3].u4 && ast_eq(a->d[4].p, b->d[4].p);
        default:
            break;
    }
    return false;
}

te *ast_cpy(const te *an) {
    ast_p(an, 0);
    HERE("TODO");
    exit(1);
    return NULL;
}
