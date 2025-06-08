
#pragma once

#include <stdarg.h>
#include "tbl.h"

#define AST_CLS(N) AST_CLS_##N

typedef enum {
    AST_CLS(S), // scalar type [un]
    AST_CLS(V), // vector type [lst]
    AST_CLS(O), // op [te;te]
    AST_CLS(T), // target, no arg apply type [te]
    AST_CLS(A), // apply type [te;lst]
    AST_CLS(L) // list no type [lst]
} ast_cls;

// ast entry te[id;fn]

typedef tbl *asttbl_i(void);

typedef struct {
    ssize_t r;
    const alfr *af, *na;
    tbl *t;
} ast;

ast *ast_i(const alfr *af, const alfr *na, tbl *t);

// ast t te[parent;psr;cls;un[type];...]

te *ast_t_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un type, ...);

void ast_f(ast *a);
