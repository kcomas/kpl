
#include "./asm.h"

error *x64_asm_label(x64_state *state, ssize_t label, type_table *fn) {
    if (state->next_label != -1)
        return ERROR_INIT(0, &def_unused_fn_table, DEF(_), "X64 LABEL ALL READY SET");
   state->next_label = label;
   state->next_fn = fn;
   return nullptr;
}

error *_x64_asm(x64_state *state, x64_pfx_flag pfx, x64_mne mne, va_list args) {
    (void) state;
    (void) pfx;
    (void) mne;
    va_end(args);
    return nullptr;
}

extern inline error *x64_asm(x64_state *state, x64_mne mne, ...);

extern inline error *x64_asm_pfx(x64_state *state, x64_pfx_flag pfx, x64_mne mne, ...);
