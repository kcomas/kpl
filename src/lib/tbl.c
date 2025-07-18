
#include "tbl.h"

#define TBL_POOL 100

static tbl *tblp[TBL_POOL];

static size_t tpi = 0;

static void *al(size_t n) {
    (void) n;
#ifdef NPOOL
    return malloc(sizeof(tbl));
#else
    if (!tpi) return malloc(sizeof(tbl));
    return tblp[--tpi];
#endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    if (tpi == TBL_POOL) return free(p);
    tblp[tpi++] = p;
#endif
}

const alfr al_tbl = { .a = al, .f = fr };

static __attribute__((destructor(101))) void al_tbl_f(void) {
    for (size_t i = 0; i < tpi; i++) free(tblp[i]);
}

tbl *tbl_i(const alfr *af, hhfn *hf, cmpfn *cf, lst *i, te *b) {
    tbl *t = af->a(0);
    t->r = 1;
    t->af = af;
    t->hf = hf;
    t->cf = cf;
    t->i = i;
    t->b = b;
    return t;
}

tbl *tbl_i_tbl(const tbl *t) {
    return tbl_i(t->af, t->hf, t->cf, lst_i_lst(t->i), te_i_te(t->b));
}

tbl *tbl_c(tbl *t) {
    t->r++;
    return t;
}

bool tbl_eq(const tbl *restrict a, const tbl *restrict b, tbl_cmp_fn cmp) {
   if (!a && !b) return true;
   if (!a || !b || a->i->l != b->i->l) return false;
   te *ah = a->i->h;
   te *bh = b->i->h;
   while (ah && bh) {
        if (!cmp(ah->d[0].p, bh->d[0].p)) return false;
        ah = ah->d[2].p;
        bh = bh->d[2].p;
   }
   return true;
}

size_t tbl_no_hsh(un d) {
    return d.u6;
}

size_t tbl_mc_sdbm(un d) {
    mc *md = d.p;
    size_t hsh = 0;
    for (size_t i = 0; i < md->l; i++) hsh = md->d[i] + (hsh << 6) + (hsh << 16) - hsh;
    return hsh;
}

bool tbl_un_eq(un a, un b) {
    return a.u6 == b.u6;
}

bool tbl_mc_eq(un a, un b) {
    return mc_eq(a.p, b.p);
}

size_t tbl_g_l(const tbl *t) {
    return t->i->l;
}

size_t tbl_g_s(const tbl* const t) {
    return t->b->l;
}

static tbl_stat fdrm(tbl *t, un k, te **kv, bool rm) {
    size_t idx = t->hf(k) % t->b->l;
    size_t i = idx;
    do {
        te *li = t->b->d[i].p;
        if (li && t->cf(((te*) li->d[0].p)->d[0], k)) {
            *kv = li->d[0].p;
            if (rm) {
                lst_li_d(t->i, li);
                t->b->d[i] = P(NULL);
            }
            return TBL_STAT(OK);
        }
        i = (i + 1) % t->b->l;
    } while (i != idx);
    return TBL_STAT(NF);
}

tbl_stat tbl_g_i(tbl *t, un k, te **kv) {
    return fdrm(t, k, kv, false);
}

#ifndef TBL_RES
    #define TBL_RES ((double) 0.9)
#endif

#ifndef TBL_MUL
    #define TBL_MUL 2
#endif

static tbl_stat rsize(tbl *t) {
    te *nb = te_i(t->b->l * TBL_MUL, t->b->af, t->b->tf);
    te_f(t->b);
    t->b = nb;
    te *h = t->i->h;
    while (h) {
        size_t idx = t->hf(((te*) h->d[0].p)->d[0]) % t->b->l;
        size_t i = idx;
        do {
            if (t->b->d[i].p == NULL) {
                t->b->d[i].p = h;
                i = t->b->l;
                break;
            }
            i = (i + 1) % t->b->l;
        } while (i != idx);
        if (i == idx) return TBL_STAT(OAE);
        h = h->d[2].p;
    }
    return TBL_STAT(RES);
}

static bool ins(tbl *t, te *kv) {
    size_t idx = t->hf(kv->d[0]) % t->b->l;
    size_t i = idx, r = 0, maxr = t->b->l - t->i->l; // total buckets - used buckets
    do {
        te *li = t->b->d[i].p;
        if (li == NULL) {
            lst_ab(t->i, P(kv));
            t->b->d[i].p = t->i->t; // weak ref
            if (r >= maxr) return rsize(t);
            return TBL_STAT(OK);
        } else if (t->cf(((te*) li->d[0].p)->d[0], kv->d[0])) {
            te_f(li->d[0].p);
            li->d[0].p = kv;
            if (r >= maxr) return rsize(t);
            return TBL_STAT(OK);
        }
        i = (i + 1) % t->b->l;
        r++;
    } while (i != idx);
    return TBL_STAT(OAE);
}

tbl_stat tbl_a(tbl *t, te *kv) {
    tbl_stat tstat = TBL_STAT(OK);
    if ((tstat = ins(t, kv)) > TBL_STAT(RES)) return tstat;
    if ((double) t->i->l / (double) t->b->l >= TBL_RES) return rsize(t);
    return tstat;
}

tbl_stat tbl_s(tbl *t, un k, te **kv) {
    return fdrm(t, k, kv, true);
}

void tbl_f(tbl *t) {
    if (!t || --t->r > 0) return;
    te_f(t->b);
    lst_f(t->i);
    t->af->f(t);
}
