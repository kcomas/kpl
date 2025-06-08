
#pragma once

#include "def.h"

// pair
typedef struct {
    ssize_t r;
    alfn *a; // allocate
    frfn *kf, *vf, *f, // key free, value free, free
    un k, v; // key, value
} kv;

kv *kv_i(alfn *a, frfm *kf, frfn *vf, frfn *f);

void kv_f(kv *p);
