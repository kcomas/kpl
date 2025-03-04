
#include "chk.h"

static void chk_p_conts(const tbl *ct, ast_cls base, size_t n, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        putchar('\n');
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], CHK_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], CHK_HSH_T);
        if (n == 2 && base == AST_CLS(O)) printf("[%s %s]", ast_oc_str(cls), type_str(type));
        else printf("[%s %s]", ast_cls_str(cls), type_str(type));
        if (n > 0) chk_p_conts(kv->d[1].p, cls, n - 1, idnt + 1);
        h = h->d[2].p;
    }
}

void chk_p(const tbl *ct, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], CHK_HSH_C);
        uint16_t type = u4_g_o(kv->d[0], CHK_HSH_T);
        printf("([%s %s]", ast_cls_str(cls), type_str(type));
        chk_p_conts(kv->d[1].p, cls, chk_cls_conts[cls] - 1, idnt + 1);
        printf(")\n");
        h = h->d[2].p;
    }
}

static chk_stat chk_cst_fn_lst_b(chk *c, te *an, te **e) {
    (void) c;
    (void) e;
    tbl *fat = ((te*) ((te*) an->d[5].p)->d[3].p)->d[3].p; // fn args tbl
    tbl *lt = ((te*) an->d[6].p)->d[3].p;
    te *h = lt->i->h, *lei, *kv;
    while (h) {
        lei = h->d[0].p;
        if (tbl_g_i(fat, lei->d[0], &kv) == TBL_STAT(OK)) lei->d[2] = P(te_c(kv->d[2].p));
        h = h->d[2].p;
    }
    return CHK_STAT(OK);
}

static chk_stat chk_nop(chk *c, te *an, te **e) {
    (void) c;
    (void) an;
    (void) e;
    return CHK_STAT(OK);
}

static chk_stat chk_op_e_eq(chk *c, te *an, te **e) {
    (void) c;
    (void) an;
    (void) e;
    printf("%s", __FUNCTION__);
    return CHK_STAT(INV);
}

chk *chk_b(chk *c) {
    // before
    CHK_AB(c, chk_cst_fn_lst_b, AST_CLS(O), TYPE(_N), OC(CST), TYPE(_A), AST_CLS(T), TYPE(FN), AST_CLS(L), TYPE(_A));
    // after
    CHK_AA(c, chk_nop, AST_CLS(R), TYPE(_A), AST_CLS(A), TYPE(_N));
    // ops
    CHK_AA(c, chk_op_e_eq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    return c;
}
