
#pragma once

#include "./state.h"

#define TOKEN_TABLE_ITEM_MATCH_SIZE 6

typedef struct _token_table_match token_table_match;

typedef def_status token_table_match_fn(const token_table_match *match, token_slice *slice);

typedef struct _token_table_match {
    token_class class; // TOKEN_CLASS(_) for no more matches
    const char str[TOKEN_TABLE_ITEM_MATCH_SIZE];
    token_table_match_fn *fn;
} token_table_match;

def_status token_table_match_next(token_slice *slice);
