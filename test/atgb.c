
#include "atg_t.h"

T(fndiv) {
    atg_t_i();
    te *ee = chk_rt_err_t(&al_te), *eu = TH(UN, 2, "e", te_c(ee), "v", TS(I6)), *kv;
    AI(TPGM(fndiv), 3, 2);
    gen *gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(40)))), gen_data(gc, X64_TYPE(M), P(err_f)), NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    gen_type_aff(gc, eu, &e, "");
    D(eu, gc);
    gen_f(gc);
    D(ee, te_f);
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(E), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(32)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_idx_m(gc, X64_TYPE(M), 2, gen_arg(gc, X64_TYPE(M), 0), gen_data(gc, X64_TYPE(U3), U3(48)))), gen_data(gc, X64_TYPE(M), P(mc_f)), NULL));
    gen_type_aff(gc, ee->d[2].p, &e, "");
    D(ee->d[2].p, gc);
    gen_f(gc);
    atg_d_p(t);
    te *fn = TFN(FN, te_c(eu), 2, "x", TS(I6), 0, "y", TS(I6), 1);
    te *cn = RN(LN(LT(1, "f", FLG(-1, LTE_FLG(F)), te_c(fn)), L(3,
        ON(te_c(fn), DFN, EN("f", FLG(-1, LTE_FLG(F)), te_c(fn)), SN(_G, U5(-1))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), ON(TS(I6), UNER, NULL,
            AN(te_c(eu), EN("f", FLG(-1, LTE_FLG(F)), te_c(fn)), L(2, SN(I6, I6(4)), SN(I6, I6(2)))))),
        ON(TS(VD), DUMP, SN(U5, U5(1)), ON(TS(I6), UNER, NULL,
            AN(te_c(eu), EN("f", FLG(-1, LTE_FLG(F)), te_c(fn)), L(2, SN(I6, I6(1)), SN(I6, I6(0))))))
    )));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, (uint32_t) -1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(NE), gen_arg(gc, X64_TYPE(I6), 1), gen_data(gc, X64_TYPE(I6), I6(0)), gen_lbl(gc, 0)));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 0), gen_call_m(gc, 2, gen_char(gc, "x/0"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 1, 0, 0, NULL, 2, 10, atg_div_inv_str));
    A(tbl_g_i(t->dt, P(eu), &kv) == TBL_STAT(OK), "cannot get un des");
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 3), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 3), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(M), 1), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 0), NULL, NULL));
    S(gen_a(gc, GEN_OP(DIV), gen_tmp(gc, X64_TYPE(I6), 4), gen_arg(gc, X64_TYPE(I6), 0), gen_arg(gc, X64_TYPE(I6), 1)));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 5), gen_call_m(gc, 3, gen_data(gc, X64_TYPE(U6), U6(2)), gen_data(gc, X64_TYPE(M), P(&al_te)), gen_data(gc, X64_TYPE(M), kv->d[1])), gen_data(gc, X64_TYPE(M), P(te_i))));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 5), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), NULL));
    S(gen_a(gc, GEN_OP(SET), gen_idx_m(gc, X64_TYPE(N), 2, gen_tmp(gc, X64_TYPE(M), 5), gen_data(gc, X64_TYPE(U3), U3(40))), gen_tmp(gc, X64_TYPE(I6), 4), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(M), 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 5), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    cn = RN(SN(_G, U5(1)));
    gc = gen_i_gen(bg);
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 1), NULL, NULL));
    S(gen_a(gc, GEN_OP(ENTER), NULL, NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 6), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(I6), I6(4)), gen_data(gc, X64_TYPE(I6), I6(2))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 3)));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 2)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 7), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 7), gen_data(gc, X64_TYPE(U3), U3(0))), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 7), gen_data(gc, X64_TYPE(U3), U3(0))), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 6)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 7), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 2), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 10), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 8, 0, 10, NULL, 3, 3, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 8), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 3), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 11), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 6), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 6)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(I6), 11), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 11)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 12), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(I6), I6(1)), gen_data(gc, X64_TYPE(I6), I6(0))), gen_lbl(gc, (uint32_t) -1)));
    S(gen_a(gc, GEN_OP(EQ), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 12), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(1)), gen_lbl(gc, 5)));
    S(gen_a(gc, GEN_OP(NE), gen_idx_m(gc, X64_TYPE(U6), 2, gen_tmp(gc, X64_TYPE(M), 12), gen_data(gc, X64_TYPE(U3), U3(32))), gen_data(gc, X64_TYPE(U6), U6(0)), gen_lbl(gc, 4)));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(M), 13), gen_idx_m(gc, X64_TYPE(M), 2, gen_tmp(gc, X64_TYPE(M), 12), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(ADD), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 13), gen_data(gc, X64_TYPE(U3), U3(0))), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 13), gen_data(gc, X64_TYPE(U3), U3(0))), gen_data(gc, X64_TYPE(I6), I6(1))));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 12)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 13), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 4), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_tmp(gc, X64_TYPE(M), 16), gen_call_m(gc, 2, gen_char(gc, "v"), gen_data(gc, X64_TYPE(M), P(&al_mc))), gen_data(gc, X64_TYPE(M), P(mc_i_cstr))));
    S(atg_rt_err_u(gc, 14, 0, 16, NULL, 4, 3, atg_un_inv_str));
    S(gen_a(gc, GEN_OP(LEAVE), gen_tmp(gc, X64_TYPE(M), 14), NULL, NULL));
    S(gen_a(gc, GEN_OP(LBL), gen_lbl(gc, 5), NULL, NULL));
    S(gen_a(gc, GEN_OP(SET), gen_tmp(gc, X64_TYPE(I6), 17), gen_idx_m(gc, X64_TYPE(I6), 2, gen_tmp(gc, X64_TYPE(M), 12), gen_data(gc, X64_TYPE(U3), U3(40))), NULL));
    S(gen_a(gc, GEN_OP(CALL), gen_call_m(gc, 1, gen_tmp(gc, X64_TYPE(M), 12)), gen_data(gc, X64_TYPE(M), P(te_f)), NULL));
    S(gen_a(gc, GEN_OP(NOP), gen_tmp(gc, X64_TYPE(I6), 17), NULL, NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 5, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_strs[TYPE(I6)])), gen_data(gc, X64_TYPE(U5), U5(0)), gen_data(gc, X64_TYPE(M), P(atg_dump_idnt)), gen_tmp(gc, X64_TYPE(I6), 17)), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(CALLV), gen_call_m(gc, 2, gen_data(gc, X64_TYPE(M), P(stdout)), gen_data(gc, X64_TYPE(M), P(atg_dump_nl))), gen_data(gc, X64_TYPE(M), P(fprintf)), NULL));
    S(gen_a(gc, GEN_OP(LEAVE), gen_data(gc, X64_TYPE(M), P(NULL)), NULL, NULL));
    V(cn, gc);
    te_f(cn);
    gen_f(gc);
    te_f(ee);
    te_f(eu);
    te_f(fn);
    AE(1);
    err_p(e, 0, true);
    A(e->m == atg_div_inv_str, "inv err msg");
    err_f(e);
    atg_t_f();
}
