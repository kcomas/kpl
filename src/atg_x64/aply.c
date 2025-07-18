
#include "../atg_x64.h"

static void vr_ab_nc(gen *g, size_t l, vr **v, un d) {
    if (!*v) *v = vr_i(l, g->va, (void*) te_f);
    vr_ab(v, d);
}

static atg_stat lst_args_var(atg *t, gen *g, err **e, lst *l, vr **v) {
    atg_stat stat = ATG_STAT(OK);
    if (!l) return stat;
    uint32_t flgs;
    te *h = l->h, *an, *lte, *arg;
    while (h) {
        an = h->d[0].p;
        if ((stat = atg_r(t, g, an, e)) != ATG_STAT(OK)) return stat;
        switch (an->d[2].u4) {
            case AST_CLS(E):
                lte = an->d[3].p;
                flgs = ast_lst_tbl_e_g_f(lte);
                if (flgs & LTE_FLG(A)) arg = gen_arg(g, type_g_x64_type(lte->d[2].p), ast_lst_tbl_e_g_i(lte));
                else if (flgs & LTE_FLG(L)) arg = gen_stkv(g, type_g_x64_type(lte->d[2].p), ast_lst_tbl_e_g_i(lte));
                else if (flgs & LTE_FLG(Y)) arg = gen_tmp(g, type_g_x64_type(lte->d[2].p), ast_lst_tbl_e_g_i(lte));
                else if (flgs & LTE_FLG(S)) arg = gen_stka(g, x64_type_to_ref(type_g_x64_type(lte->d[2].p)), ast_lst_tbl_e_g_i(lte));
                else return atg_err(t, an, e, "atg inv lst args FLG");
                break;
            case AST_CLS(S):
                arg = gen_data(g, type_g_x64_type(an->d[3].p), an->d[4]);
                break;
            case AST_CLS(O):
            case AST_CLS(A):
                arg = te_c((atg_g_g(an))->d[1].p);
                break;
            default:
                return atg_err(t, an, e, "atg lst arg cls inv");
        }
        if (gen_var_g_t(arg) == X64_TYPE(M)) {
            if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(MM), t->tc++), arg, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
            arg = te_c(((te*) g->code->t->d[0].p)->d[1].p);
        }
        vr_ab_nc(g, l->l, v, P(arg));
        h = h->d[2].p;
    }
    return stat;
}

static te *fn_call_lbl(gen *g, const te *an) {
    uint32_t lbl = ast_lst_tbl_e_g_i(an->d[3].p);
    return gen_lbl(g, lbl);
}

static atg_stat fn_call(atg *t, gen *g, te *restrict an, gen_op go, te *restrict ca, te *restrict tgt) {
    x64_type xt = type_g_x64_type(an->d[3].p);
    if (xt == X64_TYPE(N)) {
        if (ca) {
            if (gen_a(g, go, ca, tgt, NULL) != GEN_STAT(OK)) return ATG_STAT(INV);
        } else if (gen_a(g, go, tgt, NULL, NULL) != GEN_STAT(OK)) return ATG_STAT(INV);
    } else {
        if (ca) {
            if (gen_a(g, go, gen_tmp(g, xt, t->tc++), ca, tgt) != GEN_STAT(OK)) return ATG_STAT(INV);
        } else if (gen_a(g, go, gen_tmp(g, xt, t->tc++), tgt, NULL) != GEN_STAT(OK)) return ATG_STAT(INV);
    }
    return ATG_STAT(OK);
}

static atg_stat aply_fn(atg *t, gen *g, te *restrict an, err **e, te *restrict call) {
    atg_stat stat = ATG_STAT(OK);
    vr *v = NULL;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if ((stat = fn_call(t, g, an, go, gen_call_v(g, v), call)) != ATG_STAT(OK)) {
        vr_f(v);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return stat;
}

static atg_stat aply_e_fn(atg *t, gen *g, te *an, err **e) {
    return aply_fn(t, g, an, e, fn_call_lbl(g, an->d[4].p));
}

static atg_stat aply_s_fn(atg *t, gen *g, te *an, err **e) {
    return aply_fn(t, g, an, e, gen_data(g, X64_TYPE(U5), ((te*) an->d[4].p)->d[4]));
}

static atg_stat aply_s_fp(atg *t, gen *g, te *an, err **e) {
    return aply_fn(t, g, an, e, gen_data(g, X64_TYPE(M), ((te*) an->d[4].p)->d[4]));
}

static atg_stat aply_e_nf(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    uint32_t flgs, id;
    vr *v = NULL, *s = NULL;
    te *h = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p, *ps, *pn, *lte, *kv;
    tbl *lt;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    if (!h) return atg_err(t, an, e, "atg inv nf scope");
    h = ((tbl*) h->d[4].p)->i->h;
    while (h) {
        lte = h->d[0].p;
        ps = an;
        while (ps) {
            if (ast_g_pn(AST_CLS(L), ps, &pn) != AST_STAT(OK)) return atg_err(t, an, e, "atg cant get pn for scope var");
            lt = pn->d[3].p;
            if (lt && tbl_g_i(lt, lte->d[0], &kv) == TBL_STAT(OK)) {
                flgs = ast_lst_tbl_e_g_f(kv);
                id = ast_lst_tbl_e_g_i(kv);
                if (flgs & LTE_FLG(F)) break;
                if (flgs & LTE_FLG(L)) {
                    vr_ab_nc(g, lt->i->l, &s, P(gen_stkv(g, type_g_x64_type(kv->d[2].p), id)));
                } else return atg_err(t, an, e, "atg load scope var nyi");
                break;
            }
            ps = pn->d[0].p;
            if (ps->d[2].u4 == AST_CLS(O) && ps->d[4].u4 == OC(CST)) {
                pn = ps->d[5].p;
                if (pn->d[2].u4 == AST_CLS(T)) {
                    kv = pn->d[3].p;
                    if (kv->d[1].u4 == TYPE(FN)) return atg_err(t, an, e, "atg cannot search for scope past fn");
                    if (kv->d[1].u4 == TYPE(NF) && tbl_g_i(kv->d[4].p, lte->d[0], &kv) == TBL_STAT(OK)) {
                        id = ast_lst_tbl_e_g_i(kv);
                        vr_ab_nc(g, 1, &s, P(gen_stka(g, x64_type_to_ref(type_g_x64_type(kv->d[2].p)), id)));
                        break;
                    }
                }
            }
        }
        h = h->d[2].p;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if ((stat = fn_call(t, g, an, go, gen_call_w(g, s, v), fn_call_lbl(g, an->d[4].p))) != ATG_STAT(OK)) {
        vr_f(v);
        vr_f(s);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return stat;
}

static atg_stat aply_e_te(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    size_t n;
    te *lte;
    if (an->d[0].p && ((te*) an->d[0].p)->d[2].u4 == AST_CLS(O) && ((te*) an->d[0].p)->d[4].u4 == OC(AGN)) return ATG_STAT(OK);
    if ((stat = aply_te_g_idx(t, an, e, &n, &lte)) != ATG_STAT(OK)) return stat;
    x64_type xt = type_g_x64_type(an->d[3].p);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, t->tc++), gen_idx_m(g, xt, 2, var_arg(g, lte, X64_TYPE(M)), atg_te_idx_d(g, n)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat aply_z_te(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
    te *sn = ((lst*) an->d[5].p)->h->d[0].p, *tn = atg_g_g(an->d[4].p)->d[1].p;
    x64_type xt = gen_var_g_t(tn);
    if (sn->d[2].u4 != AST_CLS(S)) return atg_err(t, an, e, "atg aply_z_te inv access");
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, t->tc++), gen_idx_m(g, xt, 2, te_c(tn), atg_te_idx_d(g, sn->d[4].i6)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

#define CS_BYTES_NUM 50

#define CS_PAD 10

static atg_stat aply_e_cs(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t ts1 = t->tc++, ts2 = t->tc++, ts3 = t->tc++;
    mc *s = ((te*) an->d[4].p)->d[4].p;
    uint64_t ms = s->l + CS_PAD;
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(U6), ts1), gen_data(g, X64_TYPE(U6), U6(0)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    lst *l = an->d[5].p;
    te *h = l->h, *tn;
    while (h) {
        if (ast_g_t(h->d[0].p, &tn) != AST_STAT(OK)) return atg_err(t, an, e, "atg aply cs inv arg type");
        switch (tn->d[1].u4) {
            case TYPE(I6):
            case TYPE(F6):
                ms += CS_BYTES_NUM;
                break;
            default:
                return atg_err(t, an, e, "atg aply cs inv arg type");
        }
        h = h->d[2].p;
    }
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), ts1), gen_tmp(g, X64_TYPE(U6), ts1), gen_data(g, X64_TYPE(U6), U6(ms))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ts2), gen_call_m(g, 2, gen_tmp(g, X64_TYPE(U6), ts1), gen_data(g, X64_TYPE(M), P(&al_mc))), gen_data(g, X64_TYPE(M), P(mc_i))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    vr *pv = vr_i(l->l + 4, g->va, (void*) te_f);
    if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), ts3), gen_tmp(g, X64_TYPE(M), ts2), gen_data(g, X64_TYPE(U3), U3(offsetof(mc, d)))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    vr_ab(&pv, P(gen_tmp(g, X64_TYPE(M), ts3)));
    vr_ab(&pv, P(gen_tmp(g, X64_TYPE(U6), ts1)));
    vr_ab(&pv, P(gen_data(g, X64_TYPE(S), P(s))));
    if ((stat = lst_args_var(t, g, e, l, &pv)) != ATG_STAT(OK)) {
        vr_f(pv);
        return stat;
    }
    if (gen_a(g, GEN_OP(CALLV), gen_tmp(g, X64_TYPE(U6), ts1), gen_call_v(g, pv), gen_data(g, X64_TYPE(M), P(snprintf))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), ts1), gen_tmp(g, X64_TYPE(U6), ts1), gen_data(g, X64_TYPE(U6), U6(1))) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ts2), gen_data(g, X64_TYPE(U3), U3(offsetof(mc, l)))), gen_tmp(g, X64_TYPE(U6), ts1), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if (gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ts2), NULL, NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

atg_stat atg_aply_acc(atg *t, gen *g, te *restrict an, err **e, te *restrict vlte, te **i, uint32_t erlb) {
    te *x = *i;
    if (gen_var_g_t(x) == X64_TYPE(I6) && gen_var_g_c(x) == GEN_CLS(D) && x->d[1].i6 < 0) {
        if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(I6), t->tc++), gen_idx_m(g, X64_TYPE(U6), 2, var_arg(g, vlte, X64_TYPE(M)), gen_data(g, X64_TYPE(U3), U3(offsetof(vr, l)))), x) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        x = te_c(((te*) g->code->t->d[0].p)->d[1].p);
    }
    if (gen_var_g_c(x) == GEN_CLS(D) || gen_var_g_t(x) == X64_TYPE(I6)) {
        if(gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(U6), t->tc++), gen_var_g_c(x) == GEN_CLS(D) ? x : te_c(x), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
        *i = te_c(((te*) g->code->t->d[0].p)->d[1].p);
    }
    if (gen_var_g_c(x) != GEN_CLS(D) && gen_a(g, GEN_OP(LT), x, gen_data(g, X64_TYPE(I6), I6(0)), gen_lbl(g, erlb)) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

#define VR_ERR_STR_LEN 30

const char *atg_vr_inv_str = "inv vr idx";

gen_stat aply_e_vr_err_c(gen *g, te *restrict i, te *restrict v, uint32_t erlb, uint32_t glb, uint32_t ei, uint32_t esi, uint32_t si, uint32_t ii, mc *fname, uint16_t lno, uint32_t cno) {
    if (gen_a(g, GEN_OP(LT), i, gen_idx_m(g, X64_TYPE(U6), 2, v, gen_data(g, X64_TYPE(U3), U3(offsetof(vr, l)))), gen_lbl(g, glb)) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, erlb), NULL, NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(CALL), gen_tmp(g, X64_TYPE(M), ii), gen_call_m(g, 2, gen_data(g, X64_TYPE(U6), U6(VR_ERR_STR_LEN)), gen_data(g, X64_TYPE(M), P(&al_mc))), gen_data(g, X64_TYPE(M), P(mc_i))) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(REF), gen_tmp(g, X64_TYPE(M), si), gen_tmp(g, X64_TYPE(M), ii), gen_data(g, X64_TYPE(U3), U3(offsetof(mc, d)))) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(CALLV), gen_tmp(g, X64_TYPE(U6), esi), gen_call_m(g, 4, gen_tmp(g, X64_TYPE(M), si), gen_data(g, X64_TYPE(U6), U6(VR_ERR_STR_LEN - 1)), gen_char(g, "%lu"), te_c(i)), gen_data(g, X64_TYPE(M), P(snprintf))) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(ADD), gen_tmp(g, X64_TYPE(U6), esi), gen_tmp(g, X64_TYPE(U6), esi), gen_data(g, X64_TYPE(U6), U6(1))) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ii), gen_data(g, X64_TYPE(U3), U3(offsetof(mc, l)))), gen_tmp(g, X64_TYPE(U6), esi), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    return atg_rt_err_c(g, ei, si, ii, fname, lno, cno, atg_vr_inv_str);
}

gen_stat aply_e_vr_c(gen *g, te *restrict i, te *restrict v, uint32_t erlb, uint32_t glb, uint32_t el, uint32_t ui, uint32_t ei, uint32_t esi, uint32_t si, uint32_t ii, uint32_t vi, mc *fname, uint16_t lno, uint32_t cno, x64_type xt, bool ref, uint64_t esym, uint64_t vsym) {
    if (aply_e_vr_err_c(g, i, v, erlb, glb, ei, esi, si, ii, fname, lno, cno) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(g, X64_TYPE(U6), U6(esym)), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*)))), gen_tmp(g, X64_TYPE(M), ei), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(JMP), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, glb), NULL, NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, xt, vi), gen_idx_m(g, xt, 3, te_c(v), gen_data(g, X64_TYPE(U3), U3(offsetof(vr, d))), te_c(i)), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (ref && inc_ref_c(g, vi) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(g, X64_TYPE(U6), U6(vsym)), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(SET), gen_idx_m(g, X64_TYPE(N), 2, gen_tmp(g, X64_TYPE(M), ui), gen_data(g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*)))), gen_tmp(g, xt, vi), NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    if (gen_a(g, GEN_OP(LBL), gen_lbl(g, el), NULL, NULL) != GEN_STAT(OK)) return GEN_STAT(INV);
    return gen_a(g, GEN_OP(NOP), gen_tmp(g, X64_TYPE(M), ui), NULL, NULL);
}

static atg_stat aply_e_vr(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t erlb = t->lc++, glb = t->lc++, el = t->lc++, ui = t->tc++, ei = t->tc++, esi = t->tc++, si = t->tc++, ii = t->tc++, vi = t->tc++;
    uint64_t esym, vsym;
    te *vlte = ((te*) an->d[4].p)->d[3].p, *i = ((lst*) an->d[5].p)->h->d[0].p, *vt = ((te*) vlte->d[2].p)->d[2].p, *tkn = ((te*) an->d[1].p)->d[2].p, *pn = an->d[0].p;
    if (pn->d[2].u4 == AST_CLS(O) && pn->d[4].u4 == OC(AGN)) return ATG_STAT(OK);
    if ((stat = atg_an_var(t, g, &i, e)) != ATG_STAT(OK)) return stat;
    if ((stat = atg_te_init(t, g, an, e, an->d[3].p, 2, ui)) != ATG_STAT(OK)) return stat;
    if ((stat = atg_aply_acc(t, g, an, e, vlte, &i, erlb)) != ATG_STAT(OK)) return stat;
    if (atg_g_un_ev(an->d[3].p, &esym, &vsym) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv `e`v for un");
    if (aply_e_vr_c(g, i, var_arg(g, vlte, X64_TYPE(M)), erlb, glb, el, ui, ei, esi, si, ii, vi, node_root_fname(an->d[1].p), tkn_m_g_l(tkn), tkn_m_g_c(tkn), type_g_x64_type(vt), type_is_ref(vt->d[1].u4), esym, vsym) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    return ATG_STAT(OK);
}

static atg_stat aply_e_cj(atg *t, gen *g, te *an, err **e) {
    atg_stat stat;
    uint32_t ci = t->tc++, idx = 0, fi = t->tc++;
    te *lte = ((te*) an->d[4].p)->d[3].p, *h, *n;
    vr *v = NULL, *s = NULL;
    if ((stat = lst_args_var(t, g, e, an->d[5].p, &v)) != ATG_STAT(OK)) {
        vr_f(v);
        return stat;
    }
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), ci), var_arg(g, lte, X64_TYPE(M)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    h = ((tbl*) ((te*) ((te*) lte->d[2].p)->d[2].p)->d[4].p)->i->h;
    while (h) {
        n = h->d[0].p;
        vr_ab_nc(g, 1, &s, P(gen_idx_m(g, type_g_x64_type(n->d[2].p), 2, gen_tmp(g, X64_TYPE(M), ci), atg_te_idx_d(g, idx++))));
        h = h->d[2].p;
    }
    gen_op go = GEN_OP(CALL);
    if ((stat = call_npr(&go, an)) != ATG_STAT(OK)) return atg_err(t, an, e, "atg inv reg prev");
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), fi), gen_idx_m(g, X64_TYPE(M), 2, gen_tmp(g, X64_TYPE(M), ci), atg_te_idx_d(g, idx)), NULL) != GEN_STAT(OK)) return atg_err(t, an, e, __FUNCTION__);
    if ((stat = fn_call(t, g, an, go, gen_call_w(g, s, v), gen_tmp(g, X64_TYPE(M), fi))) != ATG_STAT(OK)) {
        vr_f(v);
        vr_f(s);
        return atg_err(t, an, e, __FUNCTION__);
    }
    return ATG_STAT(OK);
}

void atg_aply(atg *t) {
    atg_a_a(t, TYPE(VD), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(I6), AST_CLS(S), TYPE(FN), aply_s_fn);
    atg_a_a(t, TYPE(UN), AST_CLS(S), TYPE(FP), aply_s_fp);
    atg_a_a(t, TYPE(UN), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(U6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(F6), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(CJ), AST_CLS(E), TYPE(FN), aply_e_fn);
    atg_a_a(t, TYPE(VD), AST_CLS(E), TYPE(NF), aply_e_nf);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(NF), aply_e_nf);
    atg_a_a(t, TYPE(F6), AST_CLS(E), TYPE(NF), aply_e_nf);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(TE), aply_e_te);
    atg_a_a(t, TYPE(SG), AST_CLS(Z), TYPE(TE), aply_z_te);
    atg_a_a(t, TYPE(SG), AST_CLS(S), TYPE(CS), aply_e_cs);
    atg_a_a(t, TYPE(UN), AST_CLS(E), TYPE(VR), aply_e_vr);
    atg_a_a(t, TYPE(I6), AST_CLS(E), TYPE(CJ), aply_e_cj);
}
