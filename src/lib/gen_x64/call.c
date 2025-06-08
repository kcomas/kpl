
#include "../gen_x64.h"

static gen_stat call_auml_fn(alfn *al, frfn fr, gen *g, void *s, te *ci, as *a) {
    (void) g;
    gen_stat stat;
    gen_st *st = s;
    te *kvr;
    if ((stat = get_reg(st, ci->d[1].p, &kvr)) != GEN_STAT(OK)) return stat;
    uint8_t rsaves[28];
    size_t rsl = 0;
    te *h = st->atm->i->h;
    while (h) {
        te *r = h->d[0].p;
        if (r->d[2].u3 != kvr->d[2].u3) rsaves[rsl++] = r->d[2].u3;
        h = h->d[2].p;
    }
    for (size_t i = 0; i < rsl; i++) AS1(a, AS_X64(PUSH), as_arg(al, fr, ARG_ID(R), U3(rsaves[i])), ci);
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
                        // TODO xmm
                        break;
                    default:
                        AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(ir[iri++])), as_arg(al, fr, ARG_ID(R), kv->d[2]), ci);
                        drop_atm_kv(st, kv, ci);
                        break;
                }
                break;
            case GEN_CLS(V):
                // TODO move rsp into reg
                break;
            case GEN_CLS(D):
                // TODO load data into reg
                switch (ovt->d[1].u3) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        // TODO xmm
                        break;
                    case X64_TYPE(M):
                    case X64_TYPE(U6):
                    case X64_TYPE(I6):
                        AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(ir[iri++])), as_arg(al, fr, ARG_ID(QW), ovt->d[2]), ci);
                        break;
                    default:
                        return GEN_STAT(INV);
                }
                break;
            default:
                return GEN_STAT(INV);
        }
    }
    AS1(a, AS_X64(CALL), as_arg(al, fr, ARG_ID(L), ((te*) ci->d[3].p)->d[2]), ci);
    while (rsl > 0) {
        AS1(a, AS_X64(POP), as_arg(al, fr, ARG_ID(R), U3(rsaves[rsl - 1])), ci);
        rsl--;
    }
    switch (((te*) ci->d[1].p)->d[1].u3) {
        case X64_TYPE(F5):
        case X64_TYPE(F6):
            // TODO
            break;
        default:
            AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), kvr->d[2]), as_arg(al, fr, ARG_ID(R), U3(R(AX))), ci);
            break;
    }
    drop_atm_kv(st, kvr, ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), U3(X64_TYPE(U6)), GEN_CLS(M), U3(X64_TYPE(N)), GEN_CLS(L), U3(X64_TYPE(N)), &call_auml_fn);
}
