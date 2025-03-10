
#include "atg.h"

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *aoti, atg_lst_i ali, gen *g, as *a) {
   atg *t = af->a(sizeof(atg));
   t->lc = 0;
   t->r = 1;
   t->af = af;
   t->ta = ta;
   t->saf = saf;
   t->sta = sta;
   t->aoti = aoti;
   t->ali = ali;
   t->bg = g;
   t->a = a;
   t->q = ali();
   t->se = ali();
   t->at = aoti();
   t->ot = aoti();
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

atg_stat atg_qn(atg *t) {
    un v;
    if (lst_sb(t->q, &v) != LST_STAT(OK)) return ATG_STAT(INV);
    te **rn = v.p;
    atg_cc_fn *s = NULL, *e = NULL;
    te *h = t->se->h;
    while (h) {
        te *fns = h->d[0].p;
        if (((atg_test_fn*) fns->d[0].p)(*rn)) {
            s = fns->d[1].p;
            e = fns->d[2].p;
            break;
        }
        h = h->d[2].p;
    }
    if (!s || !e) return ATG_STAT(INV);
    // impl next gen
    HERE("TODO");
    return ATG_STAT(OK);
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
