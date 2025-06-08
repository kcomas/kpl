
#include "mc.h"

mc *mc_i(size_t s, alfn *ma, frfn *mf) {
    mc *m = ma(sizeof(mc) + sizeof(uint8_t) * s);
    m->r = 1;
    m->s = s;
    m->l = 0;
    m->ma = ma;
    m->mf = mf;
    return m;
}

mc *mc_c(mc *m) {
    m->r++;
    return m;
}

#ifndef MC_RESIZE
    #define MC_RESIZE 2
#endif

void mc_wb(mc **m, uint8_t b) {
    if ((*m)->l == (*m)->s) {
        mc *nm = mc_i((*m)->s * MC_RESIZE, (*m)->ma, (*m)->mf);
        nm->l = nm->l;
        memcpy(nm->d, (*m)->d, nm->l);
        nm->mf(m);
        *m = nm;
    }
    (*m)->d[(*m)->l++] = b;
}

void mv_f(mc *m) {
    if (!m || --m->r > 0) return;
    m->mf(m);
}
