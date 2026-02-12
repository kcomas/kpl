
#pragma once

#include "../ast/ast.h"
#include "./class.h"

typedef struct {
    uint16_t line_no;
    uint32_t str_pos;
    const string *str; // weak ref
} token_state;

void token_state_init(token_state *state, const string *str);

typedef struct {
    token_class class;
    uint16_t line_no;
    uint32_t str_start, str_end;
    const string *str; // weak ref
} token_slice;

void token_slice_init(const token_state *state, token_slice *slice);

bool token_slice_match_c_str(const token_slice *slice, const char *c_str, uint32_t slice_str_offset);

#define TOKEN_SLICE_PRINT(NAME) TOKEN_SLICE_PRINT_##NAME

typedef enum [[gnu::packed]] {
    TOKEN_SLICE_PRINT(NL_END)   = 1 << 0,
    TOKEN_SLICE_PRINT(CLASS)    = 1 << 1,
    TOKEN_SLICE_PRINT(_)        = 0
} token_slice_print_opts;

void token_slice_print(const token_slice *slice, FILE *file, int32_t idnt, token_slice_print_opts print_opts);
