
#include "gen.h"

const char* gen_cls_str(gen_cls cls) {
    switch (cls) {
        case GEN_CLS(N): return "N";
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

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, un info, size_t id) {
    te *v = te_i(3, ga, gf);
    v->d[0] = U3(cls);
    v->d[1] = info;
    v->d[2] = U6(id);
    return v;
}

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, frfn *cef, cls_tbl_i *cti, tbl *oci, lst *code) {
    gen *g = ga(sizeof(gen));
    g->r = 1;
    g->ga = ga;
    g->gf = gf;
    g->ocef = ocef;
    g->cef = cef;
    g->cti = cti;
    g->oci = oci;
    g->code = code;
    return g;
}

static un gen_op_hsh(gen_cls cls, un info) {
    return U6((info.u3 << 8) + cls);
}

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, un info1, gen_cls cls2, un info2, gen_cls cls3, un info3, gen_fn *fn) {
    tbl *oci = g->oci;
    te *kv;
    if (tbl_g_i(oci, U6(op_id), &kv) == TBL_STAT(NF)) {
        kv = te_i(3, g->ga, g->ocef);
        kv->d[0] = U6(op_id);
        kv->d[2] = P(g->cti());
        tbl_a(oci, kv);
        oci = kv->d[2].p;
    } else oci = kv->d[2].p;
    const gen_cls cls[] = {cls1, cls2, cls3};
    const un info[] = {info1, info2, info3};
    for (size_t i = 0; i < 3; i++) {
        if (cls[i] == GEN_CLS(N)) break;
        un hsh = gen_op_hsh(cls[i], info[i]);
        if (tbl_g_i(oci, hsh, &kv) == TBL_STAT(NF)) {
            kv = te_i(6, g->ga, g->ocef);
            kv->d[0] = hsh;
            kv->d[1] = U6(cls[i]);
            kv->d[2] = info[i];
            kv->d[4] = P(g->cti());
            tbl_a(oci, kv);
            oci = kv->d[4].p;
        } else oci = kv->d[4].p;
    }
    if (kv->l == 3) kv->d[1] = P(fn);
    else kv->d[3] = P(fn);
    return GEN_STAT(OK);
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
        un hsh = gen_op_hsh(ac[i]->d[0].u3, ac[i]->d[1]);
        if (tbl_g_i(oci, hsh, &kv) == TBL_STAT(NF)) return GEN_STAT(INV);
        oci = kv->d[4].p;
        fn = kv->d[3].p;
    }
    te *e = te_i(7, g->ga, g->cef);
    e->d[0] = U6(op_id);
    e->d[1] = P(ac1);
    e->d[2] = P(ac2);
    e->d[3] = P(ac3);
    e->d[4] = P(fn);
    lst_ab(g->code, P(e));
    return GEN_STAT(OK);
}

void gen_f(gen *g) {
    if (!g || --g->r > 0) return;
    tbl_f(g->oci);
    lst_f(g->code);
    g->gf(g);
}
