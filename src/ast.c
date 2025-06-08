
#include "ast.h"

const char *ast_op_str(op o) {
    static const char *ops[] = {
        "_START",
        "ADD",
        "SUB",
        "_END"
    };
    const char *s = "INV";
    if (o > OP(_START) && o < OP(_END)) s = ops[o];
    return s;
}

static ast_stat err(ast_stat stat, te *pn, te **e) {
    *e = pn;
    return stat;
}

static ast_stat root(ast *a, te *pn, void **vn, te **e) {
    te **an = (te**) vn;
    *an = ast_an_i(a, NULL, pn, AST_CLS(R), P(a->ati()));
    if (!pn->d[2].p) return AST_STAT(OK);
    return ast_n(a, pn->d[2].p, &(*an)->d[4].p, e);
}

static ast_stat i(ast *a, te *pn, void **vn, te **e) {
    te **an = (te**) vn;
    int64_t i = 0;
    if (tkn_g_i6(pn->d[2].p, node_root_mc(pn), &i) != TKN_STAT(OK)) return err(AST_STAT(INV), pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(S), P(type_s_i(a->ta, TYPE(I6))), I6(i));
    return AST_STAT(OK);
}

static ast_stat o(ast *a, te *pn, void **vn, te **e) {
    ast_stat stat;
    te **an = (te**) vn;
    size_t oid;
    if ((stat = ast_t_n(a, ((te*) pn->d[2].p)->d[0].u6, &oid)) != AST_STAT(OK)) return err(stat, pn, e);
    *an = ast_an_i(a, *an, pn, AST_CLS(O), P(NULL), NULL, NULL, U6(oid));
    if (pn->d[3].p && (stat = ast_n(a, pn->d[3].p, &(*an)->d[4].p, e)) != AST_STAT(OK)) return err(stat, pn, e);
    if (!pn->d[4].p) return AST_STAT(OK);
    return ast_n(a, pn->d[4].p, &(*an)->d[5].p, e);
}

static ast_stat aply(ast *a, te *pn, void **vn, te **e) {
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

static ast *ast_tkn(ast *a) {
    ast_t_a(a, TCUST(ADD), OP(ADD));
    ast_t_a(a, TCUST(SUB), OP(SUB));
    return a;
}

ast *ast_b(ast *a) {
    ast_a(a, NODE_TYPE(ROOT), root);
    ast_a(a, NODE_TYPE(INT), i);
    ast_a(a, NODE_TYPE(OP), o);
    ast_a(a, NODE_TYPE(APLY), aply);
    return ast_tkn(a);
}

void ast_p(const te *an, size_t idnt) {
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    switch (an->d[2].u6) {
        case AST_CLS(R):
            ast_p(an->d[4].p, idnt);
            break;
        case AST_CLS(T):
            break;
        case AST_CLS(I):
            break;
        case AST_CLS(S):
            printf("(S ");
            type_p(an->d[3].p);
            switch (((te*) an->d[3].p)->d[0].u6) {
                case TYPE(I6):
                    printf(" %ld", an->d[4].i6);
                    break;
                default:
                    printf("INV");
                    break;
            }
            putchar(')');
            break;
        case AST_CLS(V):
            break;
        case AST_CLS(O):
            printf("(O ");
            type_p(an->d[3].p);
            printf(" [%s]", ast_op_str(an->d[6].u6));
            putchar(')');
            break;
        case AST_CLS(Z):
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
            break;
        default:
            printf("INV");
            break;
    }
}

bool ast_eq(const te *a, const te *b) {
    if (!a && !b) return true;
    if (!a || !b || a->d[2].u6 != b->d[2].u6) return false;
    switch (a->d[2].u6) {
        case AST_CLS(R):
            return ast_eq(a->d[4].p, b->d[4].p);
        case AST_CLS(T):
            break;
        case AST_CLS(I):
            break;
        case AST_CLS(S):
            break;
        case AST_CLS(V):
            break;
        case AST_CLS(O):
            break;
        case AST_CLS(Z):
            break;
        case AST_CLS(A):
            break;
        case AST_CLS(L):
            break;
        default:
            break;
    }
    return false;
}
