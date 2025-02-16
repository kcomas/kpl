
#pragma once

#include "./lib/ast.h"
#include "type.h"
#include "psr.h"

#define OC(N) OC_##N

typedef enum {
    OC(_START),
    OC(ADD),
    OC(SUB),
    OC(_END)
} oc; // op code

const char *ast_op_str(oc o);

ast *ast_b(ast *a);

void ast_p(const te *an, size_t idnt);

bool ast_eq(const te *restrict a, const te *restrict b);
