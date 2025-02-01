
#include "../gen_x64.h"

static gen_stat add_auauau_fn(alfn *al, frfn *fr, gen *g, void *s, te *ci, as *a)  {
    (void) g;
    gen_st *st = (gen_st*) s;
    te *kv[3];
    for (size_t i = 1; i < 4; i++) {
        if (!ci->d[i].p || tbl_g_i(st->atm, ovt_hsh(ci->d[i].p), &kv[i - 1]) == TBL_STAT(NF)) return GEN_STAT(INV);
    }
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return GEN_STAT(INV); // second reg cannot be dest
    if (kv[0]->d[2].u3 == kv[1]->d[2].u3) {
        // add in place op
        AS2(a, AS_X64(ADD), as_arg(al, fr, ARG_ID(R), U3(kv[1]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[2]->d[2].u3)), ci);
    } else {
        AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[1]->d[2].u3)), ci);
        AS2(a, AS_X64(ADD), as_arg(al, fr, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[2]->d[2].u3)), ci);
    }
    for (size_t i = 0; i < 3; i++) drop_atm_kv(st, kv[i], ci);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_arith(gen *g) {
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), &add_auauau_fn);
}
