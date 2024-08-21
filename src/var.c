
#include "var.h"

#define VAR_OP_T(N, OP, T, CT) VAR_FN_T(N, T, CT) { \
    return l OP r; \
}

// add
VAR_OP_T(add, +, i6, int64_t)
VAR_OP_T(add, +, u6, uint64_t)

// sub
VAR_OP_T(sub, -, i6, int64_t)
VAR_OP_T(sub, -, u6, uint64_t)
