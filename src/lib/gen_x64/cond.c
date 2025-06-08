
#include "../gen_x64.h"

#define AAL(N, U, J) static gen_stat N##_a##U##a##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv_n(s, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AAL(eq, u, JE);
AAL(ne, u, JNE);
AAL(gt, u, JA);
AAL(lt, s, JL);
AAL(gt, s, JG);

// using data segment for cmp is slower
//AS2(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), ci);

#define ADL(N, U, J) static gen_stat N##_a##U##d##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *ovt = ci->d[1].p, *kv; \
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    uint64_t qw = ((te*) ci->d[2].p)->d[1].u6; \
    if (qw <= UINT32_MAX) { \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv->d[2]), bd_arg(a, qw), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } else { \
        if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), U6(qw)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

ADL(eq, u, JE);
ADL(ne, u, JNE);
ADL(gt, u, JA);
ADL(lt, s, JL);
ADL(lte, u, JBE);
ADL(gt, s, JG);

#define VDL(N, U, J) static gen_stat N##_v##U##d##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0; \
    if (st_stkv_idx(s, gen_var_g_t(ci->d[1].p), ((te*) ci->d[1].p)->d[1].u3, &v0) != GEN_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    uint64_t qw = ((te*) ci->d[2].p)->d[1].u6; \
    if (qw <= UINT32_MAX) { \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(RM), U3(R(BP))), bd_arg(a, v0), bd_arg(a, qw), NULL, ci)) return gen_err(g, ci, e, __FUNCTION__); \
    } else return gen_err(g, ci, e, "nyi"); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

VDL(gt, s, JG);
VDL(gte, s, JGE);
VDL(lt, s, JL);
VDL(lte, s, JLE);

#define AXDXL(N, C, J) static gen_stat N##_axdxl_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *ovt = ci->d[1].p, *kv; \
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(C), as_arg_i(a, ARG_ID(X), kv->d[2]), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AXDXL(eq, UCOMISD, JE);
AXDXL(ne, UCOMISD, JNE);
AXDXL(gt, COMISD, JA);
AXDXL(lt, COMISD, JB);

void gen_cond(gen *g) {
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), eq_auaul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), eq_audul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), eq_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), ne_auaul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), ne_audul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), ne_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), gt_auaul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_asasl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), gt_audul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), gt_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gte_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_asasl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), lt_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lte_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), lte_audul_fn);
}
