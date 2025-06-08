
#include "chk.h"

chk *chk_i(const alfr *af, const alfr *ta, frfn *cef, chk_tbl_i cti, ast *a, tbl *ct) {
    chk *c = af->a(sizeof(chk));
    c->af = af;
    c->ta = ta;
    c->cef = cef;
    c->cti = cti;
    c->a = *a;
    c->ct = cti();
    return c;
}

void chk_f(chk *c) {
    if (!c || --c->r > 0) return;
    tbl_f(c->ct);
    c->af->f(c);
}
