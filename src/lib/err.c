
#include "err.h"

err *err_i(const alfr *af, err_d_p dp, err_d_f df, void *d, const char *m) {
    err *e = af->a(sizeof(err));
    e->r = 1;
    e->af = af;
    e->dp = dp;
    e->df = df;
    e->d = d;
    e->m = m;
    return e;
}

void err_p(const err *e) {
    printf("\e[91m%s\e[0m\n", e->m);
    if (e->d) {
        e->dp(e->d);
        putchar('\n');
    }
}

void err_f(err *e) {
    if (!e || --e->r > 0) return;
    if (e->d) e->df(e->d);
    e->af->f(e);
}
