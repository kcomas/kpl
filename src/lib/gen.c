
#include "gen.h"

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, size_t type, size_t id) {
    te *v = te_i(3, ga, gf);
    v->d[0] = U3(cls);
    v->d[1] = U6(type);
    v->d[2] = U6(id);
    return v;
}

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, cls_tbl_i *cti, tbl *oc, lst *code) {
    gen *g = ga(sizeof(gen));
    g->r = 1;
    g->ga = ga;
    g->gf = gf;
    g->ocef = ocef;
    g->cti = cti;
    g->oc = oc;
    g->code = code;
    return g;
}

void gen_f(gen *g) {
    if (!g || --g->r > 0) return;
    tbl_f(g->oc);
    lst_f(g->code);
    g->gf(g);
}
