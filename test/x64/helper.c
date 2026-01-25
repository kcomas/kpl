
#include "./helper.h"

bool asm_check_error(x64_state *state, error *er) {
    if (!er)
        return true;
    error_print(er, stdout, 0, ERROR_PRINT(NL_END));
    error_free(er);
    x64_state_free(state);
    return false;
}
