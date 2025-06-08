
#include "../gen_x64.h"

static gen_stat enter_fn(gen *g, void *s, te *ci, as *a, te **e) {
    (void) g;
    gen_st *st = s;
    AS1(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(BP))), ci);
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(BP))), as_arg_i(a, ARG_ID(R), U3(R(SP))), ci);
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return gen_err(GEN_STAT(INV), ci, e);
        AS2(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(stks)), ci);
    }
    if (st->rac >= 3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(10))), as_arg_i(a, ARG_ID(R), U3(R(DX))), ci);
    if (st->rac >= 4) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(11))), as_arg_i(a, ARG_ID(R), U3(R(CX))), ci);
    // TODO swap xmm
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_e(gen_st *st, te *ci, as *a, te **e)  {
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return gen_err(GEN_STAT(INV), ci, e);
        AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(stks)), ci);
    }
    AS1(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(R(BP))), ci);
    AS0(a, AS_X64(RET), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_au_fn(gen *g, void *s, te *ci, as *a, te **e)  {
    (void) g;
    gen_stat stat;
    gen_st *st = s;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return gen_err(stat, ci, e);
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), ci);
    drop_atm_kv(st, kv, ci);
    return leave_e(st, ci, a, e);
}

static gen_stat leave_du_fn(gen *g, void *s, te *ci, as *a, te **e)  {
    (void) g;
    gen_st *st = s;
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), ci);
    return leave_e(st, ci, a, e);
}
gen_stat leave_fn(gen *g, void *s, te *ci, as *a, te **e)  {
    (void) g;
    gen_st *st = s;
    return leave_e(st, ci, a, e);
}

void gen_enter_leave(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), enter_fn);
    GEN_OP_A0(g, GEN_OP(LEAVE), leave_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(U6), leave_du_fn);
}
