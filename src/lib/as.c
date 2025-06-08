
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

static te *add_code(as *const a, code_id cid, size_t op_lbl_id, te *arg1, te *arg2, te *arg3, te *arg4, as_code_fn *fn, as_code_fn *lbl_fn) {
    te *c = te_i(10, a->aa, a->cf);
    c->d[0] = U6(cid);
    c->d[1] = U6(op_lbl_id == LABEL(UN) ? a->lc++ : op_lbl_id);
    c->d[2] = P(arg1);
    c->d[3] = P(arg2);
    c->d[4] = P(arg3);
    c->d[5] = P(arg4);
    c->d[6] = P(fn);
    c->d[7] = P(lbl_fn);
    // 8 pos
    // 9 len
    lst_ab(a->code, P(c));
    return c;
}

static size_t add_lbl(as *const a, size_t lbl_id) {
    te *c = te_c(add_code(a, CODE_ID(L), lbl_id, NULL, NULL, NULL, NULL, NULL, NULL));
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

as_stat as_lbl_g_i(as *const a, size_t lbl_id, te **lbl) {
    if (tbl_g_i(a->lbls, U6(lbl_id), lbl) == TBL_STAT(NF)) return AS_STAT(INV);
    return AS_STAT(OK);
}

static tbl *add_op_entry(as *const a, tbl *const co, te **kv, size_t id) {
    *kv = te_i(4, a->aa, a->oef);
    (*kv)->d[0] = U6(id);
    (*kv)->d[3] = P(a->oti());
    tbl_a(co, *kv);
    return (*kv)->d[3].p;
}

as_stat as_op_a(as *const a, size_t op_id, arg_id ai1, arg_id ai2, arg_id ai3, arg_id ai4, as_code_fn *fn, as_code_fn *lbl_fn) {
    if (op_id == LABEL(UN)) return AS_STAT(INV);
    tbl *co = a->ops;
    te *kv;
    if (tbl_g_i(co, U6(op_id), &kv) == TBL_STAT(NF)) co = add_op_entry(a, co, &kv, op_id);
    else co = kv->d[3].p;
    const arg_id args[] = {ai1, ai2, ai3, ai4};
    for (size_t i = 0; i < 4; i++) {
        if (args[i] == ARG_ID(N)) break;
        if (tbl_g_i(co, U6(args[i]), &kv) == TBL_STAT(NF)) co = add_op_entry(a, co, &kv, args[i]);
        else co = kv->d[3].p;
    }
    kv->d[1] = P(fn);
    kv->d[2] = P(lbl_fn);
    return AS_STAT(OK);
}

as_stat as_a(as *const a, size_t op_id, te *arg1, te *arg2, te *arg3, te *arg4) {
    tbl *co = a->ops;
    te *kv;
    as_code_fn *fn = NULL, *lbl_fn = NULL;
    if (tbl_g_i(co, U6(op_id), &kv) == TBL_STAT(NF)) return AS_STAT(INV);
    co = kv->d[3].p;
    const te *args[] = {arg1, arg2, arg3, arg4};
    for (size_t i = 0; i < 4; i++) {
        if (!args[i] || tbl_g_i(co, U6(args[i]->d[0].u6), &kv) == TBL_STAT(NF)) break;
        co = kv->d[3].p;
    }
    fn = kv->d[1].p;
    if (!fn) return AS_STAT(INV);
    lbl_fn = kv->d[2].p;
    add_code(a, CODE_ID(O), op_id, arg1, arg2, arg3, arg4, fn, lbl_fn);
    return AS_STAT(OK);
}

as_stat as_n(as *const a, uint8_t *m) {
    size_t p = 0;
    te *h = a->code->h;
    while (h) {
        te *c = h->d[0].p;
        c->d[8] = U6(p);
        if (c->d[0].u6 == CODE_ID(O)) {
            as_code_fn *fn = (as_code_fn*) c->d[6].p;
            if (!fn(a, &p, m, c->d[2].p, c->d[3].p, c->d[4].p, c->d[5].p)) return AS_STAT(INV);
        }
        c->d[9] = U6(p - c->d[8].u6);
        h = h->d[2].p;
    }
    // TODO fill labels
    return AS_STAT(OK);
}

void as_f(as *a) {
    if (!a || --a->r > 0) return;
    tbl_f(a->lbls);
    tbl_f(a->ops);
    lst_f(a->code);
    a->af(a);
}
