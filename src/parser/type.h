
#pragma once

#include "../token/token.h"

typedef struct {
    const char *c_str;
    type_name name; // TYPE_NAME(_) for end matches
} parser_type_match;

type *parser_type(const token_slice *slice); // nullptr for error
