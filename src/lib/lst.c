
#include "lst.h"

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

size_t lst_g_l(const lst *l) {
    return l->l;
}

lst_stat lst_ab(lst *l, un d) {
    te *li = te_i(3, l->ta, l->tf);
    li->d[0] = d;
    if (!l->h) l->h = l->t = li;
    else {
        li->d[1].p = l->t;
        l->t = l->t->d[2].p = li;
    }
    l->l++;
    return LST_STAT(OK);
}

lst_stat lst_af(lst *l, un d) {
    te *li = te_i(3, l->ta, l->tf);
    li->d[0] = d;
    if (!l->t) l->t = l->h = li;
    else {
        li->d[2].p = l->h;
        l->h = l->h->d[1].p = li;
    }
    l->l++;
    return LST_STAT(OK);
}

lst_stat lst_sb(lst *l, un *d) {
    if (l->l == 0) return LST_STAT(SUB);
    te *li = l->t;
    l->t = l->t->d[1].p;
    l->t->d[2] = P(NULL);
    *d = li->d[0];
    te_f(li);
    l->l--;
    return LST_STAT(OK);
}

lst_stat lst_sf(lst *l, un *d) {
    if (l->l == 0) return LST_STAT(SUB);
    te *li = l->h;
    l->h = l->h->d[2].p;
    l->h->d[1] = P(NULL);
    *d = li->d[0];
    te_f(li);
    l->l--;
    return LST_STAT(OK);
}

void lst_li_d(lst *l, te *li) {
    if (li == l->h) l->h = l->h->d[2].p;
    else if (li == l->t) l->t = l->t->d[1].p;
    if (li->d[1].p && li->d[2].p) {
        ((te*) li->d[1].p)->d[2].p = li->d[2].p;
        ((te*) li->d[2].p)->d[1].p = li->d[1].p;
    } else if (li->d[1].p && !li->d[2].p) ((te*) li->d[1].p)->d[2] = P(NULL);
    else if (!li->d[1].p && li->d[2].p) ((te*) li->d[2].p)->d[1] = P(NULL);
    te_f(li);
    l->l--;
}

void lst_f(lst *l) {
    if (!l || --l->r > 0) return;
    te *h = l->h;
    while (h) {
        te *tmp = h;
        h = h->d[2].p;
        if (l->df) l->df(tmp->d[0].p);
        te_f(tmp);
    }
    l->lf(l);
}
