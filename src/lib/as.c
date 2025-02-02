
#include "as.h"

te *arg_i(size_t id, un d, alfn *aa, frfn *af) {
    te *a = te_i(2, aa, af);
    a->d[0] = U6(id);
    a->d[1] = d;
    return a;
}

as *as_i(alfn *aa, frfn *af, frfn *lef, frfn *oef, frfn *cf, op_tbl_i *oti, tbl *lbls, lst *code) {
    as *a = aa(sizeof(as));
    a->r = 1;
    a->lc = LABEL(USER);
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

static te *add_code(as *a, code_id cid, size_t op_lbl_id, te *arg1, te *arg2, te *arg3, te *arg4, as_code_fn *fn, as_code_fn *lbl_fn) {
    te *c = te_i(10, a->aa, a->cf);
    c->d[0] = U6(cid);
    c->d[1] = U6(op_lbl_id);
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

static size_t add_lbl(as *a, size_t lbl_id) {
    te *c = te_c(add_code(a, CODE_ID(L), lbl_id == LABEL(UN) ? a->lc++ : lbl_id, NULL, NULL, NULL, NULL, NULL, NULL));
    te *lbl = te_i(3, a->aa, a->lef);
    lbl->d[0] = c->d[1];
    lbl->d[1] = P(c);
    lbl->d[2] = P(lst_i(a->aa, a->aa, a->af, (void*) &te_f, a->af));
    tbl_a(a->lbls, lbl);
    return lbl->d[0].u6;
}

size_t as_lbl_a(as *a, size_t lbl_id) {
    te *kv;
    if (lbl_id == LABEL(UN)) return add_lbl(a, lbl_id);
    if (tbl_g_i(a->lbls, U6(lbl_id), &kv) == TBL_STAT(OK)) return ((te*) kv->d[1].p)->d[0].u6;
    return add_lbl(a, lbl_id);
}

static te *get_lbl(as *a, size_t id) {
    te *lbl = NULL;
    if (tbl_g_i(a->lbls, U6(id), &lbl) == TBL_STAT(NF)) return NULL;
    return lbl;
}

te *as_lbl_g_c(as *a, size_t lbl_id) {
    te *lbl = get_lbl(a, lbl_id);
    if (!lbl) return NULL;
    return (te*) lbl->d[1].p;
}

as_stat as_lbl_s_c(as *a, size_t lbl_id, te *c) {
    te *lbl = get_lbl(a, lbl_id);
    if (!lbl) return AS_STAT(INV);
    lst_ab(lbl->d[2].p, P(te_c(c)));
    return AS_STAT(OK);
}

static tbl *add_op_entry(as *a, tbl *co, te **kv, size_t id) {
    *kv = te_i(4, a->aa, a->oef);
    (*kv)->d[0] = U6(id);
    (*kv)->d[3] = P(a->oti());
    tbl_a(co, *kv);
    return (*kv)->d[3].p;
}

as_stat as_op_a(as *a, size_t op_id, size_t ai1, size_t ai2, size_t ai3, size_t ai4, as_code_fn *fn, as_lbl_fn *lbl_fn) {
    if (op_id == LABEL(UN)) return AS_STAT(INV);
    tbl *co = a->ops;
    te *kv;
    if (tbl_g_i(co, U6(op_id), &kv) == TBL_STAT(NF)) co = add_op_entry(a, co, &kv, op_id);
    else co = kv->d[3].p;
    const size_t args[] = {ai1, ai2, ai3, ai4};
    for (size_t i = 0; i < 4; i++) {
        if (!args[i]) break;
        if (tbl_g_i(co, U6(args[i]), &kv) == TBL_STAT(NF)) co = add_op_entry(a, co, &kv, args[i]);
        else co = kv->d[3].p;
    }
    kv->d[1] = P(fn);
    kv->d[2] = P(lbl_fn);
    return AS_STAT(OK);
}

as_stat as_a(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
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

as_stat as_n(as *a, uint8_t *m) {
    size_t p = 0;
    te *h = a->code->h;
    while (h) {
        te *c = h->d[0].p;
        c->d[8] = U6(p);
        if (c->d[0].u6 == CODE_ID(O)) {
            as_code_fn *fn = (as_code_fn*) c->d[6].p;
            if (!fn || !fn(a, c, &p, m, c->d[2].p, c->d[3].p, c->d[4].p, c->d[5].p)) return AS_STAT(INV);
            c->d[9] = U6(p - c->d[8].u6);
        } else c->d[9] = U6(1);
        h = h->d[2].p;
    }
    h = a->lbls->i->h;
    while (h) {
        te *lbl = h->d[0].p;
        te *lh = ((lst*) lbl->d[2].p)->h;
        while (lh) {
            as_lbl_fn *lfn = (as_lbl_fn*) ((te*) lh->d[0].p)->d[7].p;
            if (!lfn || !lfn(a, m, lbl->d[1].p, lh->d[0].p)) return AS_STAT(INV);
            lh = lh->d[2].p;
        }
        h = h->d[2].p;
    }
    return AS_STAT(OK);
}

void as_f(as *a) {
    if (!a || --a->r > 0) return;
    tbl_f(a->lbls);
    tbl_f(a->ops);
    lst_f(a->code);
    a->af(a);
}
