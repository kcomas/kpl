
#pragma once

#include "./init.h"

def_status parser_ast_node_join_value(ast_node **current_node, ast_node *next_node);

def_status parser_ast_node_join_op(ast_node **current_node, ast_node *next_node);
