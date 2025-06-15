
#include "../atg_x64.h"

static atg_stat op_tel_ter_t(atg *t, gen *g, te *an, err **e, gen_op oc, te *c0, const char *pf) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = ((te*) an->d[6].p)->d[3].p;
    if (!(l = var_arg(g, l, atg_x64_g_t(l->d[2].p)))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (!(r = var_arg(g, r, atg_x64_g_t(r->d[2].p)))) return atg_err(t, an->d[6].p, e, "atg op r inv");
    if (gen_a(g, oc, c0, l, r) != GEN_STAT(OK)) return atg_err(t, an, e, pf);
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) {
    return op_tel_ter_t(t, g, an, e, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), __FUNCTION__);
}

static atg_stat add_f6_e_f6_e_f6(atg *t, gen *g, te *an, err **e) {
    return op_tel_ter_t(t, g, an, e, GEN_OP(ADD), gen_tmp(g, X64_TYPE(F6), t->tc++), __FUNCTION__);
}

static atg_stat sub_f6_e_f6_e_f6(atg *t, gen *g, te *an, err **e) {
    return op_tel_ter_t(t, g, an, e, GEN_OP(SUB), gen_tmp(g, X64_TYPE(F6), t->tc++), __FUNCTION__);
}

static atg_stat mul_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) {
    return op_tel_ter_t(t, g, an, e, GEN_OP(MUL), gen_tmp(g, X64_TYPE(I6), t->tc++), __FUNCTION__);
}

#define A_E_E(N, O, T) static atg_stat N##_i6_e_i6_e_i6(atg *t, gen *g, te *an, err **e) { \
    te *l = ((te*) an->d[5].p)->d[3].p; \
    if (!(l = var_arg(g, l, X64_TYPE(T)))) return atg_err(t, an->d[5].p, e, "atg op l inv"); \
    return op_tel_ter_t(t, g, an, e, GEN_OP(O), l, __FUNCTION__); \
}

A_E_E(adda, ADD, I6);
A_E_E(mula, MUL, I6);

static atg_stat add_i6_a_i6_a_i6(atg *t, gen *g, te *an, err **e) {
    te *l = atg_g_g(an->d[5].p)->d[1].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), te_c(l), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat op_e_o(atg *t, gen *g, te *an, err **e, gen_op op, x64_type xt, const char *pf) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (!(l = var_arg(g, l, xt))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, op, gen_tmp(g, xt, t->tc++), l, te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, pf);
    return ATG_STAT(OK);
}

static atg_stat a__e__s__(atg *t, gen *g, te *an, err **e, gen_op c, x64_type xt) {
    te *l = ((te*) an->d[5].p)->d[3].p;
    if (!(l = var_arg(g, l, xt))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, c, l, te_c(l), gen_data(g, xt, ((te*) an->d[6].p)->d[4])) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat a__e__o__(atg *t, gen *g, te *an, err **e, gen_op c, x64_type xt) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (!(l = var_arg(g, l, xt))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, c, l, te_c(l), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat adda_i6_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return a__e__s__(t, g, an, e, GEN_OP(ADD), X64_TYPE(I6));
}

static atg_stat adda_i6_e_i6_oa_i6(atg *t, gen *g, te *an, err **e) {
    te *l = ((te*) an->d[5].p)->d[3].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (!(l = var_arg(g, l, X64_TYPE(I6)))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, GEN_OP(ADD), l, te_c(l), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat add_i6_e_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    return op_e_o(t, g, an, e, GEN_OP(ADD), X64_TYPE(I6), __FUNCTION__);
}

#define OES(o, O, u, U) static atg_stat o##_##u##_e_##u##_s_##u(atg *t, gen *g, te *an, err **e) { \
    te *l = ((te*) an->d[5].p)->d[3].p; \
    if (!(l = var_arg(g, l, X64_TYPE(U)))) return atg_err(t, an->d[5].p, e, "atg op l inv"); \
    if (gen_a(g, GEN_OP(O), gen_tmp(g, X64_TYPE(U), t->tc++), l, gen_data(g, X64_TYPE(U), ((te*) an->d[6].p)->d[4])) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__); \
    return ATG_STAT(OK); \
}

OES(add, ADD, i6, I6);
OES(add, ADD, u6, U6);
OES(sub, SUB, u6, U6);
OES(sub, SUB, i6, I6);

static atg_stat suba_i6_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    return a__e__s__(t, g, an, e, GEN_OP(SUB), X64_TYPE(I6));
}

static atg_stat neg_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    uint32_t id = ((te*) atg_g_g(an->d[6].p)->d[1].p)->d[1].u5;
    if (gen_a(g, GEN_OP(NEG), gen_tmp(g, X64_TYPE(I6), id), gen_tmp(g, X64_TYPE(I6), id), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat mul_i6_s_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    te *l = an->d[5].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(MUL), te_c(r), gen_data(g, X64_TYPE(I6), l->d[4]), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat mul_i6_e_i6_s_i6(atg *t, gen *g, te *an, err **e) {
    te *l = ((te*) an->d[5].p)->d[3].p;
    if (!(l = var_arg(g, l, atg_x64_g_t(l->d[2].p)))) return atg_err(t, an->d[5].p, e, "atg op l inv");
    if (gen_a(g, GEN_OP(MUL), gen_tmp(g, X64_TYPE(I6), t->tc++), l, gen_data(g, X64_TYPE(I6), ((te*) an->d[6].p)->d[4])) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat mul_u6_e_u6_a_u6(atg *t, gen *g, te *an, err **e) {
    return op_e_o(t, g, an, e, GEN_OP(MUL), X64_TYPE(U6), __FUNCTION__);
}

static atg_stat mul_f6_e_f6_o_f6(atg *t, gen *g, te *an, err **e) {
    return op_e_o(t, g, an, e, GEN_OP(MUL), X64_TYPE(F6), __FUNCTION__);
}

static atg_stat mul_f6_e_f6_e_f6(atg *t, gen *g, te *an, err **e) {
    return op_tel_ter_t(t, g, an, e, GEN_OP(MUL), gen_tmp(g, X64_TYPE(F6), t->tc++), __FUNCTION__);
}

static atg_stat mula_f6_e_f6_s_f6(atg *t, gen *g, te *an, err **e) {
    return a__e__s__(t, g, an, e, GEN_OP(MUL), X64_TYPE(F6));
}

static atg_stat mula_i6_e_i6_o_i6(atg *t, gen *g, te *an, err **e) {
    return a__e__o__(t, g, an, e, GEN_OP(MUL), X64_TYPE(I6));
}

static atg_stat div_f6_o_f6_o_f6(atg *t, gen *g, te *an, err **e) {
    te *l = atg_g_g(an->d[5].p)->d[1].p, *r = atg_g_g(an->d[6].p)->d[1].p;
    if (gen_a(g, GEN_OP(DIV), gen_tmp(g, X64_TYPE(F6), t->tc++), te_c(l), te_c(r)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

void atg_arith(atg *t) {
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), add_i6_e_i6_e_i6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), add_i6_e_i6_s_i6);
    atg_a_o(t, OC(ADD), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), add_f6_e_f6_e_f6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), add_i6_e_i6_o_i6);
    atg_a_o(t, OC(ADD), TYPE(I6), AST_CLS(A), TYPE(I6), AST_CLS(A), TYPE(I6), add_i6_a_i6_a_i6);
    atg_a_o(t, OC(ADD), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), add_u6_e_u6_s_u6);
    atg_a_o(t, OC(ADDA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), adda_i6_e_i6_s_i6);
    atg_a_o(t, OC(ADDA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), adda_i6_e_i6_e_i6);
    atg_a_o(t, OC(ADDA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), adda_i6_e_i6_oa_i6);
    atg_a_o(t, OC(ADDA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(A), TYPE(I6), adda_i6_e_i6_oa_i6);
    atg_a_o(t, OC(SUB), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), sub_i6_e_i6_s_i6);
    atg_a_o(t, OC(SUB), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(U6), sub_u6_e_u6_s_u6);
    atg_a_o(t, OC(SUB), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), sub_f6_e_f6_e_f6);
    atg_a_o(t, OC(SUB), TYPE(I6), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6), neg_i6_o_i6);
    atg_a_o(t, OC(SUBA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), suba_i6_e_i6_s_i6);
    atg_a_o(t, OC(MUL), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6), mul_f6_e_f6_e_f6);
    atg_a_o(t, OC(MUL), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), mul_i6_e_i6_e_i6);
    atg_a_o(t, OC(MUL), TYPE(U6), AST_CLS(E), TYPE(U6), AST_CLS(A), TYPE(U6), mul_u6_e_u6_a_u6);
    atg_a_o(t, OC(MUL), TYPE(I6), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6), mul_i6_s_i6_o_i6);
    atg_a_o(t, OC(MUL), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6), mul_i6_e_i6_s_i6);
    atg_a_o(t, OC(MUL), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(O), TYPE(F6), mul_f6_e_f6_o_f6);
    atg_a_o(t, OC(MULA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6), mula_i6_e_i6_e_i6);
    atg_a_o(t, OC(MULA), TYPE(F6), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6), mula_f6_e_f6_s_f6);
    atg_a_o(t, OC(MULA), TYPE(I6), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6), mula_i6_e_i6_o_i6);
    atg_a_o(t, OC(DIV), TYPE(F6), AST_CLS(O), TYPE(F6), AST_CLS(O), TYPE(F6), div_f6_o_f6_o_f6);
}
