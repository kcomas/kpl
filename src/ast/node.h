
#pragma once

#include "../namespace/namespace.h"

typedef union {
    list *stmts;
    tuple *op;
} ast_node_children;

typedef struct _ast_node {
    MEM_HEADER(_ast_node);
    type *ty;
    struct _ast_node *parent;
    ast_node_children *childrem;
    void *ir_node; // TODO keep space for pointer
} ast_node;

ast_node *ast_node_init(type *ty, ast_node *parent,  ast_node_children *children);
