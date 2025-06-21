
#include "vr.h"

static vr *vah = NULL;

#define VR_S_MIN 5

static void *al(size_t s) {
    if (s < VR_S_MIN) s = VR_S_MIN;
#ifdef NPOOL
    return malloc(sizeof(vr) + sizeof(un) * s);
#else
    vr *h = vah;
    while (h) {
        if (h->s >= s) break;
        h = h->d[1].p;
    }
    if (h && h == vah) vah = vah->d[1].p;
    else if (h) {
        ((vr*) h->d[0].p)->d[1] = h->d[1];
        if (h->d[1].p) ((vr*) h->d[1].p)->d[0] = h->d[0];
    } else h = malloc(sizeof(vr) + sizeof(un) * s);
    return h;
#endif
}

static void fr(void *p) {
#ifdef NPOOL
    return free(p);
#else
    vr *v = p;
    v->d[0] = P(NULL);
    v->d[1] = P(NULL);
    if (vah) {
        vah->d[0] = P(v);
        v->d[1] = P(vah);
    }
    vah = v;
#endif
}

const alfr al_vr = { .a = al, .f = fr };

static __attribute__((destructor(103))) void al_vr_f(void) {
    while (vah) {
        vr *tmp = vah;
        vah = vah->d[1].p;
        free(tmp);
    }
}

vr *vr_i(size_t s, const alfr *af, frfn *df) {
    vr *v = af->a(s);
    v->r = 1;
    v->s = s;
    v->l = 0;
    v->af = af;
    v->df = df;
    for (size_t i = 0; i < s; i++) v->d[i] = P(NULL);
    return v;
}

vr *vr_i_vr(const vr *v) {
    return vr_i(v->s, v->af, v->df);
}

bool vr_eq(const vr *restrict a, const vr *const b, vr_eq_fn fn) {
    if (!a && !b) return true;
    if (!a || !b || a->l != b->l) return false;
    for (size_t i = 0; i < a->l; i++) if (!fn(a->d[i], b->d[i])) return false;
    return true;
}

vr *vr_c(vr *v) {
    v->r++;
    return v;
}

size_t vr_g_s(const vr *v) {
    return v->s;
}

size_t vr_g_l(const vr *v) {
    return v->l;
}

vr_stat vr_g_i(vr *v, size_t i, un *d) {
    if (i >= v->l) return VR_STAT(IDX);
    *d = v->d[i];
    return VR_STAT(OK);
}

vr_stat vr_s_i(vr *v, size_t i, un d) {
    if (i >= v->l) return VR_STAT(IDX);
    if (v->df) v->df(v->d[i].p);
    v->d[i] = d;
    return VR_STAT(OK);
}

#ifndef VR_RES
    #define VR_RES 2
#endif

static vr *resize(vr *v) {
    vr *nv = vr_i(v->s * VR_RES, v->af, v->df);
    nv->l = v->l;
    for (size_t i = 0; i < v->l; i++) nv->d[i] = v->d[i];
    v->af->f(v);
    return nv;
}

vr_stat vr_ab(vr **v, un d) {
    vr_stat vstat = VR_STAT(OK);
    if ((*v)->l == (*v)->s) {
        *v = resize(*v);
        vstat = VR_STAT(RES);
    }
    (*v)->d[(*v)->l++] = d;
    return vstat;
}

vr_stat vr_af(vr **v, un d) {
    vr_stat vstat = VR_STAT(OK);
    if ((*v)->l + 1 >= (*v)->s) {
        *v = resize(*v);
        vstat = VR_STAT(RES);
    }
    for (size_t i = (*v)->l; i > 0; i--) (*v)->d[i] = (*v)->d[i - 1];
    (*v)->d[0] = d;
    (*v)->l++;
    return vstat;
}

vr_stat vr_sb(vr *v, un *d) {
    if (v->l == 0) return VR_STAT(SUB);
     *d = v->d[--v->l];
     return VR_STAT(OK);
}

vr_stat vr_sf(vr *v, un *d) {
    if (v->l == 0) return VR_STAT(SUB);
    *d = v->d[0];
    v->l--;
    for (size_t i = 0; i < v->l; i++) v->d[i] = v->d[i + 1];
    return VR_STAT(OK);
}

void vr_r(vr *v) {
    un d;
    for (size_t i = 0; i < v->l / 2; i++) {
        d = v->d[i];
        v->d[i] = v->d[v->l - 1 - i];
        v->d[v->l - 1 - i] = d;
    }
}

void vr_d(vr *v) {
    if (v->df) for (size_t i = 0; i < v->l; i++) v->df(v->d[i].p);
    v->l = 0;
}

void vr_f(vr *v) {
    if (!v || --v->r > 0) return;
    vr_d(v);
    v->af->f(v);
}
