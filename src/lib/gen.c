
#include "gen.h"

const char* gen_cls_str(gen_cls cls) {
    switch (cls) {
        case GEN_CLS(N): return "N";
        case GEN_CLS(M): return "M";
        case GEN_CLS(A): return "A";
        case GEN_CLS(V): return "V";
        case GEN_CLS(T): return "T";
        case GEN_CLS(L): return "L";
        case GEN_CLS(D): return "D";
        default:
            break;
    }
    return "INV";
}

gen *gen_i(const alfr *af, const alfr *ta, cls_tbl_i cti, tbl *oci, lst *code) {
    gen *g = af->a(sizeof(gen));
    g->r = 1;
    g->af = af;
    g->ta = ta;
    g->cti = cti;
    g->oci = oci;
    g->code = code;
    return g;
}

gen *gen_i_gen(const gen *g) {
    return gen_i(g->af, g->ta, g->cti, tbl_c(g->oci), lst_i_lst(g->code));
}

static bool gen_lst_eq(un x, un y) {
    te *a = x.p;
    te *b = y.p;
    if (!a || !b) return false;
    if (a->d[0].u6 != b->d[0].u6) return false;
    for (size_t i = 1; i < 4; i++) {
        te *l = a->d[i].p;
        te *r = b->d[i].p;
        if (!l && !r) continue;
        if ((l && !r) || (!l && r) || l->d[0].u6 != r->d[0].u6 || l->d[1].u6 != r->d[1].u6) return false;
    }
    return true;
}

bool gen_code_eq(const gen *restrict a, const gen *restrict b) {
    return lst_eq(a->code, b->code, gen_lst_eq);
}

static void gen_entry_f(void *p) {
    te *t = p;
    tbl_f(t->d[2].p);
    t->af->f(t);
}

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, uint16_t type1, gen_cls cls2, uint16_t type2, gen_cls cls3, uint16_t type3, gen_fn *fn) {
    tbl *oci = g->oci;
    te *kv;
    if (tbl_g_i(oci, U6(op_id), &kv) == TBL_STAT(NF)) {
        kv = te_i(3, g->ta, gen_entry_f);
        kv->d[0] = U6(op_id);
        kv->d[2] = P(g->cti());
        tbl_a(oci, kv);
        oci = kv->d[2].p;
    } else oci = kv->d[2].p;
    const gen_cls cls[] = {cls1, cls2, cls3};
    const uint16_t type[] = {type1, type2, type3};
    for (size_t i = 0; i < 3; i++) {
        if (cls[i] == GEN_CLS(N)) break;
        un hsh = U6(0);
        hsh = u4_s_o(hsh, 1, cls[i]);
        hsh = u4_s_o(hsh, 0, type[i]);
        if (tbl_g_i(oci, hsh, &kv) == TBL_STAT(NF)) {
            kv = te_i(3, g->ta, gen_entry_f);
            kv->d[0] = hsh;
            kv->d[2] = P(g->cti());
            tbl_a(oci, kv);
            oci = kv->d[2].p;
        } else oci = kv->d[2].p;
    }
    kv->d[1] = P(fn);
    return GEN_STAT(OK);
}

static void gen_code_entry_f(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    te_f(t->d[3].p);
    te_f(t->d[5].p);
    te_f(t->d[6].p);
    t->af->f(t);
}

gen_stat gen_a(gen *g, size_t op_id, te *restrict ac1, te *restrict ac2, te *restrict ac3) {
    tbl *oci = g->oci;
    te *kv;
    gen_fn *fn = NULL;
    if (tbl_g_i(oci, U6(op_id), &kv) == TBL_STAT(NF)) return GEN_STAT(INV);
    oci = kv->d[2].p;
    fn = kv->d[1].p;
    const te *ac[] = {ac1, ac2, ac3};
    for (size_t i = 0; i < 3; i++) {
        if (!ac[i]) break;
        if (tbl_g_i(oci, ac[i]->d[0], &kv) == TBL_STAT(NF)) return GEN_STAT(INV);
        oci = kv->d[2].p;
        fn = kv->d[1].p;
    }
    te *e = te_i(7, g->ta, gen_code_entry_f);
    e->d[0] = U6(op_id);
    e->d[1] = P(ac1);
    e->d[2] = P(ac2);
    e->d[3] = P(ac3);
    e->d[4] = P(fn);
    lst_ab(g->code, P(e));
    return GEN_STAT(OK);
}

uint16_t gen_var_g_c(const te *v) {
    return u4_g_o(v->d[0], 1);
}

void gen_var_s_c(te *v, uint16_t c) {
    v->d[0] = u4_s_o(v->d[0], 1, c);
}

uint16_t gen_var_g_t(const te *v) {
    return u4_g_o(v->d[0], 0);
}

void gen_var_s_t(te *v, uint16_t t) {
    v->d[0] = u4_s_o(v->d[0], 0, t);
}

te *gen_var_i(gen *g, frfn fr, gen_cls cls, uint16_t type, un id) {
    te *v = te_i(2, g->ta, fr);
    gen_var_s_c(v, cls);
    gen_var_s_t(v, type);
    v->d[1] = id;
    return v;
}

gen_stat gen_n(gen *g, void *st, as *a, te **e) {
    gen_stat stat;
    te *h = g->code->h;
    while (h) {
        te *c = h->d[0].p;
        gen_fn *fn = c->d[4].p;
        if ((stat = fn(g, st, c, a, e)) != GEN_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return GEN_STAT(OK);
}

void gen_f(gen *g) {
    if (!g || --g->r > 0) return;
    tbl_f(g->oci);
    lst_f(g->code);
    g->af->f(g);
}
