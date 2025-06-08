
#include "../gen_x64.h"

static gen_stat neg_auau_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), ci);
    AS1(a, AS_X64(NEG), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), ci);
    drop_atm_kv_n(s, kv, ci, 2);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat add_auauau_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    te *kv[3];
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), ci); /* not in place */
    AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[2]->d[2]), ci);
    drop_atm_kv_n(s, kv, ci, 3);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat add_vuvuau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *c0 = ci->d[1].p, *c1 = ci->d[2].p, *kv;
    if ((stat = get_reg(s, ci->d[3].p, &kv) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
    int32_t v0, v1;
    if (st_stkv_idx(s, gen_var_g_t(c0), c0->d[1].u3, &v0) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (st_stkv_idx(s, gen_var_g_t(c1), c1->d[1].u3, &v1) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (v0 == v1) gen_as_rmbdr(a, AS_X64(ADD), R(BP), v0, kv->d[2].u3, ci);
    else {
        gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), v1, ci);
        AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), kv->d[2]), ci);
        gen_as_rmbdr(a, AS_X64(ADD), R(BP), v0, R(AX), ci);
    }
    drop_atm_kv(s, kv, ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

#define MULDIVAUAUAU(N, O) static gen_stat N##_auauau_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[3]; \
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), ci); \
    AS1(a, AS_X64(O), as_arg_i(a, ARG_ID(R), kv[2]->d[2]), ci); \
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci); \
    drop_atm_kv_n(s, kv, ci, 3); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

MULDIVAUAUAU(mul, MUL);
MULDIVAUAUAU(imul, IMUL);
MULDIVAUAUAU(div, DIV);
MULDIVAUAUAU(idiv, IDIV);

#define AUAUBU(N, O) static gen_stat N##_auaubu_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), ci); \
    AS2(a, AS_X64(O), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), ci); \
    drop_atm_kv_n(s, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AUAUBU(add, ADD);
AUAUBU(sub, SUB);

#define AXAXDX(N, O) static gen_stat N##_axaxdx_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    te *kv[2]; \
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[1]->d[2]), ci); \
    AS2(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[3].p)->d[1]), ci); \
    drop_atm_kv_n(s, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AXAXDX(addsd, ADD);
AXAXDX(subsd, SUB);

#define AXAXAX(N, O) static gen_stat N##_axaxax_fn(gen *g, void *s, te *ci, as *a, err **e)  { \
    gen_stat stat; \
    te *kv[3]; \
    if ((stat = get_reg_n(s, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */ \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[1]->d[2]), ci); /* not in place */ \
    AS2(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), kv[0]->d[2]), as_arg_i(a, ARG_ID(X), kv[2]->d[2]), ci); \
    drop_atm_kv_n(s, kv, ci, 3); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AXAXAX(addsd, ADD);
AXAXAX(subsd, SUB);
AXAXAX(mulsd, MUL);
AXAXAX(divsd, DIV);

static gen_stat cvtsi2sd_axau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    AS2(a, AS_X64(CVTSI2SD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), ci);
    drop_atm_kv_n(s, kv, ci, 2);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_arith(gen *g) {
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), add_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), add_vuvuau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), addsd_axaxdx_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), addsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), subsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), subsd_axaxdx_fn);
    GEN_OP_A2(g, GEN_OP(NEG), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), neg_auau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), mul_auauau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), imul_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), div_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), div_auauau_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), idiv_auauau_fn);
    GEN_OP_A3(g, GEN_OP(MUL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), mulsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(DIV), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), divsd_axaxax_fn);
    GEN_OP_A2(g, GEN_OP(CST), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(U6), cvtsi2sd_axau_fn);
}
