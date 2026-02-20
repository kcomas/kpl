
#pragma once

#include "./op.h"

ast_node *parser_ast_node_init_type(const token_slice *slice);

ast_node *parser_ast_node_init_var(const token_slice *slice);

ast_node *parser_ast_node_init_var_unused(const token_slice *slice);

ast_node *parser_ast_node_init_base(const token_slice *slice);

ast_node *parser_ast_node_init_true_value(const token_slice *slice);

ast_node *parser_ast_node_init_true_const(const token_slice *slice);

ast_node *parser_ast_node_init_false_value(const token_slice *slice);

ast_node *parser_ast_node_init_false_const(const token_slice *slice);

ast_node *parser_ast_node_init_integer(const token_slice *slice);

ast_node *parser_ast_node_init_float(const token_slice *slice);

ast_node *parser_ast_node_init_string(const token_slice *slice);

ast_node *parser_ast_node_init_symbol(const token_slice *slice);

ast_node *parser_ast_node_init_symbol_unused(const token_slice *slice);

ast_node *parser_ast_node_init_command(const token_slice *slice);

ast_node *parser_ast_node_init_apply(const token_slice *slice);

ast_node *parser_ast_node_init_define(const token_slice *slice);

ast_node *parser_ast_node_init_lambda(const token_slice *slice);

ast_node *parser_ast_node_init_op(const token_slice *slice);
