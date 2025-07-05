
#include "te.h"

#define MPIS 20 // max pool item size

static te *tah = NULL;

static void *al(size_t n) {
    if (n < 2) STOP("TE MUST HAVE LENGTH OF AT LEAST 2");
#ifdef NPOOL
    return malloc(sizeof(te) + sizeof(un) * n);
#else
    if (n > MPIS) return malloc(sizeof(te) + sizeof(un) * n);
    te *h = tah;
    while (h) {
        if (h->l == n) break;
        h = h->d[1].p;
    }
    if (h && h == tah) tah = tah->d[1].p;
    else if (h) {
        ((te*) h->d[0].p)->d[1] = h->d[1];
        if (h->d[1].p) ((te*) h->d[1].p)->d[0] = h->d[0];
    } else h = malloc(sizeof(te) + sizeof(un) * n);
    return h;
#endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    te *t = p;
    if (t->l > MPIS) return free(t);
    t->d[0] = P(NULL);
    t->d[1] = P(NULL);
    if (tah) {
        tah->d[0] = P(t);
        t->d[1] = P(tah);
    }
    tah = t;
#endif
}

const alfr al_te = { .a = al, .f = fr };

static __attribute__((destructor(103))) void al_te_f(void) {
    while (tah) {
        te *tmp = tah;
        tah = tah->d[1].p;
        free(tmp);
    }
}

static void td(void *p) {
    te *t = p;
    t->af->f(t);
}

te *te_i(size_t l, const alfr *af, frfn *tf) {
    te *t = af->a(l);
    t->r = 1;
    t->l = l;
    t->af = af;
    t->tf = tf ? tf : td;
    for (size_t i = 0; i < l; i++) t->d[i] = P(NULL);
    return t;
}

te *te_i_te(const te *t) {
    return te_i(t->l, t->af, t->tf);
}

te *te_c(te *t) {
    t->r++;
    return t;
}

size_t te_g_l(const te *t) {
    return t->l;
}

void te_f(te *t) {
    if (!t || --t->r > 0) return;
    t->tf(t); // can be jit code
}
