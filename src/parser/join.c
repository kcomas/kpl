
#include "./join.h"

def_status parser_ast_node_join_value(ast_node **current_node, ast_node *next_node) {
    if (!*current_node) {
        *current_node = next_node;
        return DEF_STATUS(OK);
    }
    if (!(*current_node)->ty || ast_node_get_ast_type((*current_node)->ty) != AST_NODE_TYPE(OP))
        return DEF_STATUS(ERROR);
    if ((*current_node)->children.op && (*current_node)->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data.ptr)
        return DEF_STATUS(ERROR);
    if (ast_node_op_set_side(*current_node, AST_NODE_OP_SIDE(RIGHT), next_node) != DEF_STATUS(OK))
        return DEF_STATUS(ERROR);
    return DEF_STATUS(OK);
}

def_status parser_ast_node_join_op(ast_node **current_node, ast_node *next_node) {
    if (!*current_node) {
        *current_node = next_node;
        return DEF_STATUS(OK);
    }
    if (!(*current_node)->ty)
        return DEF_STATUS(ERROR);
    if (ast_node_get_ast_type((*current_node)->ty) == AST_NODE_TYPE(OP)) {
        if ((*current_node)->children.op) {
            ast_node *right_node = (*current_node)->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data.ptr;
            if (right_node) {
                (*current_node)->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data = def();
                if (ast_node_op_set_side(next_node, AST_NODE_OP_SIDE(LEFT), right_node) != DEF_STATUS(OK))
                    return DEF_STATUS(ERROR);
            }
        }
        if (ast_node_op_set_side(*current_node, AST_NODE_OP_SIDE(RIGHT), next_node) != DEF_STATUS(OK))
            return DEF_STATUS(ERROR);
    } else
        if (ast_node_op_set_side(next_node, AST_NODE_OP_SIDE(LEFT), *current_node) != DEF_STATUS(OK))
            return DEF_STATUS(ERROR);
    *current_node = next_node;
    return DEF_STATUS(OK);
}

def_status parser_ast_node_join_left(ast_node **current_node, ast_node *next_node) {
    if (!*current_node) {
        *current_node = next_node;
        return DEF_STATUS(OK);
    }
    if (!(*current_node)->ty)
        return DEF_STATUS(ERROR);
    if (ast_node_get_ast_type((*current_node)->ty) == AST_NODE_TYPE(OP) && (*current_node)->children.op) {
        ast_node *right_node = (*current_node)->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data.ptr;
        if (!right_node)
            return DEF_STATUS(ERROR);
        (*current_node)->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data = def();
        next_node->parent = right_node->parent;
        right_node->parent = next_node;
        next_node->left = right_node;
        if (ast_node_op_set_side(*current_node, AST_NODE_OP_SIDE(RIGHT), next_node) != DEF_STATUS(OK))
            return DEF_STATUS(ERROR);
    } else {
        next_node->parent = (*current_node)->parent;
        (*current_node)->parent = next_node;
        next_node->left = *current_node;
        *current_node = next_node;
    }
    return DEF_STATUS(OK);
}
