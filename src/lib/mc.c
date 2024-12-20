
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

mc *mc_i_cstr(const char *const s, alfn *ma, frfn *mf) {
    size_t l = strlen(s);
    mc *m = mc_i(l, ma, mf);
    m->l = l;
    memcpy(m->d, s, l);
    return m;
}

mc *mc_c(mc *m) {
    m->r++;
    return m;
}

#ifndef MC_RESIZE
    #define MC_RESIZE 2
#endif

void mc_wa(mc **m, uint8_t b) {
    if ((*m)->l == (*m)->s) {
        mc *nm = mc_i((*m)->s * MC_RESIZE, (*m)->ma, (*m)->mf);
        nm->l = (*m)->l;
        memcpy(nm->d, (*m)->d, nm->l);
        nm->mf(*m);
        *m = nm;
    }
    (*m)->d[(*m)->l++] = b;
}

void mc_wb(mc **m, uint8_t *b, size_t l) {
    for (size_t i = 0; i < l; i++) mc_wa(m, b[i]);
}

void mc_f(mc *m) {
    if (!m || --m->r > 0) return;
    m->mf(m);
}
