
#pragma once

#include "./node_type.h"

typedef struct _ast_node ast_node;

typedef struct {
    type_base *base;
    ast_node *root;
    token_state state;
} ast_container;

void ast_container_init(ast_container *cont, const string *str, type_base *base, ast_node *wrapper);

typedef union {
    list *stmts;
    tuple *op;
    ast_container *cont; // weak ref
} ast_node_children;

typedef struct {
    uint32_t str_start;
    uint16_t str_len, line_no;
} ast_position;

inline ast_position ast_position_init(uint32_t str_start, int16_t str_len, int16_t line_no) {
    return (ast_position) { .str_start = str_start, .str_len = str_len, .line_no = line_no };
}

typedef struct _ast_node {
    MEM_HEADER(_ast_node);
    type *ty;
    struct _ast_node *parent, *left; // weak ref, owned
    ast_node_children children;
    void *ir; // TODO
    ast_position position;
} ast_node;

inline void ast_container_set_root_parent(ast_container* cont, ast_node *wrapper) {
    cont->root->parent = wrapper;
}

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

ast_node *ast_node_init(type *ty, ast_node *parent, ast_node_children children, ast_position pos);

inline ast_node *ast_node_init_slice(type *ty, ast_node *parent, const token_slice *slice) {
    ast_position pos = ast_position_init(slice->str_start, slice->str_end - slice->str_start, slice->line_no);
    return ast_node_init(ty, parent, ast_node_children_empty(), pos);
}

void ast_node_free(ast_node *node);

ast_container *ast_node_get_container(const ast_node *node);

#define AST_NODE_PRINT(NAME) AST_NAME_PRINT_##NAME

typedef enum [[gnu::packed]] {
    AST_NODE_PRINT(NL_END)      = 1 << 0,
    AST_NODE_PRINT(NL_LEFT)     = 1 << 1,
    AST_NODE_PRINT(STRING)      = 1 << 2,
    AST_NODE_PRINT(POSITION)    = 1 << 3,
    AST_NODE_PRINT(_)           = 0
} ast_node_print_opts;

void ast_node_print(const ast_node *node, FILE *file, int32_t idnt, ast_node_print_opts print_opts);

extern def_fn_table ast_node_fn_table;

def_status ast_node_op_set_side(ast_node *restrict op_node, ast_node_op_side side, ast_node *restrict child);

def_status ast_node_list_add_back(ast_node *restrict list_node, ast_node *restrict child);

def_status ast_node_left_set(ast_node *restrict parent, ast_node *restrict child);
