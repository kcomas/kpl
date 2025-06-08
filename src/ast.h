
#pragma once

#include "./lib/ast.h"
#include "type.h"
#include "psr.h"

#define OP(N) OP_##N

typedef enum {
    OP(_START),
    OP(ADD),
    OP(SUB),
    OP(_END)
} op;

ast *ast_b(ast *a);

void ast_p(const te *an);
