
#include "idx.h"

static gen_stat ref_am_amm_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(RM), U3(kv[1]->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat ref_amm_im_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    vr *i = ((te*) ci->d[2].p)->d[1].p;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if ((stat = idx_from(g, s, ci, a, e, AS_X64(LEA), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat ref_amm_vm_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    int32_t v1;
    te *kv;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (st_stkv_idx(s, gen_var_g_t(ci->d[2].p), ((te*) ci->d[2].p)->d[1].u3, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rrmbd(a, AS_X64(LEA), kv->d[2].u3, R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat ref_aad_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(RM), U3(kv[1]->d[2].u3)), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat ref_aamd_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(RM), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(RM), U3(R(AX))), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat ref_aid_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    vr *i = ((te*) ci->d[2].p)->d[1].p;
    if ((stat = idx_from(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(RM), kv->d[2]), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat ref_avd_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    int32_t v1;
    te *kv;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (st_stkv_idx(s, gen_var_g_t(ci->d[2].p), ((te*) ci->d[2].p)->d[1].u3, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(RM), U3(R(AX))), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat ref_amad_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(RM), kv[1]->d[2]), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat ref_amamd_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(RM), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(RM), kv[0]->d[2]), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat dref_aim_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    vr *i = ((te*) ci->d[2].p)->d[1].p;
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if ((stat = idx_from(g, s, ci, a, e, AS_X64(MOV), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat;
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(RM), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat dref_aam_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(RM), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

void gen_ref(gen *g) {
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(A), X64_TYPE(MM), ref_am_amm_fn);
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(T), X64_TYPE(MM), ref_am_amm_fn);
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(I), X64_TYPE(M), ref_amm_im_fn);
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(I), X64_TYPE(MM), ref_amm_im_fn); // for storing weak refs
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(I), X64_TYPE(MM), ref_amm_im_fn);
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(V), X64_TYPE(M), ref_amm_vm_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(I), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_aid_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(T), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_aad_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(D), X64_TYPE(U3), ref_aamd_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(I), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_aid_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(V), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_avd_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(V), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_avd_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(T), X64_TYPE(M), GEN_CLS(D), X64_TYPE(U3), ref_amad_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(A), X64_TYPE(MM), GEN_CLS(D), X64_TYPE(U3), ref_amamd_fn);
    GEN_OP_A3(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(T), X64_TYPE(MM), GEN_CLS(D), X64_TYPE(U3), ref_amamd_fn);
    GEN_OP_A2(g, GEN_OP(DREF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(I), X64_TYPE(MM), dref_aim_fn);
    GEN_OP_A2(g, GEN_OP(DREF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(A), X64_TYPE(MM), dref_aam_fn);
    GEN_OP_A2(g, GEN_OP(DREF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(T), X64_TYPE(MM), dref_aam_fn);
}
