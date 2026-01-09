
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
