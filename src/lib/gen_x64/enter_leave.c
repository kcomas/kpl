
#include "../gen_x64.h"

static gen_stat enter_fn(gen *g, void *s, te *ci, as *a, err **e) {
    (void) g;
    (void) e;
    gen_st *st = s;
    AS1(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(BP))), ci);
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(BP))), as_arg_i(a, ARG_ID(R), U3(R(SP))), ci);
    uint16_t sub = st->rvc * sizeof(void*);
    sub += st->xvc * sizeof(void*) * 2;
    if (sub) {
        if (sub <= INT8_MAX) AS2(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sub)), ci);
        else HERE("TODO SUB DW");
    }
    if (st->rac >= 3) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(10))), as_arg_i(a, ARG_ID(R), U3(R(DX))), ci);
    if (st->rac >= 4) AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(11))), as_arg_i(a, ARG_ID(R), U3(R(CX))), ci);
    if (st->xac >= 1) AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(7))), as_arg_i(a, ARG_ID(X), U3(XMM(0))), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_e(gen *g, gen_st *st, te *ci, as *a, err **e)  {
    (void) g;
    (void) e;
    uint16_t add = st->rvc * sizeof(void*);
    add += st->xvc * sizeof(void*) * 2;
    if (add) {
        if (add <= INT8_MAX) AS2(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(add)), ci);
        else HERE("TODO ADD DW");
    }
    AS1(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(R(BP))), ci);
    AS0(a, AS_X64(RET), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_au_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), ci);
    drop_atm_kv(s, kv, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_ax_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), U3(kv->d[2].u3)), ci);
    drop_atm_kv(s, kv, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_du_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_dx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    AS2(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_vu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
     gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_vx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), idx, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    return leave_e(g, s, ci, a, e);
}

static gen_stat set_du_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) AS2(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), ci);
    else AS2(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), ci);
    gen_as_rmbdr(a, AS_X64(MOV), R(BP), idx, R(AX), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat set_dx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (!((te*) ci->d[2].p)->d[1].u6) AS2(a, AS_X64(PXOR), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), U3(XMM(0))), ci);
    else AS2(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[2].p)->d[1]), ci);
    gen_as_rmbdr(a, AS_X64(MOVSD), R(BP), idx, XMM(0), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_enter_leave(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), enter_fn);
    GEN_OP_A0(g, GEN_OP(LEAVE), leave_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(F6), leave_ax_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(F6), leave_ax_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(U6), leave_du_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(F6), leave_dx_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(V), X64_TYPE(I6), leave_vu_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(V), X64_TYPE(F6), leave_vx_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(I6), GEN_CLS(D), X64_TYPE(I6), set_du_fn);
    GEN_OP_A2(g, GEN_OP(SET), GEN_CLS(V), X64_TYPE(F6), GEN_CLS(D), X64_TYPE(F6), set_dx_fn);
}
