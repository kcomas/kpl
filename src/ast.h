
#pragma once

#include "kpl.h"
#include "tbl.h"
#include "tkn.h"

#define AST_STAT(N) ast_stat_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(TKN_ERR),
    AST_STAT(END)
} ast_stat;

typedef struct {
    tkn_stat tstat;
    tkn_st ts;
    tkn next, peek;
    char *str;
} ast_st;

inline void ast_st_i(ast_st *const at, char *const str) {
    tkn_st_i(&at->ts);
    at->str = str;
}

#define TKN_IGN(N) TKN_IGN_##N

typedef enum {
    TKN_IGN(NL) = (1 << 0),
    TKN_IGN(WS) = (1 << 1),
    TKN_IGN(CMT) = (1 << 2)
} tkn_ign;

ast_stat _ast_tkn_get(ast_st *const at, bool inc, uint8_t ign_flgs);

inline ast_stat ast_tkn_next(ast_st *const at, uint8_t ign_flgs) {
    return _ast_tkn_get(at, true, ign_flgs);
}

inline ast_stat ast_tkn_peek(ast_st *const at, uint8_t ign_flgs) {
    return _ast_tkn_get(at, false, ign_flgs);
}

typedef struct _ast ast;

void ast_f(ast *a);

#define TYPE(N) TYPE_##N

typedef enum {
    // ast types
    TYPE(INT),
    TYPE(FLT),
    TYPE(STR),
    // data types
    TYPE(VD)
} type;

typedef struct {
    type t;
    ast *a;
} type_node;

inline type_node *type_node_i(type t, ast *const a) {
    type_node *tn = calloc(1, sizeof(type_node));
    tn->t = t;
    tn->a = a;
    return tn;
}

inline void type_node_f(type_node *tn) {
    FNNF(tn->a, ast_f);
    free(tn);
}

typedef struct {
    type t;
} val_node; // val is tkn str

inline val_node *val_node_i(type t) {
    val_node *v = calloc(1, sizeof(val_node));
    v->t = t;
    return v;
}

inline void val_node_f(val_node *v) {
    free(v);
}

#define OP_TYPE(N) OP_TYPE_##N

typedef enum {
    OP_TYPE(ASS), // :
    OP_TYPE(CST) // $
} op_type;

typedef struct {
    op_type ot;
    type_node *tn;
    ast *l, *r;
} op_node;

typedef struct _lst_itm {
    ast *a;
    struct _lst_itm *next;
} lst_itm;

inline lst_itm *lst_itm_i(ast *const a) {
    lst_itm *itm = calloc(1, sizeof(lst_itm));
    itm->a = a;
    return itm;
}

typedef struct _lst_node {
    size_t len;
    type_node *tn;
    lst_itm *h, *t;
} lst_node;

inline lst_node *lst_node_i(void) {
    return calloc(1, sizeof(lst_node));
}

inline void lst_node_a(lst_node *const lst, ast *const a) {
    if (!lst->h) lst->h = lst->h->next = lst->t = lst_itm_i(a);
    else lst->t = lst->t->next = lst_itm_i(a);
    lst->len++;
}

typedef struct {
    type_node *tn;
    tbl *tl;
} tbl_node;

typedef struct _fn_node {
    uint8_t idc; // var id counter
    tbl *tl; // sym tbl
    struct _fn_node *par; // parent node
    lst_node *args, *body; // tail arg is ret type only mods have NULL args
} fn_node;

typedef struct {
    uint8_t id;
    type_node *tn; // null for unknown
    fn_node *fns; // scope
    char str[]; // null term
} var_node; // tbl itm data TODO struct padding

inline var_node *var_node_i(fn_node *const fns, const tkn *const t, const char *const str) {
    var_node *vn = calloc(1, sizeof(var_node) + t->len + 1);
    vn->id = fns->idc++;
    vn->fns = fns;
    memcpy(vn->str, str + t->pos, t->len);
    return vn;
}

inline void var_node_f(var_node *vn) {
    FNNF(vn->tn, type_node_f);
    free(vn);
}

typedef struct {
    ast *tgt;
    type_node *tn;
    lst_node *args;
} call_node;

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(TYPE),
    AST_TYPE(VAL),
    AST_TYPE(OP),
    AST_TYPE(LST),
    AST_TYPE(TBL),
    AST_TYPE(FN),
    AST_TYPE(VAR),
    AST_TYPE(CALL)
} ast_type;

typedef union {
    type_node *tn;
    val_node *val;
    op_node *op;
    lst_node *lst;
    tbl_node *tl;
    fn_node *fn;
    var_node *var;
    call_node *cl;
} node;

typedef struct _ast {
    ast_type at;
    node *n;
    tkn t;
} ast;

inline ast *ast_i(ast_type at, node *const n, tkn *t) {
    ast *a = calloc(1, sizeof(ast));
    a->at = at;
    a->n = n;
    memcpy(&a->t, t, sizeof(tkn));
    return a;
}

ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, ast **cur);
