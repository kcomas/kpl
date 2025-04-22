
#include "../gen_x64.h"

static bool reg_gen_stk(const reg *ir, size_t iri, te *ci, as *a, te *kv, const uint8_t *rsaves, size_t rsl) {
    for (size_t i = 0; i < rsl; i++) {
        if (rsaves[i] == kv->d[2].u3) {
            if (i == rsl - 1) return gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), NULL, NULL, ci) == AS_STAT(OK);
            return gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((rsl - i - 1) * sizeof(void*))), NULL, ci) == AS_STAT(OK);
        }
    }
    return gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) == AS_STAT(OK);
}

static bool reg_xmm_stk(const reg *xr, size_t xri, te *ci, as *a, te *kv, const uint8_t *xsaves, size_t xsl, size_t rsl) {
    for (size_t i = 0; i < xsl; i++) {
        if (xsaves[i] == kv->d[2].u3) {
            if (rsl) return gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((i + rsl) * sizeof(void*))), NULL, ci) == AS_STAT(OK);
            if (i) return gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3((i) * sizeof(void*))), NULL, ci) == AS_STAT(OK);
            return gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), NULL, NULL, ci) == AS_STAT(OK);
        }
    }
    return gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(X), kv->d[2]), NULL, NULL, ci) == AS_STAT(OK);
}

static gen_stat swap_args(te *restrict ci, as *a, te *restrict r, const vr *args, vr **stk, arg_id ai) {
    un nr;
    for (size_t i = 0; i < args->l; i++) {
        if (r->d[0].u6 == gen_var_hsh(args->d[i].p).u6) { // check if this is an arg being passed in
            if (vr_sf(*stk, &nr) != VR_STAT(OK)) return GEN_STAT(INV);
            if (ai == ARG_ID(R)) {
                if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), nr), as_arg_i(a, ai, r->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), nr), as_arg_i(a, ai, r->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            vr_ab(stk, r->d[2]);
            r->d[2] = nr;
            break;
        }
    }
    return GEN_STAT(OK);
}

static gen_stat call_arg(gen_st *st, te *ci, as *a, vr *args, const uint8_t *rsaves, size_t rsl, const uint8_t *xsaves, size_t xsl) {
    gen_stat stat;
    int32_t idx;
    static const reg ir[] = {R(DI), R(SI), R(DX), R(CX), R(8), R(9)};
    static const reg xr[] = {XMM(0), XMM(1), XMM(2), XMM(3), XMM(4), XMM(5), XMM(6)};
    size_t iri = 0, xri = 0;
    te *ovt, *kv;
    if (!rsaves && !xsaves) { // swap regs that are passed for args
        te *h = st->atm->i->h;
        while (h) {
            te *r = h->d[0].p;
            for (size_t i = 0; i < 6; i++) {
                if (r->d[2].u3 == ir[i]) {
                    if ((stat = swap_args(ci, a, r, args, &st->rstk, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                } else if (r->d[2].u3 == xr[i] && (stat = swap_args(ci, a, r, args, &st->xstk, ARG_ID(X))) != GEN_STAT(OK)) return stat;
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
                        if (xsl > 0) {
                            if (!reg_xmm_stk(xr, xri, ci, a, kv, xsaves, xsl, rsl)) return GEN_STAT(INV);
                        } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri])), as_arg_i(a, ARG_ID(X), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        xri++;
                        break;
                    default:
                        if (rsl > 0) {
                            if (!reg_gen_stk(ir, iri, ci, a, kv, rsaves, rsl)) return GEN_STAT(INV);
                        } else if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri])), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
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
                        if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri++])), as_arg_i(a, ARG_ID(X), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        break;
                    default:
                        if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri++])), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        break;
                }
                drop_atm_kv(st, kv, ci);
                break;
            case GEN_CLS(V):
                if ((stat = st_stkv_idx(st, gen_var_g_t(ovt), ovt->d[1].u3, &idx)) != GEN_STAT(OK)) return stat;
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        if (gen_as_rrmbd(a, AS_X64(MOVSD), xr[xri++], R(BP), idx, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        break;
                    default:
                        if (gen_as_rrmbd(a, AS_X64(MOV), ir[iri++], R(BP), idx, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        break;
                }
                break;
            case GEN_CLS(D):
                switch (gen_var_g_t(ovt)) {
                    case X64_TYPE(F5):
                    case X64_TYPE(F6):
                        if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xr[xri++])), as_arg_i(a, ARG_ID(QW), ovt->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
                        break;
                    case X64_TYPE(M):
                    case X64_TYPE(U6):
                    case X64_TYPE(I6):
                        if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ir[iri++])), as_arg_i(a, ARG_ID(QW), ovt->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
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
            if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kvr->d[2]), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            break;
        default:
            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kvr->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            break;
    }
    return GEN_STAT(OK);
}

// preserve regs, return value
#define CALL(PR, RV) static gen_stat call_pr_##PR##_rv_##RV(const gen *g, gen_st *st, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kvr; \
    static uint8_t rsaves[11]; \
    static uint8_t xsaves[15]; \
    size_t rsl = 0, xsl = 0; \
    if (RV) { \
        if ((stat = get_reg(st, ci->d[1].p, &kvr)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
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
        if (xsl > 0) { \
            if (gen_as(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * 2 * xsl)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            for (size_t i = 0; i < xsl; i++) { \
                if (i) { \
                    if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * i)), as_arg_i(a, ARG_ID(X), U3(xsaves[i])), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
                } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(X), U3(xsaves[i])), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            } \
        } \
        for (size_t i = 0; i < rsl; i++) if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(rsaves[i])), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } \
    vr *args = ((te*) ci->d[RV ? 2 : 1].p)->d[1].p; \
    if (PR) { \
        if ((stat = call_arg(st, ci, a, args, rsaves, rsl, xsaves, xsl)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen call"); \
    } else { \
        if ((stat = call_arg(st, ci, a, args, NULL, 0, NULL, 0)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen call"); \
    } \
    te *fn = RV ? ci->d[3].p : ci->d[2].p; \
    uint8_t fr; \
    switch (gen_var_g_c(fn)) { \
        case GEN_CLS(L): \
            if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(L), fn->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            break; \
        case GEN_CLS(D): \
            if ((stat = rstk_b(st, &fr)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen no reg for call"); \
            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(fr)), as_arg_i(a, ARG_ID(QW), fn->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(R), U3(fr)), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            break; \
        default: \
           return gen_err(g, ci, e, "gen inv call cls"); \
    } \
    if (RV) { \
        if ((stat = call_ret(ci, a, kvr)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen ret"); \
    } \
    if (PR) { \
        while (rsl > 0) { \
            if (gen_as(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(rsaves[rsl - 1])), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
            rsl--; \
        } \
        if (xsl > 0) { \
            size_t xa = xsl; \
            while (xsl > 0) { \
                if (xsl == 1) { \
                    if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xsaves[xsl - 1])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
                } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xsaves[xsl - 1])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * (xsl - 1))), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
                xsl--; \
            } \
            if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * 2 * xa)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
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

static gen_stat call_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return call_pr_true_rv_true(g, s, ci, a, e);
}

static gen_stat callnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return call_pr_true_rv_false(g, s, ci, a, e);
}

static gen_stat callnpr_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return call_pr_false_rv_true(g, s, ci, a, e);
}

static bool num_vec_args(as *a, te *ci, size_t arg_i) {
    size_t nva = 0;
    vr *args = ((te*) ci->d[arg_i].p)->d[1].p;
    for (size_t i = 0; i < args->l; i++) {
        switch (gen_var_g_t(args->d[i].p)) {
            case X64_TYPE(F5):
            case X64_TYPE(F6):
                nva++;
            default:
                break;
        }
    }
    if (nva) return gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), U6(nva)), NULL, NULL, ci) == AS_STAT(OK);
    return gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) == AS_STAT(OK);
}

static gen_stat callvnr_md(gen *g, void *s, te *ci, as *a, err **e) {
    if (!num_vec_args(a, ci, 1)) return gen_err(g, ci, e, __FUNCTION__);
    return call_pr_true_rv_false(g, s, ci, a, e);
}

static gen_stat callvnprnr_md(gen *g, void *s, te *ci, as *a, err **e) {
    if (!num_vec_args(a, ci, 1)) return gen_err(g, ci, e, __FUNCTION__);
    return call_pr_false_rv_false(g, s, ci, a, e);
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
