
#pragma once

#include "./inst.h"

#define X64_MAX_OPCODE_COUNT 26

extern const int16_t *x64_mne_table[];

#define X64_0F_OPCODE_INDEX(PO) (PO + (1 << 8))

extern const int16_t *x64_opcode_table[];
