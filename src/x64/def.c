
#include "def.h"

extern inline int8_t x64_reg_id(x64_reg reg);

extern inline bool x64_reg_is_upper(x64_reg reg);

const char *reg_strs[] = {
    "RAX",
    "RCX",
    "RDX",
    "RBX",
    "RSP",
    "RBP",
    "RDI",
    "RSI",
    "R8",
    "R9",
    "R10",
    "R11",
    "R12",
    "R13",
    "R14",
    "R15",
    "XMM0",
    "XMM1",
    "XMM2",
    "XMM3",
    "XMM4",
    "XMM5",
    "XMM6",
    "XMM7",
    "XMM8",
    "XMM9",
    "XMM10",
    "XMM11",
    "XMM12",
    "XMM13",
    "XMM14",
    "XMM15",
    "MM0",
    "MM1",
    "MM2",
    "MM3",
    "MM4",
    "MM5",
    "MM6",
    "MM7",
    "_"
};

const char *x64_reg_str(x64_reg reg) {
    if (reg < X64_REG(_))
        return reg_strs[reg];
    return "X64 INVALID REG";
}

const char *mne_strs[] = {

};

const char *x64_mne_str(x64_mne mne) {
    if (mne < X64_MNE_INST(_))
        return mne_strs[mne];
    return "X64 INVALID MNE";
}
