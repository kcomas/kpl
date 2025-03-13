
#include "atg.h"

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *ati, atg_lst_i ali, gen *g, as *a) {
   atg *t = af->a(sizeof(atg));
   t->tc = t->lc = 0;
   t->r = 1;
   t->af = af;
   t->ta = ta;
   t->saf = saf;
   t->sta = sta;
   t->ati = ati;
   t->ali = ali;
   t->bg = g;
   t->a = a;
   t->q = ali();
   t->se = ali();
   t->at = ati();
   t->ot = ati();
   return t;
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

atg_stat atg_q(atg *t, te **an, atg_test_fn enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!*an) return stat;
    if (enq(*an)) lst_ab(t->q, P(an));
    switch ((*an)->d[2].u4) {
        case AST_CLS(R):
            return atg_q(t, (te**) &(*an)->d[4].p, enq);
        case AST_CLS(V):
            // TODO
            break;
        case AST_CLS(O):
            if ((stat = atg_q(t, (te**) &(*an)->d[5].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_q(t, (te**) &(*an)->d[6].p, enq);
        case AST_CLS(A):
            if ((stat = atg_q(t, (te**) &(*an)->d[4].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_lst_q(t, (*an)->d[5].p, enq);
        case AST_CLS(L):
            return atg_lst_q(t, (*an)->d[4].p, enq);
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

static void add_e(atg *t, tbl *ot, uint16_t c, uint16_t ct, te **kv, bool at) {
    un hsh = U6(0);
    hsh = u4_s_o(hsh, AST_HSH_C, c);
    hsh = u4_s_o(hsh, AST_HSH_T, ct);
    if (tbl_g_i(ot, hsh, kv) == TBL_STAT(NF)) {
        *kv = te_i(2, t->ta, NULL);
        (*kv)->d[0] = hsh;
        if (at) (*kv)->d[1] = P(t->ati());
        tbl_a(ot, *kv);
    }
}

atg_stat atg_a_o(atg *t, uint16_t oc, type ct, ast_cls lc, type lt, ast_cls rc, type rt, atg_cc_fn cc) {
    atg_stat stat = ATG_STAT(OK);
    tbl *ot = t->ot;
    te *kv;
    add_e(t, ot, oc, ct, &kv, true);
    ot = kv->d[1].p;
    add_e(t, ot, lc, lt, &kv, true);
    ot = kv->d[1].p;
    add_e(t, ot, rc, rt, &kv, false);
    if (kv->d[1].p) return ATG_STAT(INV);
    kv->d[1] = P(cc);
    return stat;
}

static atg_stat run_cc(atg *t, gen *g, te *an, te **e);

static atg_stat run_cc_lst(atg *t, gen *g, lst *l, te **e) {
    atg_stat stat = ATG_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = run_cc(t, g, h->d[0].p, e)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

static atg_stat run_cc(atg *t, gen *g, te *an, te **e) {
    atg_stat stat = ATG_STAT(OK);
    if (!an) return stat;
    switch (an->d[2].u4) {
        case AST_CLS(R):
            if ((stat = run_cc(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            break;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(S):
            break;
        case AST_CLS(V):
            // TODO
            break;
        case AST_CLS(O):
            if ((stat = run_cc(t, g, an->d[5].p, e)) != ATG_STAT(OK) || (stat = run_cc(t, g, an->d[6].p, e)) != ATG_STAT(OK)) return stat;
            break;
        case AST_CLS(Z):
            if ((stat = run_cc(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            break;
        case AST_CLS(A):
            if ((stat = run_cc(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            if ((stat = run_cc_lst(t, g, an->d[5].p, e)) != ATG_STAT(OK)) return stat;
            break;
        case AST_CLS(L):
            if ((stat = run_cc_lst(t, g, an->d[4].p, e)) != ATG_STAT(OK)) return stat;
            break;
        default:
            *e = te_c(an);
            return ATG_STAT(INV);
    }
    switch (an->d[2].u4) {
        case AST_CLS(T):
        case AST_CLS(E):
            break;
        // run
        default:
            *e = te_c(an);
            return ATG_STAT(INV);
    }
    return ATG_STAT(OK);
}

atg_stat atg_qn(atg *t, te **e) {
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
    gen *g = gen_cpy(t->bg);
    if ((stat = sf(t, g, *rn, e)) != ATG_STAT(OK) || (stat = run_cc(t, g, *rn, e)) != ATG_STAT(OK) || (stat = ef(t, g, *rn, e)) != ATG_STAT(OK)) return stat;
    gen_p(g, NULL);
    HERE("TODO");
    return stat;
}

void atg_f(atg *t) {
    if (!t || --t->r > 0) return;
    gen_f(t->bg);
    as_f(t->a);
    lst_f(t->q);
    lst_f(t->se);
    tbl_f(t->at);
    tbl_f(t->ot);
    t->af->f(t);
}
