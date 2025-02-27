
#include "chk.h"

chk *chk_i(const alfr *af, const alfr *ta, chk_tbl_i cti, ast *a) {
    chk *c = af->a(sizeof(chk));
    c->af = af;
    c->ta = ta;
    c->cti = cti;
    c->a = a;
    c->ct = cti();
    return c;
}

static void chk_tbl_entry_f(void *p) {
    // TODO
}

chk_stat chk_a(chk *c, ast_cls cls, un ct, ...) {
    te *kv;
    if (tbl_g_i(c->ct, U6(cls), &kv) == TBL_STAT(NF)) {
        kv = te_i(3, c->ta, chk_tbl_entry_f);
        kv->d[0] = U6(cls);
    }
}

void chk_f(chk *c) {
    if (!c || --c->r > 0) return;
    tbl_f(c->ct);
    c->af->f(c);
}
