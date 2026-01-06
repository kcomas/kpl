
#pragma once

#include "./def.h"

#define X64_OP(NAME) X64_OP_##NAME

#define X64_OP_REG(NAME) X64_OP_REG_##NAME

typedef enum [[gnu::packed]] {
    X64_OP(R8)      = 1 << 0,
    X64_OP(R16)     = 1 << 1,
    X64_OP(R32)     = 1 << 2,
    X64_OP(R64)     = 1 << 3,
    X64_OP(XMM)     = 1 << 4,
    X64_OP(MM)      = 1 << 5,
    X64_OP(M8)      = 1 << 6,
    X64_OP(M16)     = 1 << 7,
    X64_OP(M32)     = 1 << 8,
    X64_OP(M64)     = 1 << 9,
    X64_OP(M128)    = 1 << 10,
    X64_OP(DSP)     = 1 << 11,
    X64_OP(I8)      = 1 << 12,
    X64_OP(I16)     = 1 << 13,
    X64_OP(I32)     = 1 << 14,
    X64_OP(I64)     = 1 << 15,
    X64_OP(SCALE1)  = 1 << 16,
    X64_OP(SCALE2)  = 1 << 17,
    X64_OP(SCALE4)  = 1 << 18,
    X64_OP(SCALE8)  = 1 << 19,
    X64_OP(REL8)    = 1 << 20,
    X64_OP(REL32)   = 1 << 21,
    X64_OP(LABEL)   = 1 << 22,
    X64_OP_REG(0)   = 1 << 23,
    X64_OP_REG(1)   = 1 << 24,
    X64_OP_REG(2)   = 1 << 25,
    X64_OP_REG(3)   = 1 << 26,
    X64_OP_REG(4)   = 1 << 27,
    X64_OP_REG(5)   = 1 << 28,
    X64_OP_REG(6)   = 1 << 29,
    X64_OP_REG(7)   = 1 << 30,
    X64_OP_REG(_)   = 0
} x64_op_reg;

#define X64_PFX(NAME) X64_PFX_##NAME

#define X64_FLAG(NAME) X64_FLAG_##NAME

#define X64_PFX_FLAG(NAME) X64_PFX_FLAG_##NAME

typedef enum [[gnu::packed]] {
    X64_PFX(LOCK)       = 1 << 0,
    X64_PFX(REP)        = 1 << 1,
    X64_PFX(REPNZ)      = 1 << 1,
    X64_PFX(REPNE)      = 1 << 1,
    X64_PFX(REPZ)       = 1 << 2,
    X64_PFX(REPE)       = 1 << 2,
    X64_FLAG(PREFIX)    = 1 << 3,
    X64_FLAG(REX)       = 1 << 4,
    X64_FLAG(0F)        = 1 << 5,
    X64_FLAG(PLUSR)     = 1 << 6,
    X64_FLAG(OPCODE)    = 1 << 7,
    X64_FLAG(MODRRM)    = 1 << 8,
    X64_FLAG(INVALID)   = 1 << 9,
    X64_FLAG(END)       = 1 << 10,
    X64_PFX_FLAG(_)     = 0
} x64_pfx_flag;

#define X64_OP_SIZE 4

typedef struct {
    const uint8_t pf, po, so, o;
    const x64_pfx_flag flags;
    const x64_mne mne;
    const x64_op_reg op[X64_OP_SIZE];
} x64_inst;

extern const x64_inst x64_inst_table[];
