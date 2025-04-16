
#include "lst.h"

#define LST_POOL 100

static lst *lstp[LST_POOL];

static _Atomic size_t lpi = 0;

static void *al(size_t n) {
    (void) n;
    if (!lpi) return malloc(sizeof(lst));
    return lstp[--lpi];
}

static void fr(void *p) {
    if (lpi == LST_POOL) {
        free(p);
        return;
    }
    lstp[lpi++] = p;
}

const alfr al_lst = { .a = al, .f = fr };

static __attribute__((destructor)) void al_lst_f(void) {
    for (size_t i = 0; i < lpi; i++) free(lstp[i]);
}

lst *lst_i(const alfr *af, const alfr *ta, frfn *df) {
    lst *l = af->a(0);
    l->r = 1;
    l->l = 0;
    l->af = af;
    l->ta = ta;
    l->df = df;
    l->h = l->t = NULL;
    return l;
}

lst *lst_i_v(const alfr *af, const alfr *ta, frfn *df, size_t n, ...) {
    lst *l = lst_i(af, ta, df);
    va_list args;
    va_start(args, n);
    while (n > 0) {
        if (lst_ab(l, va_arg(args, un)) != LST_STAT(OK)) {
            lst_f(l);
            return NULL;
        }
        n--;
    }
    va_end(args);
    return l;
}

lst *lst_i_lst(const lst *l) {
    return lst_i(l->af, l->ta, l->df);
}

lst *lst_c(lst *l) {
    l->r++;
    return l;
}

size_t lst_g_l(const lst *l) {
    return l->l;
}

bool lst_eq(const lst *restrict a, const lst *restrict b, lst_cmp_fn fn) {
    if (!a && !b) return true;
    if (!a || !b || a->l != b->l) return false;
    te *ah = a->h, *bh = b->h;
    while (ah && bh) {
        if (!(fn(ah->d[0], bh->d[0]))) return false;
        ah = ah->d[2].p;
        bh = bh->d[2].p;
    }
    return true;
}

lst_stat lst_ab(lst *l, un d) {
    te *li = te_i(3, l->ta, NULL);
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
    te *li = te_i(3, l->ta, NULL);
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
    if (l->t) l->t->d[2] = P(NULL);
    else l->h = l->t;
    *d = li->d[0];
    te_f(li);
    l->l--;
    return LST_STAT(OK);
}

lst_stat lst_sf(lst *l, un *d) {
    if (l->l == 0) return LST_STAT(SUB);
    te *li = l->h;
    l->h = l->h->d[2].p;
    if (l->h) l->h->d[1] = P(NULL);
    else l->t = l->h;
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
    l->af->f(l);
}
