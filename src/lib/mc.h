
#pragma once

#include "def.h"
#include <string.h>

typedef struct {
    ssize_t r; // ref count
    size_t s, l; // size, len
    const alfr *af;
    uint8_t d[]; // data in bytes
} mc; // memory chunk

mc *mc_i(size_t s, const alfr *af);

mc *mc_i_cstr(const char *s, const alfr *af);

mc *mc_c(mc *m);

bool mc_eq(const mc *restrict a, const mc *restrict b);

void mc_wa(mc **m, uint8_t b);

void mc_wb(mc **m, size_t l, uint8_t *b);

void mc_f(mc *m);
