
#pragma once

#include "./join.h"

#define PARSER_MODE(NAME) PARSER_MODE_##NAME

typedef enum [[gnu::packed]] {
    PARSER_MODE(INVALID),
    PARSER_MODE(SKIP),
    PARSER_MODE(NEXT_STMT),
    PARSER_MODE(NEXT_LAMBDA),
    PARSER_MODE(NEXT_APPLY),
    PARSER_MODE(NEXT_DEFINE),
    PARSER_MODE(END_STMT),
    PARSER_MODE(END_LAMBDA),
    PARSER_MODE(END_APPLY),
    PARSER_MODE(END_DEFINE),
    PARSER_MODE(END)
} parser_mode;

error *parser_next_list(ast_container *cont, ast_node *list_node, type_table *scope, parser_mode stop_mode);

typedef ast_node *parser_ast_node_init_fn(const token_slice *slice); // nullptr error

typedef def_status parser_ast_node_join_fn(ast_node **current_node, ast_node *next_node);

typedef struct {
    parser_mode mode;
    parser_ast_node_init_fn *ast_node_int_fn;
    parser_ast_node_join_fn *ast_node_join_fn;
} parser_table_action;

const parser_table_action *parser_table_action_next(token_class class);
