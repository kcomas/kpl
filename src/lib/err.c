
#include "err.h"

#define ERR_POOL 100

static err *errp[ERR_POOL];

static size_t epi = 0;

static void *al(size_t n) {
    (void) n;
#ifdef NPOOL
    return malloc(sizeof(err));
#else
    if (!epi) return malloc(sizeof(err));
    return errp[--epi];
#endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    if (epi == ERR_POOL) return free(p);
    errp[epi++] = p;
#endif
}

const alfr al_err = { .a = al, .f = fr };

static __attribute__((destructor(102))) void al_err_f(void) {
    for (size_t i = 0; i < epi; i++) free(errp[i]);
}

err *err_i(const alfr *af, err_d_p dp, err_d_f df, void *d, const char *m) {
    err *e = af->a(0);
    e->r = 1;
    e->af = af;
    e->dp = dp;
    e->df = df;
    e->d = d;
    e->m = m;
    return e;
}

void err_p(const err *e, bool nl) {
    if (!e) {
        printf("\e[3;4;1;31mNULL ERR PRINT\e[0m\n");
        return;
    }
    if (!e->m && !e->d) {
        printf("\e[3;4;1;31mEMPTY ERR PRINT\e[0m\n");
        return;
    }
    if (e->m) printf("\e[1;91m%s\e[0m\n", e->m);
    if (e->d && e->dp) e->dp(e->d);
    if (nl) putchar('\n');
}

void err_f(err *e) {
    if (!e || --e->r > 0) return;
    if (e->d && e->df) e->df(e->d);
    e->af->f(e);
}
