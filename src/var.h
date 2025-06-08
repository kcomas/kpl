
#pragma once

#include "kpl.h"

typedef union {
    int64_t i6;
    uint64_t u6;
} var;

#define VAR_FN_T(N, T, CT) CT var_##N##_##T(CT l, CT r)

// add
VAR_FN_T(add, i6, int64_t);
VAR_FN_T(add, u6, uint64_t);

// sub
VAR_FN_T(sub, i6, int64_t);
VAR_FN_T(sub, u6, uint64_t);
