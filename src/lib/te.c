
#include "te.h"

static void td(void *p) {
    te *t = p;
    t->af->f(t);
}

te *te_i(size_t l, const alfr *af, frfn *tf) {
    te *t = af->a(sizeof(te) + sizeof(un) * l);
    t->r = 1;
    t->l = l;
    t->af = af;
    t->tf = tf ? tf : td;
    for (size_t i = 0; i < l; i++) t->d[i] = P(NULL);
    return t;
}

te *te_i_t(const te *t) {
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
    t->tf(t);
}
