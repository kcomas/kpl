
#pragma once

#include "../token/token.h"

#define AST_NODE_TYPE(NAME) AST_NODE_TYPE_##NAME

typedef enum [[gnu::packed]] {
    AST_NODE_TYPE(ATOM),
    AST_NODE_TYPE(LIST),
    AST_NODE_TYPE(OP),
    AST_NODE_TYPE(LEFT),
    AST_NODE_TYPE(_)
} ast_node_type;

const char *ast_node_type_str(ast_node_type node_type);

ast_node_type ast_node_get_ast_type(const type *ty);
