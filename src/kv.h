
#pragma once

#include "def.h"

// pair
typedef struct {
    ssize_t r;
    alfn *a; // allocate
    frfm *kf, *vf, *f, // key free, value free, free
    un k, v; // key, value
} kv;
