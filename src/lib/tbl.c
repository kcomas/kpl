
#include "tbl.h"

tbl *tbl_i(alfn *ta, frfn *tf, hhfn *hf, cmpfn *cf, lst *i, te *b) {
    tbl *t = ta(sizeof(tbl));
    t->r = 1;
    t->ta = ta;
    t->tf = tf;
    t->hf = hf;
    t->cf = cf;
    t->i = i;
    t->b = b;
    return t;
}

tbl *tbl_c(tbl *t) {
    t->r++;
    return t;
}

size_t tbl_g_l(const tbl *const t) {
    return t->i->l;
}

size_t tbl_g_s(const tbl* const t) {
    return t->b->l;
}

#ifndef TBL_RES
    #define TBL_RES ((double) 0.9)
#endif

static tbl *resize(tbl *t) {
}

#define TBI(T, I) T->b->d[I].p
#define TBIT(T, I) ((te*) ((te*) TBI(T, I))->d[0].p)

tbl_stat tbl_a(tbl **t, te *kv) {
    tbl_stat tstat = TBL_STAT(OK);
    if ((double) (*t)->i->l / (double) (*t)->b->l >= TBL_RES) {
        *t = resize(*t);
        tstat = TBL_STAT(RES);
    }
    lst_ab((*t)->i, P(kv));
    size_t idx = (*t)->hf(kv->d[0]) % (*t)->b->l;
    size_t i = idx;
    do {
        if ((*t)->b->d[i].p == NULL) {
            (*t)->b->d[i].p = (*t)->i->t;
            break;
        } else if ((*t)->cf(TBIT((*t), i)->d[0], kv->d[0])) {
            te_f(TBI((*t), i));
            TBI((*t), i) = kv;
            break;
        }
        i = (i + 1) % (*t)->b->l;
    } while (i != idx);
    if (i == idx) return TBL_STAT(OAE);
    return tstat;
}

void tbl_f(tbl *t) {
    if (!t || --t->r > 0) return;
    te_f(t->b);
    lst_f(t->i);
    t->tf(t);
}
