
#include "chk.h"

static void chk_p_n(const tbl *ct, ast_cls cls, size_t n, size_t idnt) {
    te *kv;
    switch (cls) {
        case AST_CLS(OP):
            break;
        case AST_CLS(CMD):
        default:

    }
}

void chk_p(const tbl *ct, ast_cls cls, size_t idnt) {
    if (cls >= AST_CLS(_)) {
        printf("INV CLS");
        return;
    }
    return chk_p_n(ct, cls, chk_cls_conts[cls] + 1, idnt);
}

static chk_stat chk_root_lst(chk *c, te *an) {
    (void) c;
    (void) an;
    return CHK_STAT(INV);
}

chk *chk_b(chk *c) {
    chk_a(c, AST_CLS(R), TYPE(_A), AST_CLS(L), TYPE(_A), chk_root_lst);
    return c;
}
