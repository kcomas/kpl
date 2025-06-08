
#pragma once

#include <stdarg.h>
#include "mc.h"
#include "tbl.h"

#define AST_STAT(N) AST_STAT_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(INV)
} ast_stat;

#define AST_CLS(N) AST_CLS_##N

typedef enum {
    AST_CLS(R), // root te[tbl;te]
    AST_CLS(T), // type te[type]
    AST_CLS(I), // identifier [te[type];entry;mc]
    AST_CLS(S), // scalar type [un[type];un]
    AST_CLS(V), // vector type [te[type];lst]
    AST_CLS(O), // op [te[type];op_id;te;te]
    AST_CLS(Z), // left target, no arg apply type [te[type];mc;te]
    AST_CLS(A), // apply type [te[type];te;lst]
    AST_CLS(L), // list tbl with scope [tbl;lst]
    AST_CLS(_)
} ast_cls;

// ast entry te[id;fn]

typedef ast_stat psr_id_g(te *pn, size_t *pid);

typedef lst *ast_lst_i(void);

typedef struct {
    ssize_t r;
    const alfr *af, *ta, *ma;
    psr_id_g *pig;
    ast_lst_i *ali;
    tbl *pt, *tt;
} ast;

ast *ast_i(const alfr *af, const alfr *ta, const alfr *ma, psr_id_g pig, ast_lst_i ali, tbl *pt, tbl *tt);

// ast t te[par;psr;cls;...]

te *ast_an_i(ast *a, te *restrict parent, te *restrict psr, ast_cls cls, un ct, ...);

typedef ast_stat ast_tf(ast *a, te *pn, void **vn, te **e);

// do not allow dupes
ast_stat ast_a(ast *a, size_t id, ast_tf atf);

ast_stat ast_n(ast *a, te *pn, void **vn, te **e);

ast_stat ast_t_a(ast *a, size_t tid, size_t id);

ast_stat ast_t_n(ast *a, size_t tid, size_t *id);

void ast_f(ast *a);
