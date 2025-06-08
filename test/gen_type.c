
#include "../src/lib/gen_type.h"
#include "gen_t.h"

T(vr_mc) {
    te *type = type_v_i(&al_te, NULL, TYPE(VR), type_s_i(&al_te, NULL, TYPE(SG)));
    gen *g = NULL;
    void *fn = NULL;
    err *e = NULL;
    gen_stat stat = gen_type_des(bg, type, &g, &fn,  &e);
    if (e) err_p(e);
    A(stat == GEN_STAT(OK), "vr_mv_des");
    A(!g, "gen should be null");
    A(fn == mc_f, "inv des fn");
    te_f(type);
}

T(gen_te_mc) {
    gen *g = gen_i_gen(bg);
    S(gen_a(g, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_idx_m(g, X64_TYPE(M), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U5), U5(sizeof(void*) * 4)))), gen_data(g, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(g, GEN_OP(CALL), gen_call_m(g, 1, gen_idx_m(g, X64_TYPE(M), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U5), U5(sizeof(void*) * 6)))), gen_data(g, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 1), gen_idx_m(g, X64_TYPE(M), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U5), U5(sizeof(void*) * 2))), NULL));
    S(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 1), gen_idx_m(g, X64_TYPE(M), 2, gen_tmp(g, X64_TYPE(M), 1), gen_data(g, X64_TYPE(U5), U5(sizeof(void*) * 1))), NULL));
    S(gen_a(g, GEN_OP(CALLNPR), gen_tmp(g, X64_TYPE(M), 1), NULL, NULL));
    S(gen_a(g, GEN_OP(LEAVE), NULL, NULL, NULL));
    BUILD(g, m);
    te *t = te_i(3, &al_te, ((void(*)(void*)) m));
    t->d[0] = P(mc_i_cstr("a", &al_mc));
    t->d[1] = I6(1234);
    t->d[2] = P(mc_i_cstr("b", &al_mc));
    te_f(t);
}

T(te_mc) {
    te *type = type_te_i_v(&al_te, NULL, 3, type_s_i(&al_te, NULL, TYPE(SG)), type_s_i(&al_te, NULL, TYPE(I6)), type_s_i(&al_te, NULL, TYPE(SG)));
    gen *g = NULL;
    void *fn = NULL;
    err *e = NULL;
    gen_stat stat = gen_type_des(bg, type, &g, &fn, &e);
    if (e) err_p(e);
    A(stat == GEN_STAT(OK), "te_mc_des");
    A(!fn, "fn should be null");
    te_f(type);
    gen_p(g, NULL);
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(sizeof(void*) * 4)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(sizeof(void*) * 6)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 0), gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(sizeof(void*) * 2))), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 0), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U5(sizeof(void*) * 1))), NULL));
    S(gen_a(gc, GEN_OP(CALLNPR), gen_tmp(gc, X64_TYPE(M), 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), NULL, NULL, NULL));
    A(gen_code_eq(g, gc), "gen neq");
    gen_f(gc);
    BUILD(g, m);
    te *t = te_i(3, &al_te, ((void(*)(void*)) m));
    t->d[0] = P(mc_i_cstr("a", &al_mc));
    t->d[1] = I6(1234);
    t->d[2] = P(mc_i_cstr("b", &al_mc));
    te_f(t);
}
