
#include "mc.h"

mc *mc_i(size_t s, const alfr *af) {
    mc *m = af->a(sizeof(mc) + sizeof(uint8_t) * s);
    m->r = 1;
    m->s = s;
    m->l = 0;
    m->af = af;
    return m;
}

mc *mc_i_cstr(const char *s, const alfr *af) {
    size_t l = strlen(s) + sizeof(uint8_t); // len of string incs null byte
    mc *m = mc_i(l, af);
    m->l = l;
    memcpy(m->d, s, l);
    return m;
}

mc *mc_c(mc *m) {
    m->r++;
    return m;
}

bool mc_eq(const mc *restrict a, const mc *restrict b) {
    if (!a && !b) return true;
    if (!a || !b || a->l != b->l) return false;
    for (size_t i = 0; i < a->l; i++) if (a->d[i] != b->d[i]) return false;
    return true;
}

#ifndef MC_RESIZE
    #define MC_RESIZE 2
#endif

void mc_wa(mc **m, uint8_t b) {
    if ((*m)->l == (*m)->s) {
        mc *nm = mc_i((*m)->s * MC_RESIZE, (*m)->af);
        nm->l = (*m)->l;
        memcpy(nm->d, (*m)->d, nm->l);
        nm->af->f(*m);
        *m = nm;
    }
    (*m)->d[(*m)->l++] = b;
}

void mc_wb(mc **m, size_t l, uint8_t *b) {
    for (size_t i = 0; i < l; i++) mc_wa(m, b[i]);
}

void mc_f(mc *m) {
    if (!m || --m->r > 0) return;
    m->af->f(m);
}
