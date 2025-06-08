
#pragma once

#include "def.h"

#define VR_STAT(N) VR_STAT_##N

typedef enum {
    VR_STAT(OK),
    VR_STAT(RES),
    VR_STAT(SUB)
} vr_stat;

typedef struct _vr vr;

typedef struct _vr {
    ssize_t r;
    size_t s, l;
    alfn *af;
    frfn *df, *ff;
    un d[];
} vr;

vr *vr_i(size_t s, alfn *af, frfn *df, frfn *ff);

#ifndef VR_RES
    #define VR_RES 2
#endif

vr_stat vr_a(vr **v, un d);

vr_stat vr_s(vr *const v, un *d);

void vr_f(vr *v);
