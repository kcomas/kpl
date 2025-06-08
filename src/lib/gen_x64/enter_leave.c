
#include "../gen_x64.h"

static gen_stat enter_fn(alfn *al, frfn *fr, gen *g, void *s, te *ci, as *a) {
    (void) g;
    AS1(a, AS_X64(PUSH), as_arg(al, fr, ARG_ID(R), U3(R(BP))), ci);
    AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(R(BP))), as_arg(al, fr, ARG_ID(R), U3(R(SP))), ci);
    gen_st *st = (gen_st*) s;
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return GEN_STAT(INV);
        AS2(a, AS_X64(SUB), as_arg(al, fr, ARG_ID(R), U3(R(SP))), as_arg(al, fr, ARG_ID(B), U3(stks)), ci);
    }
    if (st->rac >= 3) AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(R(10))), as_arg(al, fr, ARG_ID(R), U3(R(DX))), ci);
    if (st->xac >= 4) AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(R(11))), as_arg(al, fr, ARG_ID(R), U3(R(CX))), ci);
    // TODO swap xmm
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_e(alfn *al, frfn *fr, gen_st *st, te *ci, as *a)  {
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return GEN_STAT(INV);
        AS2(a, AS_X64(ADD), as_arg(al, fr, ARG_ID(R), U3(R(SP))), as_arg(al, fr, ARG_ID(B), U3(stks)), ci);
    }
    AS1(a, AS_X64(POP), as_arg(al, fr, ARG_ID(R), U3(R(BP))), ci);
    AS0(a, AS_X64(RET), ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_au_fn(alfn *al, frfn *fr, gen *g, void *s, te *ci, as *a)  {
    (void) g;
    gen_stat stat;
    gen_st *st = (gen_st*) s;
    te *ovt = ci->d[1].p, *kv;
    if ((stat = get_reg(st, ovt, &kv)) != GEN_STAT(OK)) return stat;
    AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(R(AX))), as_arg(al, fr, ARG_ID(R), U3(kv->d[2].u3)), ci);
    drop_atm_kv(st, kv, ci);
    return leave_e(al, fr, st, ci, a);
}

static gen_stat leave_du_fn(alfn *al, frfn *fr, gen *g, void *s, te *ci, as *a)  {
    (void) g;
    gen_st *st = (gen_st*) s;
    AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(R(AX))), as_arg(al, fr, ARG_ID(QW), ((te*) ci->d[1].p)->d[2]), ci);
    return leave_e(al, fr, st, ci, a);
}

void gen_enter_leave(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), &enter_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), U3(X64_TYPE(U6)), &leave_au_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(D), U3(X64_TYPE(U6)), &leave_du_fn);
}
