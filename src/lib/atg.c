
#include "atg.h"

atg *atg_i(const alfr *af, const alfr *ta, const alfr *saf, const alfr *sta, atg_tbl_i *aoti, gen *g, as *a, lst *q) {
   atg *t = af->a(sizeof(atg));
   t->lc = 0;
   t->r = 1;
   t->af = af;
   t->ta = ta;
   t->saf = saf;
   t->sta = sta;
   t->aoti = aoti;
   t->g = g;
   t->a = a;
   t->q = q;
   t->at = aoti();
   t->ot = aoti();
   return t;
}

atg_stat atg_q(atg *t, te *an, atg_enq enq) {
    atg_stat stat = ATG_STAT(OK);
    if (!an) return stat;
    if (enq(an)) lst_ab(t->q, P(te_c(an)));
    HERE("TODO");
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
