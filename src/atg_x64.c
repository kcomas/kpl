
#include "atg_x64.h"

static atg_add_e_p(const tbl *t, size_t n, size_t idnt) {
    n--;
    for (size_t i = 0; i < idnt; i++) putchar(' ');
    te *h = t->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t hc = u4_g_o(kv->d[0], 1);
        uint16_t ht = u4_g_o(kv->d[0], 0);
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

atg_stat atg_err(atg_stat stat, te *an, te **e) {
    *e = te_c(an);
    return stat;
}

static atg_stat lst_cst_s(atg *t, gen *g, te *an, te **e) {
    (void) t;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, t->lc++), NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
    if (gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL) != GEN_STAT(OK)) return atg_err(ATG_STAT(INV), an, e);
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


static atg_stat add_i6_e_i6_e_i6(atg *t, gen *g, te *an, te **e) {
    HERE("TODO");
    return ATG_STAT(INV);
}

atg *atg_b(atg *t) {
    atg_a_se(t, lst_cst_t, lst_cst_s, lst_cst_e);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), add_i6_e_i6_e_i6);
    return t;
}
