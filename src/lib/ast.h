
#pragma once

#include "mc.h"
#include "tbl.h"
#include "tkn.h"
#include "type.h"

#define AST_STAT(N) AST_STAT_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(INV)
} ast_stat;

#define AST_CLS(N) AST_CLS_##N

typedef enum {
    AST_CLS(R), // root te[tbl;te]
    AST_CLS(T), // type te[type]
    AST_CLS(E), // lst entry te[mc;id;type;node]
    AST_CLS(I), // identifier [mc]
    AST_CLS(S), // scalar type [te[type];un]
    AST_CLS(V), // vector type [te[type];lst]
    AST_CLS(O), // op [te[type];op_id;te;te]
    AST_CLS(Z), // left target, no arg apply type [te[type];te;mc]
    AST_CLS(A), // apply type [te[type];te;lst]
    AST_CLS(L), // list tbl with scope [tbl[mc;id;type;node];lst]
    AST_CLS(C), // cmd te[cmd;tgt]
    AST_CLS(_)
} ast_cls;

const char *ast_cls_str(ast_cls cls);

// ast entry te[id;fn]

typedef ast_stat psr_id_g(te *pn, size_t *pid);

typedef lst *ast_lst_i(void);

typedef struct {
    ssize_t r;
    const alfr *af, *ta, *ma, *ea;
    err_d_p *edp;
    psr_id_g *pig;
    ast_lst_i *ali;
    tbl *pt, *tt;
} ast;

ast *ast_i(const alfr *af, const alfr *ta, const alfr *ma, const alfr *ea, err_d_p edp, psr_id_g pig, ast_lst_i ali, tbl *pt, tbl *tt);

ast *ast_i_ast(const ast *a);

ast *ast_c(ast *a);

un ast_hsh(const te *an);

#define AST_HSH_C 1
#define AST_HSH_T 0

// get first parent node of cls
ast_stat ast_g_pn(ast_cls cls, te *an, te **pn);

// get type, not null
ast_stat ast_g_t(te *an, te **type);

// ast t te[par;psr;cls;...]

#define AST_MIN_LEN 4

te *ast_an_i(ast *a, te *restrict pan, te *restrict psr, ast_cls cls, un ct, ...);

typedef ast_stat ast_tf(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e);

// do not allow dupes
ast_stat ast_a(ast *a, size_t id, ast_tf atf);

ast_stat ast_n(ast *a, te *restrict pan, te *restrict pn, void **vn, err **e);

ast_stat ast_t_a(ast *a, uint16_t tid, uint16_t id);

ast_stat ast_t_n(ast *a, te *t, uint16_t *id);

void ast_f(ast *a);
