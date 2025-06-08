
#include "../gen_x64.h"

static gen_stat neg_auau_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    gen_st *st = s;
    te *kv[2];
    if ((stat = get_reg_n(st, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), ci);
    AS1(a, AS_X64(NEG), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), ci);
    drop_atm_kv_n(st, kv, ci, 2);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat add_auauau_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    gen_st *st = s;
    te *kv[3];
    if ((stat = get_reg_n(st, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), ci); /* not in place */
    AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[2]->d[2].u3)), ci);
    drop_atm_kv_n(st, kv, ci, 3);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

#define AUAUBU(N, O) static gen_stat N##_auaubu_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    gen_st *st = s; \
    te *kv[2]; \
    if ((stat = get_reg_n(st, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), ci); \
    AS2(a, AS_X64(O), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(B), ((te*) ci->d[3].p)->d[1]), ci); \
    drop_atm_kv_n(st, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AUAUBU(add, ADD);
AUAUBU(sub, SUB);

#define AXAXDX(N, O) static gen_stat N##_axaxdx_fn(gen *g, void *s, te *ci, as *a, err **e) { \
    gen_stat stat; \
    gen_st *st = s; \
    te *kv[2]; \
    if ((stat = get_reg_n(st, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg"); \
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(X), U3(kv[1]->d[2].u3)), ci); \
    AS2(a, AS_X64(O##SD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[3].p)->d[1]), ci); \
    drop_atm_kv_n(st, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AXAXDX(addsd, ADD);
AXAXDX(subsd, SUB);

static gen_stat addsd_axaxax_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    gen_st *st = s;
    te *kv[3];
    if ((stat = get_reg_n(st, ci, kv, 3)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return gen_err(g, ci, e, "gen inv reg dest"); /* second reg cannot be dest */
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3) AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(X), U3(kv[1]->d[2].u3)), ci); /* not in place */
    AS2(a, AS_X64(ADDSD), as_arg_i(a, ARG_ID(X), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(X), U3(kv[2]->d[2].u3)), ci);
    drop_atm_kv_n(st, kv, ci, 3);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_arith(gen *g) {
    GEN_OP_A2(g, GEN_OP(NEG), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), neg_auau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(T), X64_TYPE(U6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), add_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(A), X64_TYPE(I6), GEN_CLS(T), X64_TYPE(I6), add_auauau_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), addsd_axaxdx_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(T), X64_TYPE(F6), addsd_axaxax_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(A), X64_TYPE(U6), GEN_CLS(D), X64_TYPE(U3), sub_auaubu_fn);
    GEN_OP_A3(g, GEN_OP(SUB), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(A), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), subsd_axaxdx_fn);
}
