
#include "mc.h"

static mc *mah = NULL;

static void *al(size_t s) {
#ifdef NPOOL
    return malloc(sizeof(mc) + sizeof(uint8_t) * s);
#else
    mc *h = mah;
    while (h) {
        if (h->s >= s) break;
        h = ((mc**) h->d)[1];
    }
    if (h && h == mah) mah = ((mc**) mah->d)[1];
    else if (h) {
        ((mc**) ((mc**) h->d)[0]->d)[1] = ((mc**) h->d)[1];
        if (((mc**) h->d)[1]) ((mc**) ((mc**) h->d)[1]->d)[0] = ((mc**) h->d)[0];
    } else h = malloc(sizeof(mc) + sizeof(uint8_t) * s);
    return h;
 #endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    mc *m = p;
    ((mc**) m->d)[0] = NULL;
    ((mc**) m->d)[1] = NULL;
    if (mah) {
        ((mc**) mah->d)[0] = m;
        ((mc**) m->d)[1] = mah;
    }
    mah = m;
#endif
}

const alfr al_mc = { .a = al, .f = fr };

static __attribute__((destructor(103))) void al_mc_f(void) {
    while (mah) {
        mc *tmp = mah;
        mah = ((mc**) mah->d)[1];
        free(tmp);
    }
}

#define MC_S_MIN (sizeof(void*) * 2)

mc *mc_i(size_t s, const alfr *af) {
    if (s < MC_S_MIN) s = MC_S_MIN;
    mc *m = af->a(s);
    m->r = 1;
    m->s = s;
    m->l = 0;
    m->af = af;
    memset(m->d, 0, s);
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

static void rs(mc **m) {
    if ((*m)->l + 1 < (*m)->s) return;
    mc *nm = mc_i((*m)->s * MC_RESIZE, (*m)->af);
    nm->l = (*m)->l;
    memcpy(nm->d, (*m)->d, nm->l);
    nm->af->f(*m);
    *m = nm;
}

void mc_wa(mc **m, uint8_t b) {
    rs(m);
    (*m)->d[(*m)->l++] = b;
}

void mc_wf(mc **m, uint8_t b) {
    rs(m);
    for (ssize_t i = (*m)->l; i > 0; i--) (*m)->d[i] = (*m)->d[i - 1];
    (*m)->d[0] = b;
    (*m)->l++;
}

void mc_wa_v(mc **m, size_t n, ...) {
    va_list args;
    va_start(args, n);
    while (n > 0) {
        mc_wa(m, (uint8_t) va_arg(args, int));
        n--;
    }
    va_end(args);
}

void mc_wb(mc **m, size_t l, uint8_t *b) {
    for (size_t i = 0; i < l; i++) mc_wa(m, b[i]);
}

void mc_wc4(mc **m, un c) {
    mc_wa(m, c.c.a);
    if (c.c.b) {
        mc_wa(m, c.c.b);
        if (c.c.c) {
            mc_wa(m, c.c.c);
            if (c.c.d) {
                mc_wa(m, c.c.d);
            }
        }
    }
}

void mc_wcstr(mc **m, const char *s) {
    while (*s) mc_wa(m, *s++);
}

mc *mc_str_cnct(const mc *restrict a, const mc *restrict b) {
    mc *c = mc_i(a->l + b->l + 1, a->af);
    char *s = (char*) a->d;
    while (*s) mc_wa(&c, *s++);
    s = (char*) b->d;
    while (*s) mc_wa(&c, *s++);
    mc_wa(&c, '\0');
    return c;
}

void mc_f(mc *m) {
    if (!m || --m->r > 0) return;
    m->af->f(m);
}
