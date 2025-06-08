
#include "../gen_x64.h"

static gen_stat set_du_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) {
        if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), idx, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat set_dx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) {
        if (gen_as(a, AS_X64(PXOR), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rmbdr(a, AS_X64(MOVSD), R(BP), idx, XMM(0), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat arg_id_bd_g(te *iv, arg_id *di) {
    switch (gen_var_g_t(iv)) {
        case X64_TYPE(U3):
            *di = ARG_ID(B);
            break;
        case X64_TYPE(U5):
            *di = ARG_ID(DW);
            break;
        default:
            return GEN_STAT(INV);
    }
    return GEN_STAT(OK);
}

static gen_stat idx_to(gen *g, void *s, te *restrict ci, as *a, err **e, as_inst ai, vr *i, te *restrict from) {
    gen_stat stat;
    arg_id di;
    te *kvib, *kvii;
    gen_cls x = GEN_CLS(N), y = GEN_CLS(N), z = GEN_CLS(N);
    if (i->l >= 2) {
        x = gen_var_g_c(i->d[0].p);
        y = gen_var_g_c(i->d[1].p);
    }
    if (i->l == 2) {
        if ((x == GEN_CLS(A) || x == GEN_CLS(T)) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D)) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), from, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
        } else return gen_err(g, ci, e, "nyi");
    } else if (i->l == 3) {
        z = gen_var_g_c(i->d[2].p);
        if ((x == GEN_CLS(A) || x == GEN_CLS(T)) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D) && (z == GEN_CLS(A) || z == GEN_CLS(T))) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if ((stat = get_reg(s, i->d[2].p, &kvii) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, ARG_ID(RS), as_x64_rs(kvii->d[2].u3, 8)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), from, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
            drop_atm_kv(s, kvii, ci);
        } else return gen_err(g, ci, e, "nyi");
    } else return gen_err(g, ci, e, "nyi");
    return GEN_STAT(OK);
}

static gen_stat set_iu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[2].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), kv->d[2]))) != GEN_STAT(OK)) return stat;
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat set_iv_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    int32_t idx;
    te *tgt = ci->d[2].p;
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), U3(R(AX))))) != GEN_STAT(OK)) return stat;
    return GEN_STAT(OK);
}

static gen_stat set_ad_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    uint64_t qw = ((te*) ci->d[2].p)->d[1].u6;
    if (!qw) {
        if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

void gen_set(gen *g) {
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), set_du_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), set_dx_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), set_iu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), set_iv_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), set_ad_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), set_ad_fn);
}
