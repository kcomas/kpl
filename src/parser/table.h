
#pragma once

#include "./next.h"

typedef ast_node *parser_ast_init_fn(const token_slice *slice); // nullptr error

typedef def_status parser_ast_join_fn(ast_node **current, ast_node *next);

#define PARSER_ACTION(NAME) PARSER_ACTION_##NAME

typedef enum [[gnu::packed]] {
    PARSER_ACTION(SKIP),
    PARSER_ACTION(NEXT_STMT),
    PARSER_ACTION(NEXT_LIST),
    PARSER_ACTION(END_STMT),
    PARSER_ACTION(END_LIST),
    PARSER_ACTION(STOP)
} parser_action;

typedef struct {
    parser_action action;
    token_class class;
    parser_ast_init_fn *ast_int_fn;
    parser_ast_join_fn *ast_join_fn;
} parser_table;
