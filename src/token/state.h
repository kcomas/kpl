
#pragma once

#include "../namespace/namespace.h"
#include "./class.h"

typedef struct {
    int16_t line_no; // -1 done
    uint32_t str_pos;
    const string *str; // weak ref
} token_state;

inline void token_state_init(token_state *state, const string *str) {
    state->line_no = 1;
    state->str_pos = 0;
    state->str = str;
}

extern def_fn_table token_state_fn_table;

typedef struct {
    token_class class;
    uint16_t line_no;
    uint32_t str_start, str_end;
    const string *str; // weak ref
} token_slice;

inline void token_slice_init(const token_state *state, token_slice *slice) {
    slice->class = TOKEN_CLASS(INVALID);
    slice->line_no = state->line_no;
    slice->str_start = slice->str_end = state->str_pos;
    slice->str = state->str;
}

bool token_slice_match_c_str(const token_slice *slice, const char *c_str, uint32_t slice_str_offset);

#define TOKEN_SLICE_PRINT(NAME) TOKEN_SLICE_PRINT_##NAME

typedef enum [[gnu::packed]] {
    TOKEN_SLICE_PRINT(NL_END)   = 1 << 0,
    TOKEN_SLICE_PRINT(CLASS)    = 1 << 1,
    TOKEN_SLICE_PRINT(_)        = 0
} token_slice_print_opts;

void token_slice_print(const token_slice *slice, FILE *file, int32_t idnt, token_slice_print_opts print_opts);
