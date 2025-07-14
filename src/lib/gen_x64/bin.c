
#include "../gen_x64.h"

static gen_stat and_auauau_fn(gen *g, void *s, te *ci, as *a, err **e) {
    te *kv[3];
    if (get_reg_n(s, ci, kv, 3) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (kv[0]->d[2].u3 != kv[1]->d[2].u3 && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[1]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (gen_as(a, AS_X64(AND), as_arg_i(a, ARG_ID(R), kv[0]->d[2]), as_arg_i(a, ARG_ID(R), kv[2]->d[2]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 3);
    return GEN_STAT(OK);
}

void gen_bin(gen *g) {
    GEN_OP_A3(g, GEN_OP(AND), GEN_CLS(T), X64_TYPE(U3), GEN_CLS(T), X64_TYPE(U3), GEN_CLS(T), X64_TYPE(U3), and_auauau_fn);
}
