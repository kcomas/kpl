
#include "../gen_x64.h"

static gen_stat call_auml_fn(alfn *al, frfn fr, gen *g, void *s, te *ci, as *a) {
    (void) g;
    gen_stat stat;
    gen_st *st = s;
    te *kvr;
    if ((stat = get_reg(st, ci->d[1].p, &kvr)) != GEN_STAT(OK)) return stat;
    te *h = st->atm->i->h;
    while (h) {
        te *r = h->d[0].p;
        if (r->d[2].u3 != kvr->d[2].u3) AS1(a, AS_X64(PUSH), as_arg(al, fr, ARG_ID(R), r->d[2]), ci);
        h = h->d[2].p;
    }
    static reg ir[] = {R(DI), R(SI), R(DX), R(CX), R(8), R(9)};
    size_t iri = 0, xri = 0;
    vr *args = ((te*) ci->d[2].p)->d[2].p;
    for (size_t i = 0; i < args->l; i++) {
        if (iri > 5 || xri > 5) return GEN_STAT(INV);
        te *ovt = args->d[i].p, *kv;
        switch (ovt->d[0].u3) {
            case GEN_CLS(A):
            case GEN_CLS(T):
                if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat;
                switch (ovt->d[1].u3) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        // TODO
                        break;
                    default:
                        AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(ir[iri++])), as_arg(al, fr, ARG_ID(R), kv->d[2]), ci);
                        break;
                }
                // TODO
                break;
            case GEN_CLS(V):
                // TODO move rsp into reg
                break;
            case GEN_CLS(D):
                // TODO load data into reg
                break;
            default:
                return GEN_STAT(INV);
        }
    }
    AS1(a, AS_X64(CALL), as_arg(al, fr, ARG_ID(L), ((te*) ci->d[3].p)->d[2]), ci);
    te *t = st->atm->i->t;
    while (t) {
        te *r = t->d[0].p;
        if (r->d[2].u3 != kvr->d[2].u3) AS1(a, AS_X64(POP), as_arg(al, fr, ARG_ID(R), r->d[2]), ci);
        t = t->d[1].p;
    }
    AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), kvr->d[2]), as_arg(al, fr, ARG_ID(R), U3(R(AX))), ci); // TODO xmm
    drop_atm_kv(st, kvr, ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), U3(X64_TYPE(U6)), GEN_CLS(M), U3(X64_TYPE(N)), GEN_CLS(L), U3(X64_TYPE(N)), &call_auml_fn);
}
