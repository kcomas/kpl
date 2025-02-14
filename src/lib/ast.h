
#pragma once

#include <stdarg.h>
#include "tbl.h"

#define AST_STAT(N) AST_STAT_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(INV)
} ast_stat;

#define AST_CLS(N) AST_CLS_##N

typedef enum {
    AST_CLS(R), // root te[tbl;te]
    AST_CLS(I), // identifier [te[type];te]
    AST_CLS(S), // scalar type [un[type];un]
    AST_CLS(V), // vector type [te[type];lst]
    AST_CLS(O), // op [te[type];te;te;op_id]
    AST_CLS(T), // target, no arg apply type [te[type];te;te]
    AST_CLS(A), // apply type [te[type];te;lst]
    AST_CLS(L) // list tbl with scope [tbl;lst]
} ast_cls;

const char *ast_cls_str(ast_cls cls);

// ast entry te[id;fn]

typedef ssize_t psr_id_g(te *pn); // -1 for invalid

typedef tbl *ast_tbl_i(void);

typedef lst *ast_lst_i(void);

typedef struct {
    ssize_t r;
    const alfr *af, *na;
    psr_id_g *pig;
    ast_tbl_i *ati;
    ast_lst_i *ali;
    tbl *t;
} ast;

ast *ast_i(const alfr *af, const alfr *na, psr_id_g pig, ast_tbl_i ati, ast_lst_i ali, tbl *t);

// ast t te[par;psr;cls;...]

te *ast_an_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un tt, ...);

typedef ast_stat ast_tf(ast *a, te *pn, void **vn);

// do not allow dupes
ast_stat ast_a(ast *a, size_t id, ast_tf atf);

ast_stat ast_n(ast *a, te *pn, void **vn);

void ast_f(ast *a);
