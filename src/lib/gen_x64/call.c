
#include "../gen_x64.h"

static void reg_gen_stk(const reg *ir, size_t iri, te *ci, as *a, te *kv, const uint8_t *rsaves, size_t rsl) {
    for (size_t i = 0; i < rsl; i++) {
        if (rsaves[i] == kv->d[2].u3) {
            if (i == rsl - 1) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), ci);
            else AS3(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((rsl - i - 1) * sizeof(void*))), ci);
            return;
        }
    }
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
}

static void reg_xmm_stk(const reg *xr, size_t xri, te *ci, as *a, te *kv, const uint8_t *xsaves, size_t xsl) {
    for (size_t i = 0; i < xsl; i++) {
        if (xsaves[i] == kv->d[2].u3) {
            if (i) AS3(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((xsl - i - 1) * sizeof(void*))), ci);
            else AS2(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), ci);
            return;
        }
    }
    AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(X), kv->d[2]), ci);
}

static gen_stat swap_args(te *restrict ci, as *a, te *restrict r, const vr *args, vr **stk, arg_id ai) {
    un nr;
    for (size_t i = 0; i < args->l; i++) {
        if (r->d[0].u6 == gen_var_hsh(args->d[i].p).u6) { // check if this is an arg being passed in
            if (vr_sf(*stk, &nr) != VR_STAT(OK)) return GEN_STAT(INV);
            if (ai == ARG_ID(R)) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), nr), as_arg_i(a, ai, r->d[2]), ci);
            else AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), nr), as_arg_i(a, ai, r->d[2]), ci);
            vr_ab(stk, r->d[2]);
            r->d[2] = nr;
            break;
        }
    }
    return GEN_STAT(OK);
}

static gen_stat call_arg(gen_st *st, te *ci, as *a, size_t arg_i, const uint8_t *rsaves, size_t rsl, const uint8_t *xsaves, size_t xsl) {
    gen_stat stat;
    static const reg ir[] = {R(DI), R(SI), R(DX), R(CX), R(8), R(9)};
    static const reg xr[] = {XMM(0), XMM(1), XMM(2), XMM(3), XMM(4), XMM(5), XMM(6)};
    size_t iri = 0, xri = 0;
    te *ovt, *kv;
    vr *args = ((te*) ci->d[arg_i].p)->d[1].p;
    if (!rsaves && !xsaves) { // swap regs that are passed for args
        te *h = st->atm->i->h;
        while (h) {
            te *r = h->d[0].p;
            for (size_t i = 0; i < 6; i++) {
                if (r->d[2].u3 == ir[i]) swap_args(ci, a, r, args, &st->rstk, ARG_ID(R));
                else if (r->d[2].u3 == xr[i]) swap_args(ci, a, r, args, &st->xstk, ARG_ID(X));
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
                        if (xsl > 0) reg_xmm_stk(xr, xri, ci, a, kv, xsaves, xsl);
                        else AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(X), kv->d[2]), ci);
                        xri++;
                        break;
                    default:
                        if (rsl > 0) reg_gen_stk(ir, iri, ci, a, kv, rsaves, rsl);
                        else AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
                        iri++;
                        break;
                }
                drop_atm_kv(st, kv, ci);
                break;
            case GEN_CLS(T):
                if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat;
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri++])), as_arg_i(a, ARG_ID(X), kv->d[2]), ci);
                        break;
                    default:
                        AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri++])), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
                        break;
                }
                drop_atm_kv(st, kv, ci);
                break;
            case GEN_CLS(V):
                HERE("TODO move rsp into regs");
                // TODO move rsp into reg
                break;
            case GEN_CLS(D):
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        AS2(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(xr[xri++])), as_arg_i(a, ARG_ID(QW), ovt->d[1]), ci);
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
            AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kvr->d[2]), as_arg_i(a, ARG_ID(X), U3(XMM(0))), ci);
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
    static uint8_t rsaves[11]; \
    static uint8_t xsaves[15]; \
    size_t rsl = 0, xsl = 0; \
    if (RV) { \
        if ((stat = get_reg(st, ci->d[1].p, &kvr)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } \
    if (PR) { \
        te *h = st->atm->i->h; \
        while (h) { \
            te *r = h->d[0].p; \
            if (RV) { \
                if (r->d[2].u3 != kvr->d[2].u3) { \
                    if (r->d[2].u3 < XMM(0)) rsaves[rsl++] = r->d[2].u3; \
                    else xsaves[xsl++] = r->d[2].u3; \
                } \
            } else { \
                if (r->d[2].u3 < XMM(0)) rsaves[rsl++] = r->d[2].u3; \
                else xsaves[xsl++] = r->d[2].u3; \
            } \
            h = h->d[2].p; \
        } \
        for (size_t i = 0; i < rsl; i++) AS1(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(rsaves[i])), ci); \
        if (xsl > 0) { \
            AS2(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * xsl)), ci); \
            for (size_t i = 0; i < xsl; i++) { \
                if (i) AS3(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * i)), as_arg_i(a, ARG_ID(X), U3(xsaves[i])), ci); \
                else AS2(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(X), U3(xsaves[i])), ci); \
            } \
        } \
    } \
    if (PR) { \
        if ((stat = call_arg(st, ci, a, RV ? 2 : 1, rsaves, rsl, xsaves, xsl)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
    } else { \
        if ((stat = call_arg(st, ci, a, RV ? 2 : 1, NULL, 0, NULL, 0)) != GEN_STAT(OK)) return gen_err(stat, ci, e); \
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
        if (xsl > 0) { \
            size_t xa = xsl; \
            while (xsl > 0) { \
                if (xsl == 1) AS2(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(xsaves[xsl - 1])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), ci); \
                else AS3(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(xsaves[xsl - 1])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * (xsl - 1))), ci); \
                xsl--; \
            } \
            AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * xa)), ci); \
        } \
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

static gen_stat call_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    return call_pr_true_rv_true(s, ci, a, e);
}

static gen_stat callnr_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    return call_pr_true_rv_false(s, ci, a, e);
}

static gen_stat callnpr_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    return call_pr_false_rv_true(s, ci, a, e);
}
static gen_stat callvnr_md(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    // TODO NEW FUNCTION TO PUSH XMM ON TO STACK
    AS2(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci);
    return call_pr_true_rv_false(s, ci, a, e);
}

static gen_stat callvnprnr_md(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    // TODO NEW FUNCTION TO PUSH XMM ON TO STACK
    AS2(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci);
    return call_pr_false_rv_false(s, ci, a, e);
}
void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A2(g, GEN_OP(CALL), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callnr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_fn);
    GEN_OP_A2(g, GEN_OP(CALLV), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnr_md);
    GEN_OP_A2(g, GEN_OP(CALLVNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnprnr_md);
}
