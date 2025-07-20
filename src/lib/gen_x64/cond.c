
#include "idx.h"

#define AAL(N, U, J) static gen_stat N##_a##U##a##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv_n(s, kv, ci, 2); \
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
    int64_t qw = ((te*) ci->d[2].p)->d[1].u6; \
    if (qw >= INT32_MIN && qw <= INT32_MAX) { \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv->d[2]), bd_arg(a, qw), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } else { \
        if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), U6(qw)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), kv->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    } \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    return GEN_STAT(OK); \
}

ADL(eq, u, JE);
ADL(ne, u, JNE);
ADL(gt, u, JA);
ADL(gt, s, JG);
ADL(gte, s, JGE);
ADL(lt, s, JL);
ADL(lte, u, JBE);
ADL(lte, s, JLE);

#define DUL(N, U, J) static gen_stat N##_d##U##a##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    te *kv; \
    if (get_reg(s, ci->d[2].p, &kv) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), as_arg_i(a, ARG_ID(R), kv->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    return GEN_STAT(OK); \
}

DUL(eq, u, JE);
DUL(ne, u, JNE);

#define VDL(N, U, J) static gen_stat N##_v##U##d##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0; \
    if (st_stkv_idx(s, gen_var_g_t(ci->d[1].p), ((te*) ci->d[1].p)->d[1].u3, &v0) != GEN_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    int64_t qw = ((te*) ci->d[2].p)->d[1].u6; \
    if (qw >= INT32_MIN && qw <= INT32_MAX) { \
        if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(RM), U3(R(BP))), bd_arg(a, v0), bd_arg(a, qw), NULL, ci)) return gen_err(g, ci, e, __FUNCTION__); \
    } else return gen_err(g, ci, e, "nyi"); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    return GEN_STAT(OK); \
}

VDL(eq, s, JE);
VDL(ne, s, JNE);
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
    return GEN_STAT(OK); \
}

AXDXL(eq, UCOMISD, JE);
AXDXL(ne, UCOMISD, JNE);
AXDXL(gt, COMISD, JA);
AXDXL(lt, COMISD, JB);

#define VXDXL(N, C, J) static gen_stat N##_vxdxl_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0; \
    if (st_stkv_idx(s, gen_var_g_t(ci->d[1].p), ((te*) ci->d[1].p)->d[1].u3, &v0) != GEN_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), v0, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(C), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    return GEN_STAT(OK); \
}

VXDXL(gte, COMISD, JAE);
VXDXL(lt, COMISD, JB);

#define IDL(N, U, J) static gen_stat N##_i##U##d##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    vr *i = ((te*) ci->d[1].p)->d[1].p; \
    int64_t qw = ((te*) ci->d[2].p)->d[1].u6; \
    if (qw >= INT32_MIN && qw <= INT32_MAX) { \
        if ((stat = idx_to(g, s, ci, a, e, AS_X64(CMP), i, bd_arg(a, qw), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat; \
    } else return gen_err(g, ci, e, "nyi"); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    return GEN_STAT(OK); \
}

IDL(eq, u, JE);
IDL(ne, u, JNE);

#define UIL(N, U, J) static gen_stat N##_a##U##i##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv; \
    vr *i = ((te*) ci->d[2].p)->d[1].p; \
    if ((stat = get_reg(s, ci->d[1].p, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if ((stat = idx_from(g, s, ci, a, e, AS_X64(CMP), i, as_arg_i(a, ARG_ID(R), kv->d[2]), R(AX), ARG_ID(R))) != GEN_STAT(OK)) return stat; \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kv, ci); \
    return GEN_STAT(OK); \
}

UIL(lt, u, JB);
UIL(gte, u, JAE);

#define VVL(N, U, J) static gen_stat N##_v##U##v##U##l_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    int32_t v0, v1; \
    if (stk_g_idx2(s, ci->d[1].p, ci->d[2].p, &v0, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx"); \
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v0, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as_rrmbd(a, AS_X64(CMP), R(AX), R(BP), v1, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    return GEN_STAT(OK);  \
}

VVL(gt, s, JG);
VVL(lte, s, JLE);

#define DUU(N, U, S) static gen_stat N##_d##U##a##U##a_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    te *kc, *ks; \
    if (get_reg(s, ci->d[2].p, &kc) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (get_reg(s, ci->d[3].p, &ks) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), ks->d[2]), as_arg_i(a, ARG_ID(R), ks->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(CMP), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), as_arg_i(a, ARG_ID(R), kc->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    if (gen_as(a, AS_X64(S), as_arg_i(a, ARG_ID(R), ks->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__); \
    drop_atm_kv(s, kc, ci); \
    drop_atm_kv(s, ks, ci); \
    return GEN_STAT(OK); \
}

DUU(eq, u, SETE);
DUU(ne, u, SETNE);

void gen_cond(gen *g) {
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), eq_auaul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), eq_audul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(T), X64_TYPE(U3), GEN_CLS(D), X64_TYPE(U3), GEN_CLS(L), X64_TYPE(N), eq_audul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), eq_duaul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), eq_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), eq_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(I), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), eq_iudul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(U3), eq_duaua_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), ne_auaul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), ne_audul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), ne_duaul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), ne_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), ne_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(I), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), ne_iudul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(U3), ne_duaua_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), gt_auaul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_asasl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), gt_audul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), gt_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gt_vsvsl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gte_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gte_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), gte_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), gte_vxdxl_fn);
    GEN_OP_A3(g, GEN_OP(GTE), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(I), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), gte_auiul_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_asasl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), lt_axdxl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lt_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(I), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), lt_auiul_fn);
    GEN_OP_A3(g, GEN_OP(LT), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), GEN_CLS(L), X64_TYPE(N), lt_vxdxl_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lte_vsdsl_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U6), GEN_CLS(L), X64_TYPE(N), lte_audul_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lte_asdsl_fn);
    GEN_OP_A3(g, GEN_OP(LTE), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(L), X64_TYPE(N), lte_vsvsl_fn);
}
