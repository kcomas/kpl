
#include "../atg_x64.h"

static atg_stat z_e_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t eid;
    te *lte = an->d[0].p;
    if (lte && lte->d[2].u4 == AST_CLS(O) && lte->d[4].u4 == OC(AGN)) return ATG_STAT(OK);
    if ((stat = z_lte_id(t, an, e, an, &lte, &eid)) != ATG_STAT(OK)) return stat;
    x64_type xt = type_g_x64_type(((te*) an->d[3].p)->d[2].p);
    if (gen_a(g, xt == X64_TYPE(M) ? GEN_OP(REF) : GEN_OP(SET), gen_tmp(g, xt == X64_TYPE(M) ? X64_TYPE(MM) : xt, t->tc++), gen_idx_m(g, xt, 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, eid)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_oa_st(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t id;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    te *st = ((te*) an->d[4].p)->d[3].p, *kv, *rc = atg_g_g(an->d[4].p)->d[1].p;
    if (tbl_g_i(st->d[2].p, an->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for st tbl");
    if (lst_g_i(((tbl*) st->d[2].p)->i, P(kv), &id) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for st tbl itm");
    x64_type xt = type_g_x64_type(kv->d[2].p);
    if (gen_a(g, GEN_OP(REF), gen_tmp(g, x64_type_to_ref(xt), t->tc++), gen_idx_m(g, xt, 2, te_c(rc), atg_te_idx_d(g, id)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_e_un_lte(atg *t, gen *g, te *restrict an, err **e, te *restrict lte) {
    atg_stat stat;
    uint32_t el = t->lc++, ri = t->tc++, si = t->tc++, ti = t->tc++;
    size_t eid = 0;
    te *ut, *kv;
    if (ast_g_t(an->d[4].p, &ut) != AST_STAT(OK)) return atg_err(t, an, e, "atg inv type for Z");
    if (tbl_g_i(ut->d[2].p, an->d[5], &kv) != TBL_STAT(OK)) return atg_err(t, an, e, "atg inv key for un tbl");
    if (lst_g_i(((tbl*) ut->d[2].p)->i, P(kv), &eid) != LST_STAT(OK)) return atg_err(t, an, e, "atg inv idx for un tbl itm");
    if (gen_a(g, GEN_OP(EQ), gen_idx_m(g, X64_TYPE(U6), 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(eid)), gen_lbl(g, el)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = un_inv_a(t, g, an, e, kv->d[0].p, ri, si, ti)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    kv = kv->d[2].p;
    x64_type xt = type_g_x64_type(kv);
    if (xt == X64_TYPE(MM) && !type_is_weak(kv->d[1].u4)) {
        if (gen_a(g, GEN_OP(REF), gen_tmp(g, xt, t->tc++), gen_idx_m(g, xt, 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    } else if (gen_a(g, xt == X64_TYPE(M) ? GEN_OP(REF) : GEN_OP(SET), gen_tmp(g, xt == X64_TYPE(M) ? X64_TYPE(MM) : xt, t->tc++), gen_idx_m(g, xt, 2, gen_stkv(g, X64_TYPE(M), ast_lst_tbl_e_g_i(lte)), atg_te_idx_d(g, 1)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_e_un(atg *t, gen *g, te *an, err **e) {
    return z_e_un_lte(t, g, an, e, ((te*) an->d[4].p)->d[3].p);
}

static atg_stat z_e_un_o(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    return z_e_un_lte(t, g, an, e, ((te*) ((te*) an->d[4].p)->d[5].p)->d[3].p);
}

static atg_stat z_e_un_z(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    te *pn = an->d[0].p;
    if (pn && pn->d[2].u4 == AST_CLS(O) && pn->d[4].u4 == OC(AGN)) return ATG_STAT(OK);
    return atg_err(t, an, e, "nyi");
}

static atg_stat z_un(atg *t, gen *g, te *restrict an, err **e, te *restrict type) {
    atg_stat stat;
    size_t vidx = 0;
    uint32_t ui = t->tc++;
    te *ut = ((te*) an->d[3].p)->d[2].p, *h = ((tbl*) ut->d[2].p)->i->h, *n, *lte = NULL;
    while (h) {
        n = ((te*) h->d[0].p)->d[2].p;
        if ((type && type_eq(n, type)) || (!type && n->d[1].u4 == TYPE(VD))) break;
        h = h->d[2].p;
        vidx++;
    }
    if ((stat = atg_te_init(t, g, an, e, ut, 2, ui)) != ATG_STAT(OK)) return stat;
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), atg_te_idx_d(g, 0)), gen_data(g, X64_TYPE(U6), U6(vidx)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (type) {
        n = an->d[4].p;
        if (n->d[2].u4 == AST_CLS(E)) lte = n->d[3].p;
        if (atg_an_var(g, &n) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv z un val");
        if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), atg_te_idx_d(g, 1)), n, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        if (type_is_ref(type->d[1].u4)) {
            if (lte && (ast_lst_tbl_e_g_f(lte) & LTE_FLG(M))) {
                if (gen_a(g, GEN_OP(SET), var_arg(g, lte, X64_TYPE(M)), gen_data(g, X64_TYPE(M), P(NULL)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            } else {
                h = gen_idx_m(g, X64_TYPE(I6), 2, te_c(n), gen_data(g, X64_TYPE(U3), U3(0)));
                if (gen_a(g, GEN_OP(ADD), h, te_c(h), gen_data(g, X64_TYPE(I6), I6(1))) != GEN_STAT(OK)) return atg_err(t, an, e, "atg inv z un ref inc");
            }
        }
    }
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ui), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat z_un_(atg *t, gen *g, te *an, err **e) {
    return z_un(t, g, an, e, NULL);
}

static atg_stat z_un_s_i6(atg *t, gen *g, te *an, err **e) {
    te *type = an->d[4].p;
    return z_un(t, g, an, e, type->d[3].p);
}

static atg_stat z_un_o_i6(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    te *type = an->d[4].p;
    return z_un(t, g, an, e, type->d[3].p);
}

static atg_stat z_un_e_(atg *t, gen *g, te *an, err **e) {
    te *lte = ((te*) an->d[4].p)->d[3].p;
    return z_un(t, g, an, e, lte->d[2].p);
}

void atg_z(atg *t) {
    atg_a_z(t, TYPE(U6), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(I6), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(UN), AST_CLS(E), TYPE(ST), z_e_st);
    atg_a_z(t, TYPE(UN), AST_CLS(O), TYPE(ST), z_oa_st);
    atg_a_z(t, TYPE(UN), AST_CLS(A), TYPE(ST), z_oa_st);
    atg_a_z(t, TYPE(I6), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(F6), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(WSG), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(WVR), AST_CLS(E), TYPE(UN), z_e_un);
    atg_a_z(t, TYPE(I6), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(F6), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(WSG), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(WTE), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(WVR), AST_CLS(O), TYPE(UN), z_e_un_o);
    atg_a_z(t, TYPE(ST), AST_CLS(Z), TYPE(UN), z_e_un_z);
    atg_a_z(t, TYPE(UN), AST_CLS(_), TYPE(_N), z_un_);
    atg_a_z(t, TYPE(UN), AST_CLS(S), TYPE(I6), z_un_s_i6);
    atg_a_z(t, TYPE(UN), AST_CLS(O), TYPE(I6), z_un_o_i6);
    atg_a_z(t, TYPE(UN), AST_CLS(E), TYPE(VR), z_un_e_);
    atg_a_z(t, TYPE(UN), AST_CLS(E), TYPE(SG), z_un_e_);
    atg_a_z(t, TYPE(VD), AST_CLS(S), TYPE(I6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(S), TYPE(F6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(O), TYPE(I6), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(O), TYPE(SG), atg_nop);
    atg_a_z(t, TYPE(VD), AST_CLS(V), TYPE(VR), atg_nop);
}
