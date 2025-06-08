
#include "vr.h"

vr *vr_i(size_t s, alfn *va, frfn *df, frfn *vf) {
    vr *v = va(sizeof(vr) + sizeof(un) * s);
    v->r = 1;
    v->s = s;
    v->l = 0;
    v->va = va;
    v->df = df;
    v->vf = vf;
    for (size_t i = 0; i < s; i++) v->d[i] = P(NULL);
    return v;
}

vr *vr_c(vr *v) {
    v->r++;
    return v;
}

size_t vr_g_s(const vr *const v) {
    return v->s;
}

size_t vr_g_l(const vr *const v) {
    return v->l;
}

vr_stat vr_g_i(vr *const v, size_t i, un *d) {
    if (i >= v->l) return VR_STAT(IDX);
    *d = v->d[i];
    return VR_STAT(OK);
}

vr_stat vr_s_i(vr *const v, size_t i, un d) {
    if (i >= v->l) return VR_STAT(IDX);
    if (v->df) v->df(v->d[i].p);
    v->d[i] = d;
    return VR_STAT(OK);
}

static vr *resize(vr *v) {
    vr *nv = vr_i(v->s * VR_RES, v->va, v->df, v->vf);
    nv->l = v->l;
    for (size_t i = 0; i < v->l; i++) nv->d[i] = v->d[i];
    nv->vf(v);
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
    if ((*v)->l == (*v)->s) {
        *v = resize(*v);
        vstat = VR_STAT(RES);
    }
    un n;
    for (size_t i = 1; i < (*v)->l; i++) {
        n = (*v)->d[i];
        (*v)->d[i] = (*v)->d[i - 1];
    }
    (*v)->d[(*v)->l++] = n;
    (*v)->d[0] = d;
    return vstat;
}

vr_stat vr_sb(vr *const v, un *d) {
    if (v->l == 0) return VR_STAT(SUB);
     *d = v->d[--v->l];
     return VR_STAT(OK);
}

vr_stat vr_sf(vr *const v, un *d) {
    if (v->l == 0) return VR_STAT(SUB);
    *d = v->d[0];
    v->l--;
    for (size_t i = 0; i < v->l; i++) v->d[i] = v->d[i + 1];
    return VR_STAT(OK);
}

void vr_f(vr *v) {
    if (!v || --v->r > 0) return;
    if (v->df) for (size_t i = 0; i < v->l; i++) v->df(v->d[i].p);
    v->vf(v);
}
