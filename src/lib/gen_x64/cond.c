
#include "../gen_x64.h"

#define AUAUL(N, J) static gen_stat N##_auaul_fn(gen *g, void *s, te *ci, as *a)  { \
    (void) g; \
    gen_stat stat; \
    gen_st *st = s; \
    te *kv[2]; \
    if ((stat = get_reg_n(st, ci, kv, 2)) != GEN_STAT(OK)) return stat; \
    AS2(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(kv[1]->d[2].u3)), ci); \
    AS1(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[2]), ci); \
    drop_atm_kv_n(st, kv, ci, 2); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AUAUL(eq, JE);
AUAUL(ne, JNE);
AUAUL(gt, JA);

#define AUDUL(N, J) static gen_stat N##_audul_fn(gen *g, void *s, te *ci, as *a)  { \
    (void) g; \
    gen_stat stat; \
    gen_st *st = s; \
    te *ovt = ci->d[1].p, *kv; \
    if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat; \
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[2]), ci); \
    AS2(a, AS_X64(CMP), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci); \
    AS1(a, AS_X64(J), as_arg_i(a, ARG_ID(L), ((te*) ci->d[3].p)->d[2]), ci); \
    drop_atm_kv(st, kv, ci); \
    set_code_e(ci, a); \
    return GEN_STAT(OK); \
}

AUDUL(eq, JE);
AUDUL(ne, JNE);
AUDUL(gt, JA);

void gen_cond(gen *g) {
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), eq_auaul_fn);
    GEN_OP_A3(g, GEN_OP(EQ), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(D), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), eq_audul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), ne_auaul_fn);
    GEN_OP_A3(g, GEN_OP(NE), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(D), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), ne_audul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), gt_auaul_fn);
    GEN_OP_A3(g, GEN_OP(GT), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(D), U3(X64_TYPE(U6)), GEN_CLS(L), U3(X64_TYPE(N)), gt_audul_fn);
}
