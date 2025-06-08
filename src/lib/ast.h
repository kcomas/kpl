
#pragma once

#include <stdarg.h>
#include "tbl.h"

#define AST_CLS(N) AST_CLS_##N

typedef enum {
    AST_CLS(S), // scalar [un]
    AST_CLS(V), // vector [lst]
    AST_CLS(O), // op [te;te]
    AST_CLS(T), // target, no arg apply [te]
    AST_CLS(A), // apply [te;lst]
    AST_CLS(L) // list [lst]
} ast_cls;

// node te[parent;psr;cls;un[type];...]

te *ast_node_i(const alfr *ta, te *restrict parent, te *restrict psr, ast_cls cls, un type, ...);

typedef struct {
    ssize_t r;
    alfr *af, *te;
    tbl *at;
} ast;
