
#include "vr.h"

vr *vr_i(size_t s, const alfr *af, frfn *df) {
    vr *v = af->a(sizeof(vr) + sizeof(un) * s);
    v->r = 1;
    v->s = s;
    v->l = 0;
    v->af = af;
    v->df = df;
    for (size_t i = 0; i < s; i++) v->d[i] = P(NULL);
    return v;
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
    nv->af->f(v);
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
