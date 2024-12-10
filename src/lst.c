
#include "lst.h"

static lst_itm *lst_itm_i(alfn *ta, un d) {
    lst_itm *li = ta(sizeof(lst_itm));
    li->d = d;
    return li;
}

lst *lst_i(alfn *la, alfn *ta, frfn *tf, frfn *df, frfn *lf) {
    lst *l = la(sizeof(lst));
    l->r = 1;
    l->l = 0;
    l->la = la;
    l->ta = ta;
    l->tf = tf;
    l->df = df;
    l->lf = lf;
    return l;
}

lst *lst_c(lst *l) {
    l->r++;
    return l;
}

size_t lst_g_l(const lst *const l) {
    return l->l;
}

void lst_f(lst *l) {
    if (--l->r > 0) return;
    lst_itm *h = l->h;
    while (h) {
        lst_itm *tmp = h;
        h = h->n;
        if (l->df) l->df(tmp->d.p);
        l->tf(tmp);
    }
    l->lf(l);
}
