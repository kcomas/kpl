
#include "vr.h"

vr *vr_i(size_t s, alfn *af, frfn *df, frfn *ff) {
    vr *v = af(sizeof(vr) + s * sizeof(void*));
    v->r = 1;
    v->s = s;
    v->l = 0;
    v->af = af;
    v->df = df;
    v->ff = ff;
    return v;
}

vr_stat vr_a(vr **v, var d) {
    vr_stat vstat = VR_STAT(OK);
    if ((*v)->l == (*v)->s) {
        vr *nv = vr_i((*v)->s * VR_RES, (*v)->af, (*v)->df, (*v)->ff);
        nv->l = (*v)->l;
        for (size_t i = 0; i < (*v)->l; i++) nv->d[i] = (*v)->d[i];
        nv->ff(*v);
        (*v) = nv;
        vstat = VR_STAT(RES);
    }
    (*v)->d[(*v)->l++] = d;
    return vstat;
}

vr_stat vr_s(vr *const v, var *d) {
    if (v->l == 0) return VR_STAT(SUB);
     *d = v->d[--v->l];
     return VR_STAT(OK);
}

void vr_f(vr *v) {
    if (v->df) for (size_t i = 0; i < v->l; i++) v->df(v->d[i].p);
    v->ff(v);
}
