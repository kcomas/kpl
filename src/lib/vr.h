
#pragma once

#include "def.h"

#define VR_STAT(N) VR_STAT_##N

typedef enum {
    VR_STAT(OK),
    VR_STAT(IDX),
    VR_STAT(RES),
    VR_STAT(SUB)
} vr_stat;

// vector
typedef struct {
    ssize_t r; // ref count
    size_t s, l; // size, len
    alfn *va; // allocate
    frfn *df, *vf; // data free, free
    un d[]; // data
} vr;

vr *vr_i(size_t s, alfn *al, frfn *df, frfn *vf);

vr *vr_c(vr *v);

size_t vr_g_s(const vr *const v);

vr_stat vr_s_i(vr *const v, size_t i, un d);

size_t vr_g_l(const vr *const v);

vr_stat vr_g_i(vr *const v, size_t i, un *d);

vr_stat vr_ab(vr **v, un d);

vr_stat vr_af(vr **v, un d);

vr_stat vr_sb(vr *const v, un *d);

vr_stat vr_sf(vr *const v, un *d);

void vr_f(vr *v);
