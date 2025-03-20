
#include "../gen_x64.h"

static void reg_gen_stk(const reg *ir, size_t *iri, te *ci, as *a, te *kv, const uint8_t *rsaves, size_t rsl) {
    for (size_t i = 0; i < rsl; i++) {
        if (rsaves[i] == kv->d[2].u3) {
            if (i == rsl - 1) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[*iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), ci);
            else AS3(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[*iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((rsl - i - 1) * sizeof(void*))), ci);
            return;
        }
    }
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[*iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
}

static gen_stat call_arg(gen_st *st, te *ci, as *a, size_t arg_i, const uint8_t *rsaves, size_t rsl) {
    gen_stat stat;
    static const reg ir[] = {R(DI), R(SI), R(DX), R(CX), R(8), R(9)};
    size_t iri = 0, xri = 0;
    te *ovt, *kv;
    vr *args = ((te*) ci->d[arg_i].p)->d[1].p;
    if (!rsaves) { // swap regs that are passed for args
        te *h = st->atm->i->h;
        un nr;
        while (h) {
            te *r = h->d[0].p;
            for (size_t i = 0; i < 6; i++) {
                if (r->d[2].u3 == ir[i]) {
                    for (size_t i = 0; i < args->l; i++) {
                        if (r->d[1].p == args->d[i].p) { // check if this is an arg being passed in
                            if (vr_sf(st->rstk, &nr) != VR_STAT(OK)) return GEN_STAT(INV);
                            AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), nr), as_arg_i(a, ARG_ID(R), r->d[2]), ci);
                            vr_ab(&st->rstk, r->d[2]);
                            r->d[2] = nr;
                            break;
                        }
                    }
                    break;
                }
            }
            h = h->d[2].p;
        }
    }
    for (size_t i = 0; i < args->l; i++) {
        if (iri > 5 || xri > 5) return GEN_STAT(INV);
        ovt = args->d[i].p;
        switch (gen_var_g_c(ovt)) {
            case GEN_CLS(A):
                if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat;
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        // TODO xmm
                        break;
                    default:
                        if (rsl > 0) reg_gen_stk(ir, &iri, ci, a, kv, rsaves, rsl);
                        else AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
                        iri++;
                        drop_atm_kv(st, kv, ci);
                        break;
                }
                break;
            case GEN_CLS(T):
                if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat;
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        // TODO xmm
                        break;
                    default:
                        AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri++])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
                        drop_atm_kv(st, kv, ci);
                        break;
                }
                break;
            case GEN_CLS(V):
                // TODO move rsp into reg
                break;
            case GEN_CLS(D):
                // TODO load data into reg
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        // TODO xmm
                        break;
                    case X64_TYPE(M):
                    case X64_TYPE(U6):
                    case X64_TYPE(I6):
                        AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri++])), as_arg_i(a, ARG_ID(QW), ovt->d[1]), ci);
                        break;
                    default:
                        return GEN_STAT(INV);
                }
                break;
            default:
                return GEN_STAT(INV);
        }
    }
    return GEN_STAT(OK);
}

static gen_stat call_ret(te *restrict ci, as *a, te *restrict kvr) {
    switch (gen_var_g_t(ci->d[1].p)) {
        case X64_TYPE(F5):
        case X64_TYPE(F6):
            // TODO
            break;
        default:
            AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kvr->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci);
            break;
    }
    return GEN_STAT(OK);
}

// preserve regs, return value
#define CALL(PR, RV) static gen_stat call_pr_##PR##_rv_##RV(gen_st *st, te *ci, as *a, te **e) { \
    gen_stat stat; \
    te *kvr; \
    uint8_t rsaves[28]; \
    size_t rsl = 0; \
    if (RV) { \
        if ((stat = get_reg(st, ci->d[1].p, &kvr)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } \
    if (PR) { \
        te *h = st->atm->i->h; \
        while (h) { \
            te *r = h->d[0].p; \
            if (RV) { \
                if (r->d[2].u3 != kvr->d[2].u3) rsaves[rsl++] = r->d[2].u3; \
            } else { \
                rsaves[rsl++] = r->d[2].u3; \
            } \
            h = h->d[2].p; \
        } \
        for (size_t i = 0; i < rsl; i++) AS1(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(rsaves[i])), ci); \
    } \
    if (PR) { \
        if ((stat = call_arg(st, ci, a, RV ? 2 : 1, rsaves, rsl)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } else { \
        if ((stat = call_arg(st, ci, a, RV ? 2 : 1, NULL, 0)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } \
    te *fn = RV ? ci->d[3].p : ci->d[2].p; \
    uint8_t fr; \
    switch (gen_var_g_c(fn)) { \
        case GEN_CLS(L): \
            AS1(a, AS_X64(CALL), as_arg_i(a, ARG_ID(L), fn->d[1]), ci); \
            break; \
        case GEN_CLS(D): \
            if ((stat = rstk_b(st, &fr)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
            AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(fr)), as_arg_i(a, ARG_ID(QW), fn->d[1]), ci); \
            AS1(a, AS_X64(CALL), as_arg_i(a, ARG_ID(R), U3(fr)), ci); \
            break; \
        default: \
           return gen_err(stat, ci, e); \
    } \
    if (RV) { \
        if ((stat = call_ret(ci, a, kvr)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } \
    if (PR) { \
        while (rsl > 0) { \
            AS1(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(rsaves[rsl - 1])), ci); \
            rsl--; \
        } \
    } \
    if (RV) drop_atm_kv(st, kvr, ci); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

CALL(true, true);
CALL(false, true);
CALL(true, false);
CALL(false, false);

static gen_stat call_auml_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    return call_pr_true_rv_true(s, ci, a, e);
}

static gen_stat callnpr_auml_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    return call_pr_false_rv_true(s, ci, a, e);
}

static void set_vec_reg(te *ci, as *a) {
    size_t nx = 0;
    vr *v = ((te*) ci->d[1].p)->d[1].p;
    for (size_t i = 0; i < v->l; i++) {
       x64_type t = gen_var_g_t(v->d[i].p);
       if (t == X64_TYPE(F5) || t == X64_TYPE(F6)) nx++;
    }
    if (!nx) AS2(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci);
    else AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), U6(nx)), ci);
}

static gen_stat callvnr_md(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    set_vec_reg(ci, a);
    return call_pr_true_rv_false(s, ci, a, e);
}

static gen_stat callvnprnr_md(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    set_vec_reg(ci, a);
    return call_pr_false_rv_false(s, ci, a, e);
}

void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_auml_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_auml_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_auml_fn);
    GEN_OP_A2(g, GEN_OP(CALLV), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnr_md);
    GEN_OP_A2(g, GEN_OP(CALLVNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnprnr_md);
}
