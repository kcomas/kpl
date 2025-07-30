
#include "lst.h"

#define LST_POOL 100

static lst *lstp[LST_POOL];

static size_t lpi = 0;

static void *al(size_t n) {
    (void) n;
#ifdef NPOOL
    return malloc(sizeof(lst));
#else
    if (!lpi) return malloc(sizeof(lst));
    return lstp[--lpi];
#endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    if (lpi == LST_POOL) return free(p);
    lstp[lpi++] = p;
#endif
}

const alfr al_lst = { .a = al, .f = fr };

static __attribute__((destructor(102))) void al_lst_f(void) {
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

lst_stat lst_g_i(lst *l, un v, size_t *idx) {
    if (!l->h) return LST_STAT(NF);
    size_t i = 0;
    te *h = l->h;
    while (h) {
        if (h->d[0].u6 == v.u6) {
            *idx = i;
            return LST_STAT(OK);
        }
        h = h->d[2].p;
        i++;
    }
    return LST_STAT(NF);
}

static te *ms(te *h, size_t c, lst_cmp_fn fn) {
    te *th = NULL, *n = NULL, *t, *l, *r;
    if (c < 2) return h;
    t = h;
    size_t ch = c / 2;
    for (size_t i = 0; i < ch; i++) t = t->d[2].p;
    l = t->d[1].p;
    l->d[2] = t->d[1] = P(NULL);
    l = ms(h, ch, fn);
    r = ms(t, c - ch, fn);
    while (l && r) {
        if (fn(l->d[0], r->d[0]) < 1) {
            t = l;
            l = l->d[2].p;
        } else {
            t = r;
            r = r->d[2].p;
        }
        if (!th) th = n = t;
        else {
            n->d[2] = P(t);
            t->d[1] = P(n);
            n = n->d[2].p;
        }
    }
    t = l ? l : r;
    while (t) {
        n->d[2] = P(t);
        t->d[1] = P(n);
        n = n->d[2].p;
        t = t->d[2].p;
    }
    return th;
}

void lst_s(lst *l, lst_cmp_fn fn) {
    te *h = l->h, *nh;
    l->h = l->t = NULL;
    nh = ms(h, l->l, fn);
    l->h = nh;
    while (nh) {
        l->t = nh;
        nh = nh->d[2].p;
    }
}

bool lst_eq(const lst *restrict a, const lst *restrict b, lst_eq_fn fn) {
    if (!a && !b) return true;
    if (!a || !b || a->l != b->l) return false;
    te *ah = a->h, *bh = b->h;
    while (ah && bh) {
        if (!(fn(ah->d[0], bh->d[0]))) return false;
        ah = ah->d[2].p;
        bh = bh->d[2].p;
    }
    return (ah || bh) ? false : true;
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

lst_stat lst_insb(lst *l, un d, un t, lst_eq_fn fn) {
    te *h = l->h, *p;
    while (h) {
        if (fn(h->d[0], t)) {
            te *li = te_i(3, l->ta, NULL);
            li->d[0] = d;
            li->d[2] = P(h);
            p = h->d[1].p;
            h->d[1] = P(li);
            if (!p) l->h = li;
            else p->d[2] = P(li);
            l->l++;
            return LST_STAT(OK);
        }
        h = h->d[2].p;
    }
    return LST_STAT(NF);
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
