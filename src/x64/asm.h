
#pragma once

#include "./query.h"

error *x64_asm_label(x64_state *state, ssize_t label, type_table *fn);

error *_x64_asm(x64_state *state, x64_pfx_flag pfx, x64_mne mne, va_list args);

inline error *x64_asm(x64_state *state, x64_mne mne, ...) { // X64_END term
    va_list args;
    va_start(args, mne);
    return _x64_asm(state, X64_PFX_FLAG(_), mne, args);
}

inline error *x64_asm_pfx(x64_state *state, x64_pfx_flag pfx, x64_mne mne, ...) { // X64_END term
    va_list args;
    va_start(args, mne);
    return _x64_asm(state, pfx, mne, args);
}

error *x64_asm_text_end(x64_state *state);  // DISASSEMBLER Flag

error *x64_asm_data(x64_state *state, ssize_t label, size_t data_size, def_data data);

error *x64_asm_c_str(x64_state *state, ssize_t label, const char *c_str);

error *x64_asm_buffer(x64_state *state, ssize_t label, ser_buffer *buffer);

error *x64_asm_data_end(x64_state *state);
