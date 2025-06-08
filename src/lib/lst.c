
#include "lst.h"

static lst_itm *lst_itm_i(alfn *ta, un d) {
    lst_itm *li = ta(sizeof(lst_itm));
    li->p = li->n = NULL;
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
    l->h = l->t = NULL;
    return l;
}

lst *lst_c(lst *l) {
    l->r++;
    return l;
}

size_t lst_g_l(const lst *const l) {
    return l->l;
}

lst_stat lst_ab(lst *const l, un d) {
    lst_itm *li = lst_itm_i(l->ta, d);
    if (!l->h) l->h = l->t = li;
    else {
        li->p = l->t;
        l->t = l->t->n = li;
    }
    l->l++;
    return LST_STAT(OK);
}

lst_stat lst_af(lst *const l, un d) {
    lst_itm *li = lst_itm_i(l->ta, d);
    if (!l->t) l->t = l->h = li;
    else {
        li->n = l->h;
        l->h = l->h->p = li;
    }
    l->l++;
    return LST_STAT(OK);
}

lst_stat lst_sb(lst *const l, un *d) {
    if (l->l == 0) return LST_STAT(SUB);
    lst_itm *li = l->t;
    l->t = l->t->p;
    l->t->n = NULL;
    *d = li->d;
    l->tf(li);
    l->l--;
    return LST_STAT(OK);
}

lst_stat lst_sf(lst *const l, un *d) {
    if (l->l == 0) return LST_STAT(SUB);
    lst_itm *li = l->h;
    l->h = l->h->n;
    l->h->p = NULL;
    *d = li->d;
    l->tf(li);
    l->l--;
    return LST_STAT(OK);
}

void lst_f(lst *l) {
    if (!l || --l->r > 0) return;
    lst_itm *h = l->h;
    while (h) {
        lst_itm *tmp = h;
        h = h->n;
        if (l->df) l->df(tmp->d.p);
        l->tf(tmp);
    }
    l->lf(l);
}
