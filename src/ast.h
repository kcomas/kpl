
#pragma once

#include "kpl.h"
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

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(TYPE),
    AST_TYPE(VAL),
    AST_TYPE(VAR),
    AST_TYPE(OP),
    AST_TYPE(LST),
    AST_TYPE(TBL),
    AST_TYPE(FN)
} ast_type;

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(ANON),
    TYPE(VD)
} type;

typedef struct {
    type t;
    ast *def;
} type_node;

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

typedef struct {
    uint8_t id;
    fn_node *scope;
    ast *def;
    char str[]; // null term
} var_node; // tbl itm data TODO struct padding

#define OP_TYPE(N) OP_TYPE_##N

typedef enum {
    OP_TYPE(ASS),
    OP_TYPE(CST)
} op_type;

typedef struct {
    op_type ot;
    ast *def;
    ast *l, *r;
} op_node;

typedef struct _lst_itm {
    ast *def;
    struct _lst_itm *next;
} lst_itm;

inline lst_itm *lst_itm_i(ast *const a) {
    lst_itm *itm = calloc(1, sizeof(lst_itm));
    itm->a = a;
    return itm;
}

typedef struct _lst_node {
    size_t len;
    ast *def;
    lst_itm *h, *t;
} lst_node;

inline lst_node *lst_node_int(void) {
    return calloc(1, sizeof(lst_node));
}

inline void lst_node_add(lst_node *const lst, ast *const a) {
    if (!lst->h) lst->h = lst->h->next = lst->t = lst_itm_i(a);
    else lst->t = lst->t->next = lst_itm_i(a);
    lst->len++;
}

typedef struct {
    ast *def;
    tbl *tl;
} tbl_node;

typedef struct _fn_node {
    uint8_t idc; // var id counter
    tbl *tl; // sym tbl
    struct _fn_node *par; // parent node
    lst_node *args, *body; // tail arg is ret type only mods have NULL args
} fn_node;

typedef struct _ast {
    ast_type at;
    union {
        type_node *tn;
        val_node *val;
        var_node *var;
        op_node *op;
        lst_node *lst;
        tbl_node *tl;
        fn_node *fn;
    } node;
    tkn t;
} ast;
