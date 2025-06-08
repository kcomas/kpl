
#include "chk.h"

void chk_p(const tbl *ct, size_t idnt) {
    te *h = ct->i->h;
    while (h) {
        te *kv = h->d[0].p;
        uint16_t cls = u4_g_o(kv->d[0], 1);
        uint16_t type = u4_g_o(kv->d[0], 0);
        printf("(%s %s)", ast_cls_str(cls), type_str(type));
        putchar('\n');
        h = h->d[2].p;
    }
}

static chk_stat chk_root_lst(chk *c, te *an) {
    (void) c;
    (void) an;
    return CHK_STAT(INV);
}

chk *chk_b(chk *c) {
    chk_a(c, chk_root_lst, AST_CLS(S), TYPE(_A), AST_CLS(L), TYPE(_A));
    return c;
}
