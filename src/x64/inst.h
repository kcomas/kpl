
#pragma once

#include "./def.h"

#define X64_OP_SIZE 4

typedef struct {
    const uint8_t pf, po, so, o;
    const x64_pfx_flag flags;
    const x64_mne mne;
    const x64_op_reg op[X64_OP_SIZE];
} x64_inst;

extern const x64_inst x64_inst_table[];

#define X64_INST_PRINT(NAME) X64_INST_PRINT_##NAME

typedef enum [[gnu::packed]] {
    X64_INST_PRINT(NL_END)  = 1 << 0,
    X64_INST_PRINT(_)       = 0
} x64_inst_print_opts;

void x64_inst_print(const x64_inst *inst, FILE *file, int32_t idnt, x64_inst_print_opts print_opts);
