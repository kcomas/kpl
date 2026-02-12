
#pragma once

#include "./type.h"

typedef struct {
    const char *c_str;
    type_op_name op_name; // TYPE_OP_NAME(_) for end matches
} parser_command_match;

type *parser_command(const token_slice *slice); // nullptr for error
