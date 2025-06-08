
#include "atg.h"

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *aoti, lst *q, gen *g, as *a) {
   atg *t = af->a(sizeof(atg));
   t->lc = 0;
   t->r = 1;
   t->af = af;
   t->ta = ta;
   t->saf = saf;
   t->sta = sta;
   t->aoti = aoti;
   t->q = q;
   t->g = g;
   t->a = a;
   t->at = aoti();
   t->ot = aoti();
   return t;
}

static atg_stat atg_lst_q(atg *t, lst *l, atg_enq enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = atg_q(t, h->d[0].p, enq)) != ATG_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

atg_stat atg_q(atg *t, te *an, atg_enq enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!an) return stat;
    if (enq(an)) lst_ab(t->q, P(&an));
    switch (an->d[2].u4) {
        case AST_CLS(R):
            return atg_q(t, an->d[4].p, enq);
        case AST_CLS(V):
            // TODO
            break;
        case AST_CLS(O):
            if ((stat = atg_q(t, an->d[5].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_q(t, an->d[6].p, enq);
        case AST_CLS(A):
            if ((stat = atg_q(t, an->d[4].p, enq)) != ATG_STAT(OK)) return stat;
            return atg_lst_q(t, an->d[5].p, enq);
        case AST_CLS(L):
            return atg_lst_q(t, an->d[4].p, enq);
        default:
            break;
    }
    return stat;
}

void atg_f(atg *t) {
    if (!t || --t->r > 0) return;
    gen_f(t->g);
    as_f(t->a);
    tbl_f(t->at);
    tbl_f(t->ot);
    t->af->f(t);
}
