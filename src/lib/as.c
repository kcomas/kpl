
#include "as.h"

te *arg_i(arg_id id, un d, alfn *aa, frfn *af) {
    te *a = te_i(2, aa, af);
    a->d[0] = U6(id);
    a->d[1] = d;
    return a;
}

as *as_i(alfn *aa, frfn *af, frfn *lef, frfn *oef, frfn *cf, op_tbl_i *oti, tbl *lbls, lst *code) {
    as *a = aa(sizeof(as));
    a->r = 1;
    a->lc = LABEL(_);
    a->aa = aa;
    a->af = af;
    a->lef = lef;
    a->oef = oef;
    a->cf = cf;
    a->oti = oti;
    a->lbls = lbls;
    a->ops = oti();
    a->code = code;
    return a;
}

static te *add_code(as *const a, code_id cid, size_t op_lbl_id, te *arg1, te *arg2, te *arg3, te *arg4, as_code_fn *fn) {
    te *c = te_i(9, a->aa, a->cf);
    c->d[0] = U6(cid);
    c->d[1] = U6(op_lbl_id == LABEL(UN) ? a->lc++ : op_lbl_id);
    c->d[2] = P(arg1);
    c->d[3] = P(arg2);
    c->d[4] = P(arg3);
    c->d[5] = P(arg4);
    c->d[6] = P(fn);
    // 7 pos
    // 8 len
    lst_ab(a->code, P(c));
    return te_c(c);
}

static size_t add_lbl(as *const a, size_t lbl_id) {
    te *c = add_code(a, CODE_ID(L), lbl_id, NULL, NULL, NULL, NULL, NULL);
    te *lbl = te_i(3, a->aa, a->lef);
    lbl->d[0] = c->d[1];
    lbl->d[1] = P(c);
    // TODO add lst?
    tbl_a(a->lbls, lbl);
    return lbl->d[0].u6;
}

size_t as_lbl_a(as *const a, size_t lbl_id) {
    te *kv;
    if (lbl_id == LABEL(UN)) return add_lbl(a, lbl_id);
    if (tbl_g_i(a->lbls, U6(lbl_id), &kv) == TBL_STAT(OK)) return ((te*) kv->d[1].p)->d[0].u6;
    return add_lbl(a, lbl_id);
}

void as_f(as *a) {
    if (!a || --a->r > 0) return;
    tbl_f(a->lbls);
    tbl_f(a->ops);
    lst_f(a->code);
    a->af(a);
}
