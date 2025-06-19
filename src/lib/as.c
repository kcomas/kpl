
#include "as.h"

as *as_i(const alfr *af, const alfr *ta, const alfr *la, const alfr *ea, as_mem_fn msf, as_mem_fn mef, as_err_g_p gep, op_tbl_i oti, tbl *lbls, lst *code) {
    as *a = af->a(sizeof(as));
    a->r = 1;
    a->af = af;
    a->ta = ta;
    a->la = la;
    a->ea = ea;
    a->msf = msf;
    a->mef = mef;
    a->gep = gep;
    a->oti = oti;
    a->lbls = lbls;
    a->ops = oti();
    a->dq = lst_i(la, ta, (void*) te_f);
    a->code = code;
    return a;
}

as *as_i_as(const as *a) {
    as *aa = a->af->a(sizeof(as));
    aa->r = 1;
    aa->af = a->af;
    aa->ta = a->ta;
    aa->la = a->la;
    aa->ea = a->ea;
    aa->msf = a->msf;
    aa->mef = a->mef;
    aa->gep = a->gep;
    aa->oti = a->oti;
    aa->lbls = tbl_i_tbl(a->lbls);
    aa->ops = tbl_c(a->ops);
    aa->dq = lst_i_lst(a->dq);
    aa->code = lst_i_lst(a->code);
    return aa;
}

static void as_code_entry_f(void *p) {
    te *o = p;
    te_f(o->d[2].p);
    te_f(o->d[3].p);
    te_f(o->d[4].p);
    te_f(o->d[5].p);
    o->af->f(o);
}

static te *add_code(as *a, code_id cid, size_t op_lbl_id, te *arg1, te *arg2, te *arg3, te *arg4, as_code_fn *fn, as_code_fn *lbl_fn) {
    te *c = te_i(10, a->ta, as_code_entry_f);
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

static void as_label_entry_f(void *p) {
    te *l = p;
    te_f(l->d[1].p);
    lst_f(l->d[2].p);
    l->af->f(l);
}

static size_t add_lbl(as *a, size_t lbl_id) {
    te *c = te_c(add_code(a, CODE_ID(L), lbl_id, NULL, NULL, NULL, NULL, NULL, NULL));
    te *lbl = te_i(3, a->ta, as_label_entry_f);
    lbl->d[0] = c->d[1];
    lbl->d[1] = P(c);
    lbl->d[2] = P(lst_i(a->la, a->ta, (void*) te_f));
    tbl_a(a->lbls, lbl);
    return lbl->d[0].u6;
}

size_t as_lbl_a(as *a, size_t lbl_id) {
    te *kv;
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

ssize_t as_lbl_g_c_i(as *a, size_t lbl_id) {
    te *lbl = get_lbl(a, lbl_id);
    if (!lbl) return -1;
    return ((te*) lbl->d[1].p)->d[8].u6;
}

as_stat as_lbl_s_c(as *a, size_t lbl_id, te *c) {
    te *lbl = get_lbl(a, lbl_id);
    if (!lbl) return AS_STAT(INV);
    lst_ab(lbl->d[2].p, P(te_c(c)));
    return AS_STAT(OK);
}

static void as_op_entry_f(void *p) {
    te *oe = p;
    tbl_f(oe->d[3].p);
    oe->af->f(oe);
}

static tbl *add_op_entry(as *a, tbl *co, te **kv, size_t id) {
    *kv = te_i(4, a->ta, as_op_entry_f);
    (*kv)->d[0] = U6(id);
    (*kv)->d[3] = P(a->oti());
    tbl_a(co, *kv);
    return (*kv)->d[3].p;
}

as_stat as_op_a(as *a, size_t op_id, size_t ai1, size_t ai2, size_t ai3, size_t ai4, as_code_fn *fn, as_lbl_fn *lbl_fn) {
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

static void as_dq_entry_f(void *p) {
    te *d = p;
    te_f(d->d[0].p);
    d->af->f(d);
}

void as_dq_a(as *a, te *ci, size_t size, un v, as_dq_fn dq_fn) {
    te *d = te_i(5, a->ta, as_dq_entry_f);
    d->d[0] = P(te_c(ci));
    d->d[1] = U6(size);
    d->d[2] = v;
    d->d[3] = P(dq_fn);
    // 4 pos
    lst_ab(a->dq, P(d));
}

as_stat as_a(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    tbl *co = a->ops;
    te *kv;
    as_code_fn *fn = NULL, *lbl_fn = NULL;
    if (tbl_g_i(co, U6(op_id), &kv) == TBL_STAT(NF)) return AS_STAT(INV);
    co = kv->d[3].p;
    const te *args[] = {arg1, arg2, arg3, arg4};
    for (size_t i = 0; i < 4; i++) {
        if (!args[i]) break;
        if (tbl_g_i(co, args[i]->d[0], &kv) == TBL_STAT(NF)) return AS_STAT(INV);
        co = kv->d[3].p;
    }
    fn = kv->d[1].p;
    if (!fn) return AS_STAT(INV);
    lbl_fn = kv->d[2].p;
    add_code(a, CODE_ID(O), op_id, arg1, arg2, arg3, arg4, fn, lbl_fn);
    return AS_STAT(OK);
}

as_stat as_n(as *a, size_t *p, size_t ms, uint8_t *m, err **e) {
    a->msf(ms, m);
    te *h = a->code->h;
    while (h) {
        te *c = h->d[0].p;
        c->d[8] = U6(*p);
        if (c->d[0].u6 == CODE_ID(O)) {
            as_code_fn *fn = c->d[6].p;
            if (!fn || !fn(a, c, p, m, c->d[2].p, c->d[3].p, c->d[4].p, c->d[5].p)) {
                *e = err_i(a->ea, a->gep(AS_STAT(CODE)), (void*) te_f, te_c(c), "as code");
                a->mef(ms, m);
                return AS_STAT(INV);
            }
            c->d[9] = U6(*p - c->d[8].u6);
        } else c->d[9] = U6(1);
        h = h->d[2].p;
    }
    h = a->lbls->i->h;
    while (h) {
        te *lbl = h->d[0].p;
        te *lh = ((lst*) lbl->d[2].p)->h;
        while (lh) {
            as_lbl_fn *lfn = ((te*) lh->d[0].p)->d[7].p;
            if (!lfn || !lfn(a, m, lbl->d[1].p, lh->d[0].p)) {
                *e = err_i(a->ea, a->gep(AS_STAT(LBL)), (void*) te_f, te_c(lbl), "as lbl");
                a->mef(ms, m);
                return AS_STAT(INV);
            }
            lh = lh->d[2].p;
        }
        h = h->d[2].p;
    }
    h = a->dq->h;
    size_t mod = *p & 0x0F;
    if (mod) *p = *p - mod + 0x10;
    while (h) {
        te *dqe = h->d[0].p;
        as_dq_fn *dfn = dqe->d[3].p;
        if (!dfn || !dfn(a, p, m, dqe)) {
            *e = err_i(a->ea, a->gep(AS_STAT(DATA)), (void*) te_f, te_c(dqe), "as data");
            a->mef(ms, m);
            return AS_STAT(INV);
        }
        h = h->d[2].p;
    }
    a->mef(ms, m);
    return AS_STAT(OK);
}

void as_f(as *a) {
    if (!a || --a->r > 0) return;
    tbl_f(a->lbls);
    tbl_f(a->ops);
    lst_f(a->dq);
    lst_f(a->code);
    a->af->f(a);
}
