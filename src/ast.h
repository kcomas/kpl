
#pragma once

#include "kpl.h"
#include "tkn.h"

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(VAL),
    AST_TYPE(VAR),
    AST_NODE(OP),
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

} op_type;

typedef struct {
    op_type ot;
    ast *l, *r;
} op_node;

typedef struct _lst_itm {
    ast *n;
    struct _lst_itm *next;
} lst_itm;

typedef struct {
    size_t len;
    lst_item *h, *t;
} lst_node;

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
