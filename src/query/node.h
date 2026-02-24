
#pragma once

#include "../ast/ast.h"

#define QUERY_NODE_INNER_SIZE 3

typedef error *query_match_fn(ast_node **node, void *any);

typedef struct _query_node {
    MEM_HEADER(_query_node);
    uint32_t type_name_mask; // 0 for any
    struct _query_node *inner[QUERY_NODE_INNER_SIZE];
    query_match_fn *match_fn;
    uint8_t type_op_name_mask[TYPE_OP_MASK_BYTE_SIZE];
    type_class class; // TYPE_CLASS(_) for any
} query_node;

// nullptr error, ... query_node *inner with nullptr term
query_node *query_node_init(query_match_fn match_fn, type_class class, uint32_t type_name_mask, ...);

void query_node_free(query_node *node);

#define QUERY_NODE_PRINT(NAME) QUERY_NODE_PRINT_##NAME

typedef enum [[gnu::packed]] {
    QUERY_NODE_PRINT(NL_END)    = 1 << 0,
    QUERY_NODE_PRINT(_)         = 0
} query_node_print_opts;

void query_node_print(const query_node *node, FILE *file, int32_t idnt, query_node_print_opts print_opts);

extern def_fn_table query_node_fn_table;
