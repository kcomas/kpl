
#include "ast.h"

const char *ast_oc_str(oc o) {
    static const char *ocs[] = {
        "_START",
        "AGN",
        "CST",
        "ADD",
        "SUB",
        "_END"
    };
    const char *s = "INV";
    if (o > OC(_START) && o < OC(_END)) s = ocs[o];
    return s;
}

static ast_stat err(ast_stat stat, te *pn, te **e) {
    *e = te_c(pn);
    return stat;
}

static ast_stat ast_root(ast *a, te *pn, void **vn, te **e) {
    te **an = (te**) vn;
    *an = ast_an_i(a, NULL, pn, AST_CLS(R), P(NULL)); // a->ati()
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, pn->d[2].p, &(*an)->d[4].p, e);
}

static ast_stat ast_var(ast *a, te *pn, void **vn, te **e) {
    te **an = (te**) vn;
    mc *v;
    if (tkn_g_mc(pn->d[2].p, node_root_mc(pn), 0, a->ma, &v) != TKN_STAT(OK)) return err(AST_STAT(INV), pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(I), P(NULL), v);
    return AST_STAT(OK);
}

static ast_stat ast_type(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    te **an = (te**) vn;
    size_t tid;
    if ((stat = ast_t_n(a, ((te*) pn->d[2].p)->d[0].u6, &tid)) != AST_STAT(OK)) return err(stat, pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(T), P(type_i(a->ta, tid)));
    return AST_STAT(OK);
}

static ast_stat ast_int(ast *a, te *pn, void **vn, te **e) {
    te **an = (te**) vn;
    int64_t i = 0;
    if (tkn_g_i6(pn->d[2].p, node_root_mc(pn), &i) != TKN_STAT(OK)) return err(AST_STAT(INV), pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(S), P(type_i(a->ta, TYPE(I6))), I6(i));
    return AST_STAT(OK);
}

static ast_stat ast_op(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    te **an = (te**) vn;
    size_t oid;
    if ((stat = ast_t_n(a, ((te*) pn->d[2].p)->d[0].u6, &oid)) != AST_STAT(OK)) return err(stat, pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(O), P(NULL), U6(oid), NULL, NULL);
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[5].p, e)) != AST_STAT(OK)) return err(stat, pn, e);
    if (!pn->d[4].p) return AST_STAT(OK);
    return ast_n(a, pn->d[4].p, &(*an)->d[6].p, e);
}

static ast_stat ast_lst(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, *an, pn, AST_CLS(L), P(NULL), NULL);
    if (!pn->d[3].p) return AST_STAT(OK);
    lst *ll = a->ali();
    (*an)->d[4] = P(ll);
    te *h = ((lst*) pn->d[3].p)->h;
    while (h) {
        if ((stat = ast_n(a, h->d[0].p, &ln, e)) != AST_STAT(OK)) return err(stat, pn, e);
        lst_ab(ll, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

static ast_stat ast_aply(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    void *ln;
    te **an = (te**) vn;
    *an = ast_an_i(a, *an, pn, AST_CLS(A), P(NULL), NULL, NULL);
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[4].p, e)) != AST_STAT(OK)) return err(stat, pn, e);
    if (!pn->d[4].p) return AST_STAT(OK);
    lst *pl = pn->d[4].p;
    if (pl->l == 0) return AST_STAT(OK);
    lst *al = a->ali();
    (*an)->d[5] = P(al);
    te *h = pl->h;
    while (h) {
        if ((stat = ast_n(a, h->d[0].p, &ln, e)) != AST_STAT(OK)) return err(stat, pn, e);
        lst_ab(al, P(ln));
        h = h->d[2].p;
    }
    return AST_STAT(OK);
}

static ast_stat ast_z(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    te **an = (te**) vn;
    mc *v;
    if (tkn_g_mc(pn->d[2].p, node_root_mc(pn), 1, a->ma, &v) != TKN_STAT(OK)) return err(AST_STAT(INV), pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(Z), P(type_i(a->ta, TYPE(SL))), v, NULL);
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[5].p, e)) != AST_STAT(OK)) return err(stat, pn, e);
    return AST_STAT(OK);
}

static ast *ast_tkn(ast *a) {
    // types
    ast_t_a(a, TCUST(I6), TYPE(I6));
    ast_t_a(a, TCUST(U6), TYPE(U6));
    ast_t_a(a, TCUST(F6), TYPE(F6));
    ast_t_a(a, TCUST(FN), TYPE(FN));
    ast_t_a(a, TCUST(UN), TYPE(UN));
    // ops
    ast_t_a(a, TCUST(AGN), OC(AGN));
    ast_t_a(a, TCUST(CST), OC(CST));
    ast_t_a(a, TCUST(ADD), OC(ADD));
    ast_t_a(a, TCUST(SUB), OC(SUB));
    return a;
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), ast_root);
    ast_a(a, NODE_TYPE(VAR), ast_var);
    ast_a(a, NODE_TYPE(TYPE), ast_type);
    ast_a(a, NODE_TYPE(INT), ast_int);
    ast_a(a, NODE_TYPE(OP), ast_op);
    ast_a(a, NODE_TYPE(LST), ast_lst);
    ast_a(a, NODE_TYPE(APLY), ast_aply);
    ast_a(a, NODE_TYPE(SYM), ast_z);
    return ast_tkn(a);
}

void ast_p(const te *an, size_t idnt) {
    if (!an) return;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    switch (an->d[2].u6) {
        case AST_CLS(R):
            ast_p(an->d[4].p, idnt);
            break;
        case AST_CLS(T):
            printf("(T ");
            type_p(an->d[3].p);
            putchar(')');
            break;
        case AST_CLS(I):
            printf("(I ");
            // TODO
            printf("%s", (char*) ((mc*) an->d[4].p)->d);
            putchar(')');
            break;
        case AST_CLS(S):
            printf("(S ");
            type_p(an->d[3].p);
            switch (((te*) an->d[3].p)->d[0].u6) {
                case TYPE(I6):
                    printf(" %ld", an->d[4].i6);
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
            printf(" [%s]", ast_oc_str(an->d[4].u6));
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
            printf("`%s ", (char*) ((mc*) an->d[4].p)->d);
            type_p(an->d[3].p);
            if (an->d[5].p) {
                putchar('\n');
                ast_p(an->d[5].p, idnt + 1);
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
            // TODO
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
        default:
            printf("INV CLS");
            break;
    }
}

static bool ast_v_eq(const te *restrict t, const te *restrict a, const te *restrict b) {
    if (t->d[0].u6 == TYPE(VD)) return true;
    if (t->d[0].u6 >= TYPE(I3) && t->d[0].u6 <= TYPE(F6)) return a->d[4].u6 == b->d[4].u6;
    if (t->d[0].u6 == TYPE(C4)) return c4_eq(a->d[4], b->d[4]);
    return false;
}

static bool ast_un_eq(un a, un b) {
    return ast_eq(a.p, b.p);
}

bool ast_eq(const te *restrict a, const te *restrict b) {
    if (!a && !b) return true;
    if (!a || !b || a->d[2].u6 != b->d[2].u6) return false;
    switch (a->d[2].u6) {
        case AST_CLS(R):
            // TODO compare tbls
            return ast_eq(a->d[4].p, b->d[4].p);
        case AST_CLS(T):
            return type_eq(a->d[3].p, b->d[3].p);
        case AST_CLS(I):
            // TODO compare entries
            return mc_eq(a->d[4].p, b->d[4].p);
        case AST_CLS(S):
            return type_eq(a->d[3].p, b->d[3].p) && ast_v_eq(a->d[3].p, a, b);
        case AST_CLS(V):
            break;
        case AST_CLS(O):
            return type_eq(a->d[3].p, b->d[3].p) && a->d[4].u6 == b->d[4].u6 && ast_eq(a->d[5].p, b->d[5].p) && ast_eq(a->d[6].p, b->d[6].p);
        case AST_CLS(Z):
            return type_eq(a->d[3].p, b->d[3].p) && mc_eq(a->d[4].p, b->d[4].p) && ast_eq(a->d[5].p, b->d[5].p);
        case AST_CLS(A):
            return type_eq(a->d[3].p, b->d[3].p) && ast_eq(a->d[4].p, b->d[4].p) && lst_eq(a->d[5].p, b->d[5].p, ast_un_eq);
        case AST_CLS(L):
            // TODO tbl cmp
            return lst_eq(a->d[4].p, b->d[4].p, ast_un_eq);
        default:
            break;
    }
    return false;
}
