
#pragma once

#include "def.h"
#include <string.h>

typedef struct {
    ssize_t r; // ref count
    size_t s, l; // size, len
    alfn *ma;
    frfn *mf;
    uint8_t *d; // data in bytes
} mc; // memory chunk

mc *mc_i(size_t s, alfn *ma, frfn *mf);

mc *mc_c(mc *m);

void mc_wb(mc **m, uint8_t b);

void mv_f(mc *m);
