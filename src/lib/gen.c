
#include "gen.h"

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, un info, size_t id) {
    te *v = te_i(3, ga, gf);
    v->d[0] = U3(cls);
    v->d[1] = info;
    v->d[2] = U6(id);
    return v;
}

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, frfn *cef, cls_tbl_i *cti, tbl *oct, lst *code) {
    gen *g = ga(sizeof(gen));
    g->r = 1;
    g->ga = ga;
    g->gf = gf;
    g->ocef = ocef;
    g->cef = cef;
    g->cti = cti;
    g->oct = oct;
    g->code = code;
    return g;
}

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, un info1, gen_cls cls2, un info2, gen_cls cls3, un info3, gen_fn *fn) {
    tbl *oct = g->oct;
    te *kv;
    if (tbl_g_i(oct, U6(op_id), &kv) == TBL_STAT(NF)) {
        kv = te_i(3, g->ga, g->ocef);
        kv->d[0] = U6(op_id);
        kv->d[2] = P(g->cti());
        tbl_a(oct, kv);
        oct = kv->d[2].p;
    } else oct = kv->d[2].p;
    const gen_cls cls[] = {cls1, cls2, cls3};
    const un info[] = {info1, info2, info3};
    for (size_t i = 0; i < 3; i++) {
        if (cls[i] == GEN_CLS(N)) break;
        un hsh = U6((info[i].u6 << 5) + cls[i]);
        if (tbl_g_i(oct, hsh, &kv) == TBL_STAT(NF)) {
            kv = te_i(6, g->ga, g->ocef);
            kv->d[0] = hsh;
            kv->d[1] = U6(cls[i]);
            kv->d[2] = info[i];
            kv->d[4] = P(g->cti());
            tbl_a(oct, kv);
            oct = kv->d[4].p;
        } else oct = kv->d[4].p;
    }
    if (kv->l == 3) kv->d[1] = P(fn);
    else kv->d[3] = P(fn);
    return GEN_STAT(OK);
}

gen_stat gen_a(gen *g, size_t op_id, te *restrict ac1, te *restrict ac2, te *restrict ac3) {
    tbl *oct = g->oct;
    return GEN_STAT(OK);
}

void gen_f(gen *g) {
    if (!g || --g->r > 0) return;
    tbl_f(g->oct);
    lst_f(g->code);
    g->gf(g);
}
