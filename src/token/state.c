
#include "./state.h"

void token_state_init(token_state *state, const string *str) {
    state->line_no = 1;
    state->str_pos = 0;
    state->str = str;
}

void token_slice_init(const token_state *state, token_slice *slice) {
    slice->class = TOKEN_CLASS(INVALID);
    slice->line_no = state->line_no;
    slice->str_start = slice->str_end = state->str_pos;
    slice->str = state->str;
}

void token_slice_print(const token_slice *slice, FILE *file, int32_t idnt, token_slice_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    if (print_opts & TOKEN_SLICE_PRINT(CLASS))
        fprintf(file, COLOR2(BOLD, LIGHT_BLUE) "%s" COLOR(RESET), token_class_str(slice->class));
    fprintf(file, "|" COLOR(LIGHT_GREEN));
    for (uint32_t str_idx = slice->str_start; str_idx < slice->str_end; str_idx++) {
        char charater = slice->str->data[str_idx];
        switch (charater) {
            case '\n':
                fprintf(file, "\\n");
                break;
            case '\t':
                fprintf(file, "\\t");
                break;
            default:
                fprintf(file, "%c", charater);
                break;
        }
    }
    fprintf(file, COLOR(RESET) "|");
    if (print_opts & TOKEN_SLICE_PRINT(NL_END))
        fprintf(file, "\n");
}
