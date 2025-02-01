
#include "../gen_x64.h"

static gen_stat enter_fn(alfn *al, frfn *af, gen *g, void *s, te *ci, as *a) {
    (void) g;
    as_a(a, AS_X64(PUSH), as_arg(al, af, ARG_ID(R), U3(R(BP))), NULL, NULL, NULL);
    set_code_s(ci, a);
    as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(BP))), as_arg(al, af, ARG_ID(R), U3(R(SP))), NULL, NULL);
    gen_st *st = (gen_st*) s;
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return GEN_STAT(INV);
        as_a(a, AS_X64(SUB), as_arg(al, af, ARG_ID(R), U3(R(SP))), as_arg(al, af, ARG_ID(B), U3(stks)), NULL, NULL);
    }
    if (st->rac >= 3) as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(10))), as_arg(al, af, ARG_ID(R), U3(R(DX))), NULL, NULL);
    if (st->xac >= 4) as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(11))), as_arg(al, af, ARG_ID(R), U3(R(CX))), NULL, NULL);
    // TODO swap xmm
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

static gen_stat leave_au_fn(alfn *al, frfn *af, gen *g, void *s, te *ci, as *a)  {
    (void) g;
    gen_st *st = (gen_st*) s;
    te *ovt = ci->d[1].p, *kv;
    if (!ovt) return GEN_STAT(INV);
    if (tbl_g_i(st->atm, ovt_hsh(ovt), &kv) == TBL_STAT(NF)) return GEN_STAT(INV);
    as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(AX))), as_arg(al, af, ARG_ID(R), U3(kv->d[2].u3)), NULL, NULL);
    set_code_s(ci, a);
    drop_atm_kv(st, kv, ci);
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return GEN_STAT(INV);
        as_a(a, AS_X64(ADD), as_arg(al, af, ARG_ID(R), U3(R(SP))), as_arg(al, af, ARG_ID(B), U3(stks)), NULL, NULL);
        set_code_s(ci, a);
    }
    as_a(a, AS_X64(POP), as_arg(al, af, ARG_ID(R), U3(R(BP))), NULL, NULL, NULL);
    set_code_s(ci, a);
    as_a(a, AS_X64(RET), NULL, NULL, NULL, NULL);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}
void gen_enter_leave(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), &enter_fn);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), U3(X64_TYPE(U6)), &leave_au_fn);
}
