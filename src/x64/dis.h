
#pragma once

#include "./asm.h"

error *x64_dis_start(x64_state *state, int32_t byte_idx);

error *x64_dis_next(x64_state *state, x64_op *op);

#define X64_DIS_PRINT(NAME) X64_DIS_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_DIS_PRINT(NL_END)   = 1 << 0,
    X64_DIS_PRINT(_)        = 0
} x64_dis_print_opts;

void x64_dis_print(int32_t byte_idx, FILE *file, int32_t idnt, x64_dis_print_opts print_opts);
