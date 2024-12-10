
#include "kv.h"

kv *kv_i(alfn *a, frfm *kf, frfn *vf, frfn *f) {
    kv *p = a(sizeof(kv));
    p->r = 1;
    p->kf = kf;
    p->vf = vf;
    p->f = f;
    return p;
}

void kv_f(kv *p) {
    if (--p->r > 0) return;
    if (p->kf) p->kf(p->k);
    if (p->vf) p->vf(p->v);
    p->f(p);
}
