
#include "./state.h"

extern inline void token_state_init(token_state *state, const string *str);

static void token_state_print_fn(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    (void) print_opts;
    fprintf(file, "%*s", idnt, "");
    const token_state *state = data.ptr;
    uint32_t str_start = state->str_pos, str_end = state->str_pos, byte_pos = 0;
    while (str_start > 0) {
        if (state->str->data[str_start] == '\n') {
            str_start++;
            break;
        }
        str_start--;
        byte_pos++;
    }
    while (str_end < state->str->len) {
        char charater = state->str->data[str_end];
        if (charater == '\0' || charater == '\n') {
            str_end--;
            break;
        }
        str_end++;
    }
    if (state->line_no > 0)
        fprintf(file, COLOR(BOLD) " L%d::B%u" COLOR(RESET), state->line_no, byte_pos);
    else
        fprintf(file, COLOR(BOLD) " EOS" COLOR(RESET));
    fprintf(file, "|" COLOR(GREEN));
    while (str_start < str_end)
        fprintf(file, "%c", state->str->data[str_start++]);
    fprintf(file, COLOR(RESET) "|");
}

def_fn_table token_state_fn_table = {
    .hash_fn = nullptr,
    .cmp_fn = nullptr,
    .eq_fn = nullptr,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = token_state_print_fn,
    .free_fn = nullptr
};

extern inline void token_state_rewind_from_slice(token_state *state, const token_slice *slice);

extern inline void token_slice_init(const token_state *state, token_slice *slice);

bool token_slice_match_c_str(const token_slice *slice, const char *c_str, uint32_t slice_str_offset) {
    uint32_t str_idx = slice->str_start + slice_str_offset;
    while (*c_str && str_idx < slice->str_end) {
        if (*c_str != slice->str->data[str_idx])
            return false;
        c_str++;
        str_idx++;
    }
    return !*c_str && str_idx == slice->str_end;
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
