
#pragma once

#include "kpl.h"
#include "tkn.h"

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(VAL),
    AST_TYPE(VAR),
    AST_TYPE(OP),
    AST_TYPE(LST),
    AST_TYPE(FN)
} ast_type;

#define TYPE(N) TYPE_##N

typedef enum {
    TYPE(ANON),
    TYPE(VD)
} type;

typedef struct {
    type t;
    ast *data;
} val_node;

typedef struct {
    uint8_t id;
    val_node *val;
    char str[]; // null term
} var_node;

#define OP_TYPE(N) OP_TYPE_##N

typedef enum {
    OP_TYPE(ASS),
    OP_TYPE(CST)
} op_type;

typedef struct {
    op_type ot;
    ast *l, *r;
} op_node;

typedef struct _lst_itm {
    ast *a;
    struct _lst_itm *next;
} lst_itm;

inline lst_itm *lst_itm_init(ast *const a) {
    lst_itm *itm = calloc(1, sizeof(lst_itm));
    itm->a = a;
    return itm;
}

typedef struct {
    size_t len;
    lst_itm *h, *t;
} lst_node;

inline lst_node *lst_node_int(void) {
    return calloc(1, sizeof(lst_node));
}

inline void lst_node_add(lst_node *const lst, ast *const a) {
    if (lst->h == NULL) lst->h = lst->h->next = lst->t = lst_itm_init(a);
    else {
        lst->t->next = lst_itm_init(a);
        lst->t = lst->t->next;
    }
    lst->len++;
}

typedef struct {
    ast *ret;
    lst_node *args, *body;
} fn_node;

typedef struct _ast {
    ast_type at;
    union {
        val_node *val;
        var_node *var;
        op_node *op;
        lst_node *lst;
        fn_node *fn;
    } node;
    tkn t;
} ast;

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

inline void ast_st_init(ast_st *const at, char *const str) {
    tkn_st_init(&at->ts);
    at->str = str;
}

ast_stat _ast_tkn_get(ast_st *const at, bool inc);

inline ast_stat ast_tkn_next(ast_st *const at) {
    return _ast_tkn_get(at, true);
}

inline ast_stat ast_tkn_peek(ast_st *const at) {
    return _ast_tkn_get(at, false);
}
