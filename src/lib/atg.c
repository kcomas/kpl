
#include "atg.h"

atg *atg_i(const alfr *af, const alfr *ta, const alfr *ea, atg_err_fn efn, atg_tbl_i ati, lst *q, lst *se, tbl *dt, gen *g, as *a) {
   atg *t = af->a(sizeof(atg));
   t->tc = t->lc = 0;
   t->r = 1;
   t->af = af;
   t->ta = ta;
   t->ea = ea;
   t->efn = efn;
   t->ati = ati;
   t->bg = g;
   t->a = a;
   t->q = q;
   t->se = se;
   t->dt = dt;
   t->vt = ati();
   t->at = ati();
   t->ot = ati();
   t->zt = ati();
   return t;
}

atg *atg_i_atg(const atg *t) {
    atg *tt = t->af->a(sizeof(atg));
    tt->tc = tt->lc = 0;
    tt->r = 1;
    tt->af = t->af;
    tt->ta = t->ta;
    tt->ea = t->ea;
    tt->efn = t->efn;
    tt->ati = t->ati;
    tt->bg = gen_i_gen(t->bg);
    tt->a = as_i_as(t->a);
    tt->q = lst_i_lst(t->q);
    tt->dt = tbl_c(t->dt);
    tt->se = lst_c(t->se);
    tt->vt = tbl_c(t->vt);
    tt->at = tbl_c(t->at);
    tt->ot = tbl_c(t->ot);
    tt->zt = tbl_c(t->zt);
    return tt;
}

static void atg_s_g(te *an, gen *g) {
    te *c = g->code->t;
    while (((te*) c->d[0].p)->d[0].u4 == GEN_OP(LBL)) c = c->d[1].p;
    c = c->d[0].p;
    switch (an->d[2].u4) {
        case AST_CLS(R):
        case AST_CLS(Z):
        case AST_CLS(A):
            an->d[AST_MIN_LEN + 2] = P(c);
            break;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(I):
            an->d[AST_MIN_LEN] = P(c);
            break;
        case AST_CLS(S):
        case AST_CLS(V):
        case AST_CLS(L):
        case AST_CLS(C):
            an->d[AST_MIN_LEN + 1] = P(c);
            break;
        case AST_CLS(O):
            an->d[AST_MIN_LEN + 3] = P(c);
            break;
    }
}

te *atg_g_g(te *an) {
    switch (an->d[2].u4) {
        case AST_CLS(R):
        case AST_CLS(Z):
        case AST_CLS(A):
            return an->d[AST_MIN_LEN + 2].p;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(I):
            return an->d[AST_MIN_LEN].p;
        case AST_CLS(S):
        case AST_CLS(V):
        case AST_CLS(L):
        case AST_CLS(C):
            return an->d[AST_MIN_LEN + 1].p;
        case AST_CLS(O):
            return an->d[AST_MIN_LEN + 3].p;
        default:
            break;
    }
    return NULL;
}

static atg_stat atg_lst_q(atg *t, lst *l, atg_test_fn enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = atg_q(t, (te**) &h->d[0].p, enq)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

static void atg_dt_f(void *p) {
    te *t = p;
    te_f(t->d[0].p);
    t->af->f(t);
}

static void _dt_a(atg *t, te *type);

static void _dt_a_tbl(atg *t, tbl *tt) {
    te *h = tt->i->h, *n;
    while (h) {
        n = h->d[0].p;
        _dt_a(t, n->d[2].p);
        h = h->d[2].p;
    }
}

static void _dt_a(atg *t, te *type) {
    te *kv;
    if (!type_is_des(type->d[1].u4) || tbl_g_i(t->dt, P(type), &kv) == TBL_STAT(OK)) return;
    kv = te_i(2, t->ta, atg_dt_f);
    kv->d[0] = P(te_c(type));
    tbl_a(t->dt, kv);
    switch (type_g_c(type->d[1].u4)) {
        case TYPE_CLS(V):
            _dt_a(t, type->d[2].p);
            break;
        case TYPE_CLS(H):
            _dt_a_tbl(t, type->d[2].p);
            break;
        case TYPE_CLS(C):
            switch (type->d[1].u4) {
                case TYPE(TE):
                    for (size_t i = 2; i < type->l; i++) _dt_a(t, type->d[i].p);
                    break;
                case TYPE(KV):
                    STOP("TODO");
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

static void atg_dt_a(atg *t, te *an) {
    te *nt;
    if (ast_g_t(an, &nt) == AST_STAT(OK)) _dt_a(t, nt);
}

atg_stat atg_q(atg *t, te **an, atg_test_fn enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!*an) return stat;
    atg_dt_a(t, *an);
    if (enq(*an)) lst_ab(t->q, P(an));
    switch ((*an)->d[2].u4) {
        case AST_CLS(R):
            return atg_q(t, (te**) &(*an)->d[4].p, enq);
        case AST_CLS(V):
            return atg_lst_q(t, (*an)->d[4].p, enq);
        case AST_CLS(O):
            if ((stat = atg_q(t, (te**) &(*an)->d[5].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_q(t, (te**) &(*an)->d[6].p, enq);
        case AST_CLS(A):
            if ((stat = atg_q(t, (te**) &(*an)->d[4].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_lst_q(t, (*an)->d[5].p, enq);
        case AST_CLS(L):
            return atg_lst_q(t, (*an)->d[4].p, enq);
        case AST_CLS(Z):
            atg_dt_a(t, (*an)->d[4].p);
            break;
        default:
            break;
    }
    return stat;
}

void atg_a_se(atg *t, atg_test_fn tse, atg_cc_fn s, atg_cc_fn e) {
    te *se = te_i(3, t->ta, NULL);
    se->d[0] = P(tse);
    se->d[1] = P(s);
    se->d[2] = P(e);
    lst_ab(t->se, P(se));
}

static void atg_tbl_entry_f(void *p) {
    te *t = p;
    tbl_f(t->d[1].p);
    t->af->f(t);
}

static void add_e(atg *t, tbl *ot, uint16_t c, uint16_t ct, te **kv, bool at) {
    un hsh = U6(0);
    hsh = u4_s_o(hsh, AST_HSH_C, c);
    hsh = u4_s_o(hsh, AST_HSH_T, ct);
    if (tbl_g_i(ot, hsh, kv) == TBL_STAT(NF)) {
        *kv = te_i(2, t->ta, at ? atg_tbl_entry_f : NULL);
        (*kv)->d[0] = hsh;
        if (at) (*kv)->d[1] = P(t->ati());
        tbl_a(ot, *kv);
    }
}

atg_stat atg_a_v(atg *t, type vt, atg_cc_fn cc) {
    te *kv;
    add_e(t, t->vt, AST_CLS(V), vt, &kv, false);
    kv->d[1] = P(cc);
    return ATG_STAT(OK);
}

atg_stat atg_a_a(atg *t, type rt, ast_cls tc, type tt, atg_cc_fn cc) {
    tbl *at = t->at;
    te *kv;
    add_e(t, at, AST_CLS(A), rt, &kv, true);
    at = kv->d[1].p;
    add_e(t, at, tc, tt, &kv, false);
    kv->d[1] = P(cc);
    return ATG_STAT(OK);
}

atg_stat atg_a_o(atg *t, uint16_t oc, type ct, ast_cls lc, type lt, ast_cls rc, type rt, atg_cc_fn cc) {
    tbl *ot = t->ot;
    te *kv;
    add_e(t, ot, oc, ct, &kv, true);
    ot = kv->d[1].p;
    add_e(t, ot, lc, lt, &kv, true);
    ot = kv->d[1].p;
    add_e(t, ot, rc, rt, &kv, false);
    if (kv->d[1].p) return ATG_STAT(INV);
    kv->d[1] = P(cc);
    return ATG_STAT(OK);
}

atg_stat atg_a_z(atg *t, type it, ast_cls nc, type nt, atg_cc_fn cc) {
    tbl *zt = t->zt;
    te *kv;
    add_e(t, zt, AST_CLS(Z), it, &kv, true);
    zt = kv->d[1].p;
    add_e(t, zt, nc, nt, &kv, false);
    kv->d[1] = P(cc);
    return ATG_STAT(OK);
}

static atg_stat cc_r(atg *t, gen *g, te *an, err **e, tbl *tt, size_t n, ...) {
    atg_stat stat;
    te *kv;
    va_list args;
    va_start(args, n);
    while (n > 0) {
        if (tbl_g_i(tt, va_arg(args, un), &kv) == TBL_STAT(NF)) return t->efn(t, an, e, "atg_r_cc_r");
        n--;
        if (n > 0) tt = kv->d[1].p;
    }
    va_end(args);
    if ((stat = ((atg_cc_fn*) kv->d[1].p)(t, g, an, e)) != ATG_STAT(OK)) return stat;
    atg_s_g(an, g);
    return ATG_STAT(OK);
}

static atg_stat cc(atg *t, gen *g, te *an, err **e) {
    un ha = U6(0);
    te *tn;
    switch (an->d[2].u4) {
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(S):
            return ATG_STAT(OK);
        case AST_CLS(V):
            ha = u4_s_o(ha, AST_HSH_C, AST_CLS(V));
            tn = an->d[3].p;
            ha = u4_s_o(ha, AST_HSH_T, tn->d[1].u4);
            return cc_r(t, g, an, e, t->vt, 1, ha);
        case AST_CLS(O):
            ha = u4_s_o(ha, AST_HSH_C, an->d[4].u4);
            tn = an->d[3].p;
            ha = u4_s_o(ha, AST_HSH_T, tn->d[1].u4);
            return cc_r(t, g, an, e, t->ot, 3, ha, ast_hsh(an->d[5].p), ast_hsh(an->d[6].p));
        case AST_CLS(A):
            ha = u4_s_o(ha, AST_HSH_C, AST_CLS(A));
            tn = an->d[3].p;
            ha = u4_s_o(ha, AST_HSH_T, tn->d[1].u4);
            return cc_r(t, g, an, e, t->at, 2, ha, ast_hsh(an->d[4].p));
        case AST_CLS(Z):
            ha = u4_s_o(ha, AST_HSH_C, AST_CLS(Z));
            tn = ((te*) an->d[3].p)->d[2].p;
            if (tn) ha = u4_s_o(ha, AST_HSH_T, tn->d[1].u4);
            else u4_s_o(ha, AST_HSH_T, TYPE(_N));
            return cc_r(t, g, an, e, t->zt, 2, ha, ast_hsh(an->d[4].p));
        case AST_CLS(L):
            return ATG_STAT(OK);
        default:
            break;
    }
    return t->efn(t, an, e, "atg_r_cc");
}

atg_stat atg_lst_r(atg *t, gen *g, lst *l, err **e) {
    atg_stat stat = ATG_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = atg_r(t, g, h->d[0].p, e)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

atg_stat atg_r(atg *t, gen *g, te *an, err **e) {
    atg_stat stat = ATG_STAT(OK);
    if (!an) return stat;
    switch (an->d[2].u4) {
        case AST_CLS(R):
            if ((stat = atg_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            break;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(S):
        case AST_CLS(V):
            break; // items called when building
        case AST_CLS(O):
            if (an->d[5].p && ((te*) an->d[5].p)->d[2].u4 == AST_CLS(L) && an->d[6].p && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(L)) break; // for loops ifs match cc in op...
            if ((stat = atg_r(t, g, an->d[6].p, e)) != ATG_STAT(OK) || (stat = atg_r(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat; // right before left
            break;
        case AST_CLS(Z):
        case AST_CLS(A):
            break; // called when building
        case AST_CLS(L):
            if ((stat = atg_lst_r(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            break;
        default:
            return t->efn(t, an, e, "atg_r");
    }
    return cc(t, g, an, e);
}

atg_stat atg_d_n(atg *t, te *h, gen **g, void **fn, atg_d_n_fn adnfn, err **e) {
    if (!h) return ATG_STAT(INV);
    if (!type_has_refs(h->d[0].p)) return ATG_STAT(OK);
    return adnfn(t->bg, h->d[0].p, g, fn, e) == GEN_STAT(OK) ? ATG_STAT(OK) : ATG_STAT(INV);
}

atg_stat atg_n(atg *t, gen **g, ast *a, err **e) {
    atg_stat stat = ATG_STAT(OK);
    un v;
    if (lst_sb(t->q, &v) != LST_STAT(OK)) return ATG_STAT(INV);
    te **rn = v.p;
    atg_cc_fn *sf = NULL, *ef = NULL;
    te *h = t->se->h;
    while (h) {
        te *fns = h->d[0].p;
        if (((atg_test_fn*) fns->d[0].p)(*rn)) {
            sf = fns->d[1].p;
            ef = fns->d[2].p;
            break;
        }
        h = h->d[2].p;
    }
    if (!sf || !ef) return ATG_STAT(INV);
    *g = gen_i_gen(t->bg);
    if ((stat = sf(t, *g, *rn, e)) != ATG_STAT(OK) || (stat = atg_r(t, *g, *rn, e)) != ATG_STAT(OK) || (stat = ef(t, *g, *rn, e)) != ATG_STAT(OK)) return stat;
    te *nn = ast_an_i(a, (*rn)->d[0].p, (*rn)->d[1].p, AST_CLS(S), P(type_s_i(a->ta, NULL, TYPE(_G))), U5((*g)->lbl));
    te_f(*rn);
    *rn = nn;
    return stat;
}

void atg_f(atg *t) {
    if (!t || --t->r > 0) return;
    gen_f(t->bg);
    as_f(t->a);
    lst_f(t->q);
    lst_f(t->se);
    tbl_f(t->dt);
    tbl_f(t->vt);
    tbl_f(t->at);
    tbl_f(t->ot);
    tbl_f(t->zt);
    t->af->f(t);
}
