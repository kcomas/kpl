
#pragma once

#include "./node_type.h"

typedef union {
    list *stmts;
    tuple *op;
} ast_node_children;

typedef struct _ast_node {
    MEM_HEADER(_ast_node);
    type *ty;
    struct _ast_node *parent; // weak ref
    ast_node_children children;
    void *ir; // TODO keep space for pointer
} ast_node;

inline ast_node_children ast_node_children_empty(void) {
    return (ast_node_children) { .stmts = nullptr };
}

inline ast_node_children ast_node_children_stmts(list *stmts) {
    return (ast_node_children) { .stmts = stmts };
}

inline ast_node_children ast_node_children_op(tuple *op) {
    return (ast_node_children) { .op = op };
}

#define AST_NODE_OP_SIDE(NAME) AST_NODE_OP_SIDE_##NAME

typedef enum [[gnu::packed]] {
    AST_NODE_OP_SIDE(LEFT)  = 0,
    AST_NODE_OP_SIDE(RIGHT) = 1
} ast_node_op_side;

#define AST_NODE_OP_SIZE 2

ast_node *ast_node_init(type *ty, ast_node *parent, ast_node_children children);

void ast_node_free(ast_node *node);

#define AST_NODE_PRINT(NAME) AST_NAME_PRINT_##NAME

typedef enum [[gnu::packed]] {
    AST_NODE_PRINT(NL_END)  = 1 << 0,
    AST_NODE_PRINT(_)       = 0
} ast_node_print_opts;

void ast_node_print(const ast_node *node, FILE *file, int32_t idnt, ast_node_print_opts print_opts);

extern def_fn_table ast_node_fn_table;

def_status ast_node_op_set_side(ast_node *restrict op_node, ast_node_op_side side, ast_node *restrict child);
