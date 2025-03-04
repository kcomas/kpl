
#include "chk.h"

chk *chk_i(const alfr *af, const alfr *ta, chk_tbl_i cti, ast *a) {
    chk *c = af->a(sizeof(chk));
    c->r = 1;
    c->af = af;
    c->ta = ta;
    c->cti = cti;
    c->a = ast_c(a);
    c->bt = cti();
    c->at = cti();
    return c;
}

static void chk_entry_f(void *p) {
    te *t = p;
    tbl_f(t->d[1].p);
    t->af->f(t);
}

// zero for no chk
const uint8_t chk_cls_conts[AST_CLS(_)] = {
    [AST_CLS(R)] = 1,
    [AST_CLS(T)] = 0,
    [AST_CLS(E)] = 0,
    [AST_CLS(I)] = 0,
    [AST_CLS(S)] = 1,
    [AST_CLS(V)] = 1,
    [AST_CLS(O)] = 3,
    [AST_CLS(Z)] = 1,
    [AST_CLS(A)] = 1,
    [AST_CLS(L)] = 0,
    [AST_CLS(C)] = 0
};

static void add_entry(chk *c, tbl *ct, te **kv, uint16_t cls, uint16_t type, uint8_t n) {
    un hsh = U6(0);
    hsh = u4_s_o(hsh, CHK_HSH_C, cls);
    hsh = u4_s_o(hsh, CHK_HSH_T, type);
    if (tbl_g_i(ct, hsh, kv) == TBL_STAT(NF)) {
        *kv = te_i(2, c->ta, n > 0 ? chk_entry_f : NULL);
        (*kv)->d[0] = hsh;
        if (n > 0) (*kv)->d[1] = P(c->cti());
        tbl_a(ct, *kv);
    }
}

chk_stat chk_a(chk *c, tbl *t, chk_fn cf, uint16_t cls, uint16_t type, ...) {
    tbl *ct = t;
    uint8_t n = chk_cls_conts[cls];
    if (!n) return CHK_STAT(INV);
    te *kv;
    add_entry(c, ct, &kv, cls, type, n);
    va_list args;
    va_start(args, type);
    while (n > 0) {
        n--;
        ct = kv->d[1].p;
        cls = va_arg(args, int);
        type = va_arg(args, int);
        add_entry(c, ct, &kv, cls, type, n);
    }
    va_end(args);
    if (kv->d[1].p) return CHK_STAT(INV);
    kv->d[1] = P(cf);
    return CHK_STAT(OK);
}

static un chk_hsh(const te *an) {
    un hsh = U6(0);
    if (!an) {
        hsh = u4_s_o(hsh, CHK_HSH_C, AST_CLS(_));
        hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_N));
        return hsh;
    }
    hsh = u4_s_o(hsh, CHK_HSH_C, an->d[2].u4);
    switch (an->d[2].u4) {
        case AST_CLS(R):
        case AST_CLS(L):
            hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_A));
            break;
        case AST_CLS(E):
            if (!((te*) an->d[3].p)->d[2].p) hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_N));
            else hsh = u4_s_o(hsh, CHK_HSH_T, ((te*) ((te*) an->d[3].p)->d[2].p)->d[1].u4);
            break;
        default:
            if (!an->d[3].p) hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_N));
            else hsh = u4_s_o(hsh, CHK_HSH_T, ((te*) an->d[3].p)->d[1].u4);
            break;
    }
    return hsh;
}

static chk_stat chk_foe(bool foe, te *an, te **e) {
    if (foe) *e = an;
    return foe ? CHK_STAT(INV) : CHK_STAT(OK);
}

static chk_stat chk_r(chk *c, tbl *t, te *an, te **e, uint8_t n, uint8_t ncmp, bool foe) { // if we fail on exit
    un hsh = chk_hsh(an);
    te *kv;
    if (tbl_g_i(t, hsh, &kv) == TBL_STAT(NF)) return chk_foe(foe, an, e);
    while (n > 0) {
        hsh = U6(0);
        n--;
        t = kv->d[1].p;
        if (!t) return chk_foe(foe, an, e);
        if (n == 2 && an->d[2].u4 == AST_CLS(O)) {
            hsh = u4_s_o(hsh, CHK_HSH_C, an->d[ncmp++].u4);
            hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_A));
        } else if (n == 0 && (an->d[2].u4 == AST_CLS(S) || an->d[2].u4 == AST_CLS(V))) {
            hsh = u4_s_o(hsh, CHK_HSH_C, AST_CLS(_));
            hsh = u4_s_o(hsh, CHK_HSH_T, TYPE(_A));
        } else hsh = chk_hsh(an->d[ncmp++].p);
        if (tbl_g_i(t, hsh, &kv) == TBL_STAT(NF)) return chk_foe(foe, an, e);
    }
    if (!kv->d[1].p) return chk_foe(foe, an, e);
    return ((chk_fn*) kv->d[1].p)(c, an, e);
}

static chk_stat chk_lst_n(chk *c, lst *l, te **e) {
    chk_stat stat = CHK_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = chk_n(c, h->d[0].p, e)) != CHK_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

chk_stat chk_n(chk *c, te *an, te **e) {
    chk_stat stat = CHK_STAT(OK);
    if (!an) return stat;
    const uint8_t n = chk_cls_conts[an->d[2].u4], ncmp = AST_MIN_LEN;
    if (n && (stat = chk_r(c, c->bt, an, e, n, ncmp, false)) != CHK_STAT(OK)) return stat;
    switch (an->d[2].u4) {
        case AST_CLS(R):
            if ((stat = chk_n(c, an->d[4].p, e)) != CHK_STAT(OK)) return stat;
            break;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(S):
            break;
        case AST_CLS(V):
            // TODO
            break;
        case AST_CLS(O):
            if ((stat = chk_n(c, an->d[5].p, e)) != CHK_STAT(OK) || (stat = chk_n(c, an->d[6].p, e)) != CHK_STAT(OK)) return stat;
            break;
        case AST_CLS(Z):
            if ((stat = chk_n(c, an->d[4].p, e)) != CHK_STAT(OK)) return stat;
            break;
        case AST_CLS(A):
            if ((stat = chk_n(c, an->d[4].p, e)) != CHK_STAT(OK) || (stat = chk_lst_n(c, an->d[5].p, e)) != CHK_STAT(OK)) return stat;
            break;
        case AST_CLS(L):
            if ((stat = chk_lst_n(c, an->d[4].p, e)) != CHK_STAT(OK)) return stat;
            break;
        default:
            return CHK_STAT(INV); // nodes should have been removed
    }
    if (!n) return CHK_STAT(OK); // do not test
    return chk_r(c, c->at, an, e, n, ncmp, true);
}

void chk_f(chk *c) {
    if (!c || --c->r > 0) return;
    ast_f(c->a);
    tbl_f(c->bt);
    tbl_f(c->at);
    c->af->f(c);
}
