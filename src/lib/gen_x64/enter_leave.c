
#include "../gen_x64.h"

static gen_stat enter_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_st *st = s;
    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(BP))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(BP))), as_arg_i(a, ARG_ID(R), U3(R(SP))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    uint32_t sub = st->xvc + st->rvc;
    if (sub) {
        if (sub & 1) sub++;
        if (gen_as(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), bd_arg(a, sub * sizeof(void*)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    if (st->rac >= 3 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(10))), as_arg_i(a, ARG_ID(R), U3(R(DX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (st->rac >= 4 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(11))), as_arg_i(a, ARG_ID(R), U3(R(CX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (st->xac >= 1 && gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(7))), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat leave_e(gen *g, gen_st *st, te *ci, as *a, err **e)  {
    (void) g;
    (void) e;
    uint32_t add = st->xvc + st->rvc;
    if (add) {
        if (add & 1) add++;
        if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), bd_arg(a, add * sizeof(void*)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    if (gen_as(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(R(BP))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(RET), NULL, NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat leave_au_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    gen_stat stat;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(kv->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_ax_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(s, ovt, &kv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(X), U3(kv->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv(s, kv, ci);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_du_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    if (!((te*) ci->d[1].p)->d[1].u6) {
        if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci)) return gen_err(g, ci, e, __FUNCTION__);
    } else if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_dx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    if (gen_as(a, AS_X64(MOVQ), as_arg_i(a, ARG_ID(X), U3(XMM(0))), as_arg_i(a, ARG_ID(QW), ((te*) ci->d[1].p)->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_vu_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_vx_fn(gen *g, void *s, te *ci, as *a, err **e) {
    int32_t idx;
    te *tgt = ci->d[1].p;
    if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
    if (gen_as_rrmbd(a, AS_X64(MOVSD), XMM(0), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    return leave_e(g, s, ci, a, e);
}

static gen_stat leave_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    return leave_e(g, s, ci, a, e);
}

void gen_enter_leave(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), enter_fn);
    GEN_OP_A0(g, GEN_OP(LEAVE), leave_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(U6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(I6), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(M), leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), X64_TYPE(F6), leave_ax_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(T), X64_TYPE(F6), leave_ax_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(U6), leave_du_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(F6), leave_dx_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), X64_TYPE(M), leave_du_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(V), X64_TYPE(I6), leave_vu_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(V), X64_TYPE(M), leave_vu_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(V), X64_TYPE(F6), leave_vx_fn);
}
