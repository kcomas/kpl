
#include "te.h"

te *te_i(size_t l, alfn *ta, frfn *tf) {
    te *t = ta(sizeof(te) + sizeof(un) * l);
    t->r = 1;
    t->l = l;
    t->ta = ta;
    t->tf = tf;
    for (size_t i = 0; i < l; i++) t->d[i] = P(NULL);
    return t;
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
