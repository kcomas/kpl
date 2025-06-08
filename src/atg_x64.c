
#include "atg_x64.h"

static void atg_add_e_p(const tbl *t, size_t n, size_t idnt) {
    n--;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    te *h = t->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t hc = u4_g_o(kv->d[0], AST_HSH_C);
        uint16_t ht = u4_g_o(kv->d[0], AST_HSH_T);
        printf("[%s %s]", ast_cls_str(hc), type_str(ht));
        if (n > 0) {
            putchar('\n');
            atg_add_e_p(kv->d[1].p, n, idnt + 1);
        }
        h = h->d[2].p;
    }
}

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt) {
    te *h = t->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t hc = u4_g_o(kv->d[0], 1);
        uint16_t ht = u4_g_o(kv->d[0], 0);
        putchar('(');
        switch (cls) {
            case AST_CLS(O):
                printf("[%s %s]\n", ast_oc_str(hc), type_str(ht));
                atg_add_e_p(kv->d[1].p, 2, idnt + 1);
                break;
            default:
                printf("INV ATG P)\n");
                return;
        }
        printf(")\n");
        h = h->d[2].p;
    }
}

static bool lst_cst_t(const te *an) {
    return an->d[4].u4 == OC(CST) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L);
}

bool atg_x64_enq(const te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(O):
            return lst_cst_t(an);
        case AST_CLS(L):
            return an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(R);
        default:
            break;
    }
    return false;
}

static atg_stat lst_cst_s(atg *t, gen *g, te *an, te **e) {
    (void) t;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    // TODO zero ref counted vars
    return ATG_STAT(OK);
}

static atg_stat lst_cst_e(atg *t, gen *g, te *an, te **e) {
    (void) t;
    (void) an;
    (void) g;
    (void) e;
    HERE("TODO");
    return ATG_STAT(OK);
}

static te *var_arg(gen *g, te *lte, x64_type xt) {
    uint32_t id = ast_lst_tbl_e_g_i(lte);
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (flgs & LTE_FLG(A)) return gen_arg(g, xt, id);
    // TODO var
    return NULL;
}

static atg_stat add_i6_e_i6_e_i6(atg *t, gen *g, te *an, te **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[5].p, e);
    if (!(r = var_arg(g, r, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[6].p, e);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), l, r) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_o_i6(atg *t, gen *g, te *an, te **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = te_c(((te*) g->code->t->d[0].p)->d[1].p);
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(ATG_STAT(INV), an->d[5].p, e);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), l, r) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    return ATG_STAT(OK);
}

static atg_stat neg_i6_o_i6(atg *t, gen *g, te *an, te **e) {
    HERE("TODO");
}

atg *atg_b(atg *t) {
    atg_a_se(t, lst_cst_t, lst_cst_s, lst_cst_e);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), add_i6_e_i6_e_i6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), add_i6_e_i6_o_i6);
    atg_a_o(t, OC(SUB), TYPE(I6), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6), neg_i6_o_i6);
    return t;
}
