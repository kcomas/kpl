
#include "../gen_x64.h"

static gen_stat add_auauau_fn(alfn *al, frfn *fr, gen *g, void *s, te *ci, as *a)  {
    (void) g;
    gen_stat stat;
    gen_st *st = (gen_st*) s;
    te *kv[3];
    if ((stat = get_reg_n(st, ci, kv, 3)) != GEN_STAT(OK)) return stat;
    if (kv[0]->d[2].u3 == kv[2]->d[2].u3) return GEN_STAT(INV); // second reg cannot be dest
    if (kv[0]->d[2].u3 == kv[1]->d[2].u3) {
        // add in place op
        AS2(a, AS_X64(ADD), as_arg(al, fr, ARG_ID(R), U3(kv[1]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[2]->d[2].u3)), ci);
    } else {
        AS2(a, AS_X64(MOV), as_arg(al, fr, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[1]->d[2].u3)), ci);
        AS2(a, AS_X64(ADD), as_arg(al, fr, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg(al, fr, ARG_ID(R), U3(kv[2]->d[2].u3)), ci);
    }
    drop_atm_kv_n(st, kv, ci, 3);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

void gen_arith(gen *g) {
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), &add_auauau_fn);
}
