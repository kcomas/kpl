
#include "idx.h"

static gen_stat set_vv_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t v0, v1;
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), v0, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat set_vu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) {
        if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), idx, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat set_vx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) {
        if (gen_as(a, AS_X64(PXOR), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rmbdr(a, AS_X64(MOVSD), R(BP), idx, XMM(0), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat set_vs_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idxv, idxs;
    te *tgt = ci->d[1].p;
    te *from = ci->d[2].p;
    if (st_stka_idx(gen_var_g_t(from), from->d[1].u3, &idxs) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stka inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idxs, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(RM), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idxv) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), idxv, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat set_iu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[2].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat set_ivu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    int32_t idx;
    te *tgt = ci->d[2].p;
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), U3(R(AX))), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    return GEN_STAT(OK);
}

static gen_stat set_ivx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    int32_t idx;
    te *tgt = ci->d[2].p;
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOVSD), i, as_arg_i(a, ARG_ID(X), U3(XMM(0))), XMM(0), ARG_ID(X))) != GEN_STAT(OK)) return stat;
    return GEN_STAT(OK);
}

static gen_stat set_id_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    vr *i = ((te*) ci->d[1].p)->d[1].p;
    arg_id aid = ((te*) ci->d[2].p)->d[1].u6 <= UINT32_MAX ? ARG_ID(DW) : ARG_ID(QW);
    if ((stat = idx_to(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, aid, ((te*) ci->d[2].p)->d[1]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    return GEN_STAT(OK);
}

static gen_stat set_ui_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    vr *i = ((te*) ci->d[2].p)->d[1].p;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if ((stat = idx_from(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    drop_atm_kv(s, kv, ci);
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
static gen_stat set_va_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *tgt = ci->d[1].p, *kv;
    int32_t idx;
    if ((stat = get_reg(s, ci->d[2].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), idx, kv->d[2].u3, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat set_ax_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), U3(kv[1]->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat set_xd_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kv->d[2]), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat set_au_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat set_av_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *tgt = ci->d[2].p, *kv;
    int32_t idx;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), kv->d[2].u3, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat set_avx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *tgt = ci->d[2].p, *kv;
    int32_t idx;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

void gen_set(gen *g) {
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), set_vv_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), set_vu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), set_vu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(M), GEN_CLS(D), X64_TYPE(M), set_vu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), set_vx_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(S), X64_TYPE(MI6), set_vs_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(A), X64_TYPE(I6), set_iu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(T), X64_TYPE(U6), set_iu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(T), X64_TYPE(I6), set_iu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(T), X64_TYPE(M), set_iu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(V), X64_TYPE(I6), set_ivu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(V), X64_TYPE(M), set_ivu_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(V), X64_TYPE(F6), set_ivx_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(D), X64_TYPE(U5), set_id_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(I), X64_TYPE(I6), set_ui_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(I), X64_TYPE(U6), set_ui_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(M), GEN_CLS(I), X64_TYPE(M), set_ui_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(I), X64_TYPE(N), GEN_CLS(D), X64_TYPE(I6), set_id_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), set_ad_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), set_ad_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), set_xd_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), set_va_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(M), GEN_CLS(T), X64_TYPE(M), set_va_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(M), GEN_CLS(A), X64_TYPE(F6), set_ax_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(M), GEN_CLS(A), X64_TYPE(M), set_au_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(M), GEN_CLS(V), X64_TYPE(M), set_av_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(T), X64_TYPE(M), GEN_CLS(V), X64_TYPE(F6), set_avx_fn);
}
