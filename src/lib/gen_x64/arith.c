
#include "../gen_x64.h"

static gen_stat neg_auau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL,  ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(NEG), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

static gen_stat add_auauau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[3];
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); /* not in place */
    if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[2]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 3);
    return GEN_STAT(OK);
}

static gen_stat stk_va(gen_st *st, te *restrict c, int32_t *restrict v) {
    if (gen_var_g_c(c) == GEN_CLS(V)) return st_stkv_idx(st, gen_var_g_t(c), c->d[1].u3, v);
    return st_stka_idx(gen_var_g_t(c), c->d[1].u3, v);
}

static gen_stat stk_g_idx2(gen_st *st, te *restrict c0, te *restrict c1, int32_t *restrict v0, int32_t *restrict v1) {
    if (stk_va(st, c0, v0) != GEN_STAT(OK) || stk_va(st, c1, v1) != GEN_STAT(OK)) return GEN_STAT(INV);
    return GEN_STAT(OK);
}

static gen_stat stk_g_idx3(gen_st *st, te *restrict c0, te *restrict c1, te *restrict c2, int32_t *restrict v0, int32_t *restrict v1, int32_t *restrict v2) {
    if (stk_va(st, c0, v0) != GEN_STAT(OK)) return GEN_STAT(INV);
    return stk_g_idx2(st, c1, c2, v1, v2);
}

static gen_stat add_vuvuau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[3].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
    int32_t v0, v1;
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (v0 == v1) {
        if (x64_type_is_ref(gen_var_g_t(ci->d[1].p))) return gen_err(g, ci, e, "nyi");
        if (gen_as_rmbdr(a, AS_X64(ADD), R(BP), v0, kv->d[2].u3, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else {
        if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), v0, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat add_susuau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv;
    if ((stat = get_reg(s, ci->d[3].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
    int32_t v0, v1;
    if (!x64_type_is_ref(gen_var_g_t(ci->d[1].p)) && !x64_type_is_ref(gen_var_g_t(ci->d[2].p))) return gen_err(g, ci, e, "gen stk args not refs");
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (v0 == v1) {
       if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v0, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
       if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(RM), U3(R(AX))), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else return gen_err(g, ci, e, "nyi");
    drop_atm_kv(s, kv, ci);
    return GEN_STAT(OK);
}

static gen_stat vuvuvu_fn(gen *g, void *s, te *ci, as *a, err **e, as_inst o) {
    int32_t v0, v1, v2;
    if (stk_g_idx3(s, ci->d[1].p, ci->d[2].p, ci->d[3].p, &v0, &v1, &v2) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (v0 == v2) return gen_err(g, ci, e, "gen stk src dest inv");
    if (v0 == v1) {
        if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v2, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        if (gen_as_rmbdr(a, o, R(BP), v0, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    } else {
        if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        if (gen_as_rrmbd(a, o, R(AX), R(BP), v2, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        if (gen_as_rmbdr(a, AS_X64(MOV), R(BP), v0, R(AX), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    return GEN_STAT(OK);
}

static gen_stat sub_vuvuvu_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return vuvuvu_fn(g, s, ci, a, e, AS_X64(SUB));
}

static gen_stat add_vuvuvu_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return vuvuvu_fn(g, s, ci, a, e, AS_X64(ADD));
}

static gen_stat vuvudu_fn(gen *g, void *s, te *ci, as *a, err **e, as_inst o, as_inst b) {
    int32_t v0, v1;
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    uint64_t d = ((te*) ci->d[3].p)->d[1].u6;
    if (d == 1) {
        if (v0 == v1) {
            if (x64_type_is_ref(gen_var_g_t(ci->d[1].p))) {
                if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v0, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                if (gen_as(a, o, as_arg_i(a, ARG_ID(RM), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else if (gen_as(a, o, as_arg_i(a, ARG_ID(RM), U3(R(BP))), bd_arg(a, v0), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else return gen_err(g, ci, e, "nyi");
    } else if (d <= INT32_MAX) {
        if (v0 == v1) {
            if (x64_type_is_ref(gen_var_g_t(ci->d[1].p))) {
                if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v0, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                if (gen_as(a, b, as_arg_i(a, ARG_ID(RM), U3(R(AX))), bd_arg(a, d), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else if (gen_as(a, b, as_arg_i(a, ARG_ID(RM), U3(R(BP))), bd_arg(a, v0), bd_arg(a, d), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else return gen_err(g, ci, e, "nyi");
    } else return gen_err(g, ci, e, "nyi");
    return GEN_STAT(OK);
}

static gen_stat add_susudu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return vuvudu_fn(g, s, ci, a, e, AS_X64(INC), AS_X64(ADD));
}

static gen_stat add_vuvudu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return vuvudu_fn(g, s, ci, a, e, AS_X64(INC), AS_X64(ADD));
}

static gen_stat sub_vuvudu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return vuvudu_fn(g, s, ci, a, e, AS_X64(DEC), AS_X64(SUB));
}

#define MULDIVAUAUAU(N, O) static gen_stat N##_auauau_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[3]; \
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(O), as_arg_i(a, ARG_ID(R), kv[2]->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv_n(s, kv, ci, 3); \
    return GEN_STAT(OK); \
}

MULDIVAUAUAU(mul, MUL);
MULDIVAUAUAU(imul, IMUL);
MULDIVAUAUAU(div, DIV);
MULDIVAUAUAU(idiv, IDIV);

#define MULDIVVUVUAU(N, O) static gen_stat N##_vuvuau_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv; \
    if ((stat = get_reg(s, ci->d[3].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg"); \
    int32_t v0, v1; \
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx"); \
    gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci); \
    if (gen_as(a, AS_X64(O), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    gen_as_rmbdr(a, AS_X64(MOV), R(BP), v0, R(AX), ci); \
    drop_atm_kv(s, kv, ci); \
    return GEN_STAT(OK); \
}

MULDIVVUVUAU(imul, IMUL);

#define MULDIVVUVUVU(N, O) static gen_stat N##_vuvuvu_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0, v1, v2; \
    if (stk_g_idx3(s, ci->d[1].p, ci->d[2].p, ci->d[3].p, &v0, &v1, &v2) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx"); \
    gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci); \
    if (gen_as(a, AS_X64(O), as_arg_i(a, ARG_ID(RM), U3(R(BP))), bd_arg(a, v2), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    gen_as_rmbdr(a, AS_X64(MOV), R(BP), v0, R(AX), ci); \
    return GEN_STAT(OK); \
}

MULDIVVUVUVU(imul, IMUL);

#define UUDU(N, A, M, O, AI) static gen_stat N##_##A##u##A##udu_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    uint64_t v = ((te*) ci->d[3].p)->d[1].u6; \
    if (v == 1) { \
        if (gen_as(a, AS_X64(M), as_arg_i(a, ARG_ID(AI), kv[0]->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } else if (v <= INT32_MAX) { \
        if (gen_as(a, AS_X64(O), as_arg_i(a, ARG_ID(AI), kv[0]->d[2]), bd_arg(a, v), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } else return gen_err(g, ci, e, "nyi"); \
    drop_atm_kv_n(s, kv, ci, 2); \
    return GEN_STAT(OK); \
}

UUDU(add, a, INC, ADD, R);
UUDU(add, am, INC, ADD, RM);
UUDU(sub, a, DEC, SUB, R);

#define AXAXDX(N, O) static gen_stat N##_axaxdx_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[3].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv_n(s, kv, ci, 2); \
    return GEN_STAT(OK); \
}

AXAXDX(addsd, ADD);
AXAXDX(subsd, SUB);

#define AXAXAX(N, O) static gen_stat N##_axaxax_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[3]; \
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */ \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); /* not in place */ \
    if (gen_as(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[2]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv_n(s, kv, ci, 3); \
    return GEN_STAT(OK); \
}

AXAXAX(addsd, ADD);
AXAXAX(subsd, SUB);
AXAXAX(mulsd, MUL);
AXAXAX(divsd, DIV);

#define VXVXAX(N, O) static gen_stat N##_vxvxax_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv; \
    if ((stat = get_reg(s, ci->d[3].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg"); \
    int32_t v0, v1; \
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx"); \
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as_rmbdr(a, AS_X64(MOVSD), R(BP), v0, XMM(0), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    return GEN_STAT(OK); \
}

VXVXAX(mulsd, MUL);

#define VXVXDX(N, O) static gen_stat N##_vxvxdx_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0, v1; \
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx"); \
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[3].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as_rmbdr(a, AS_X64(MOVSD), R(BP), v0, XMM(0), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    return GEN_STAT(OK); \
}

VXVXDX(subsd, SUB);
VXVXDX(mulsd, MUL);

static gen_stat cvtsi2sd_axau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(CVTSI2SD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

void gen_arith(gen *g) {
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), add_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), add_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), add_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), add_vuvuau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), add_vuvuau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), add_vuvudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), add_vuvuvu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), addsd_axaxdx_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), addsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), addsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), X64_TYPE(MU6), GEN_CLS(A), X64_TYPE(MU6), GEN_CLS(D), X64_TYPE(U6), add_amuamudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(S), X64_TYPE(MI6), GEN_CLS(S), X64_TYPE(MI6), GEN_CLS(D), X64_TYPE(I6), add_susudu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(S), X64_TYPE(MI6), GEN_CLS(S), X64_TYPE(MI6), GEN_CLS(A), X64_TYPE(I6), add_susuau_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), subsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), sub_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), sub_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), sub_auaudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), sub_vuvuvu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), sub_vuvudu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), subsd_axaxdx_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), subsd_axaxdx_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), subsd_vxvxdx_fn);
    GEN_OP_A2(g, GEN_OP(NEG), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), neg_auau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), mul_auauau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), mul_auauau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), imul_auauau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), imul_vuvuau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), imul_vuvuvu_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), mulsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), mulsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), mulsd_vxvxax_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), mulsd_vxvxdx_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), div_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), div_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), idiv_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), divsd_axaxax_fn);
    GEN_OP_A2(g, GEN_OP(CST), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(U6), cvtsi2sd_axau_fn);
}
