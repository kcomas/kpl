
#include "../gen_x64.h"

static gen_stat ref_m_mm_fn(gen *g, void *s, te *ci, as *a, err **e) {
    gen_stat stat;
    te *kv[2];
    if ((stat = get_reg_n(s, ci, kv, 2)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(kv[0]->d[2].u3)), as_arg_i(a, ARG_ID(RM), U3(kv[1]->d[2].u3)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(s, kv, ci, 2);
    return GEN_STAT(OK);
}

void gen_ref(gen *g) {
    GEN_OP_A2(g, GEN_OP(REF), GEN_CLS(T), X64_TYPE(M), GEN_CLS(A), X64_TYPE(MM), ref_m_mm_fn);
}
