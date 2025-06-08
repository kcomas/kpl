
#include "chk.h"

chk *chk_i(const alfr *af, const alfr *ta, chk_tbl_i cti, ast *a) {
    chk *c = af->a(sizeof(chk));
    c->r = 1;
    c->af = af;
    c->ta = ta;
    c->cti = cti;
    c->a = ast_c(a);
    c->ct = cti();
    return c;
}

static void chk_entry_f(void *p) {
    te *t = p;
    tbl_f(t->d[1].p);
    t->af->f(t);
}

const uint8_t chk_cls_conts[AST_CLS(_)] = {
    [AST_CLS(R)] = 1,
    [AST_CLS(T)] = 0,
    [AST_CLS(E)] = 0,
    [AST_CLS(I)] = 0,
    [AST_CLS(S)] = 1,
    [AST_CLS(V)] = 1,
    [AST_CLS(O)] = 3,
    [AST_CLS(Z)] = 2,
    [AST_CLS(A)] = 2,
    [AST_CLS(L)] = 1,
    [AST_CLS(C)] = 2
};

static void add_entry(chk *c, te **kv, uint16_t cls, uint16_t type, uint8_t n) {
    un hsh = U6(0);
    hsh = u4_s_o(hsh, cls, 1);
    hsh = u4_s_o(hsh, type, 0);
    if (tbl_g_i(c->ct, hsh, kv) == TBL_STAT(NF)) {
        *kv = te_i(2, c->ta, n > 0 ? chk_entry_f : NULL);
        (*kv)->d[0] = hsh;
        if (n > 0) (*kv)->d[1] = P(c->cti());
    }
}

void chk_a(chk *c, chk_fn cf, uint16_t cls, uint16_t type, ...) {
    uint8_t n = chk_cls_conts[cls];
    te *kv;
    add_entry(c, &kv, cls, type, n);
    va_list args;
    va_start(args, type);
    while (n > 0) {
        n--;
        cls = va_arg(args, int);
        type = va_arg(args, int);
        add_entry(c, &kv, cls, type, n);
    }
    va_end(args);
    kv->d[1] = P(cf);
}

void chk_f(chk *c) {
    if (!c || --c->r > 0) return;
    ast_f(c->a);
    tbl_f(c->ct);
    c->af->f(c);
}
