
#include "./helper.h"

static bool check_empty_node(const type_name check_array[], uint32_t *check_array_idx) {
    bool status = check_array[*check_array_idx] == TYPE_NAME(_);
    if (!status)
        printf(COLOR2(BOLD, RED) "EXPECTING %s, NONE FOUND\n" COLOR(RESET),
                type_name_str(check_array[*check_array_idx]));
    ++*check_array_idx;
    return status;
}

static bool parser_check_op(const ast_node *node, const type_name check_array[], uint32_t *check_array_idx) {
    if (!node->children.op)
        return check_empty_node(check_array, check_array_idx) && check_empty_node(check_array, check_array_idx);
    const ast_node *left_node = node->children.op->items[AST_NODE_OP_SIDE(LEFT)].data.ptr;
    const ast_node *right_node = node->children.op->items[AST_NODE_OP_SIDE(RIGHT)].data.ptr;
    return parser_check(left_node, check_array, check_array_idx) &&
        parser_check(right_node, check_array, check_array_idx);
}

static bool parser_check_list(const ast_node *node, const type_name check_array[], uint32_t *check_array_idx) {
    if (!parser_check(node->left, check_array, check_array_idx))
        return false;
    if (!node->children.stmts)
        return check_empty_node(check_array, check_array_idx);
    const list_item *head = node->children.stmts->head;
    while (head) {
        if (!parser_check(head->data.ptr, check_array, check_array_idx))
            return false;
        head = head->next;
    }
    return true;
}

bool parser_check(const ast_node *node, const type_name check_array[], uint32_t *check_array_idx) {
    if (!node)
        return check_empty_node(check_array, check_array_idx);
    if (!node->ty) {
        printf(COLOR2(BOLD, RED) "NODE WITHOUT TYPE FOUND\n" COLOR(RESET));
        return false;
    }
    if (node->ty->name != check_array[*check_array_idx]) {
        printf(COLOR2(BOLD, RED) "FOUND %s, EXPECTING: %s\n",
            type_name_str(node->ty->name), type_name_str(check_array[*check_array_idx]));
        return false;
    }
    ++*check_array_idx;
    switch (ast_node_get_ast_type(node->ty)) {
        case AST_NODE_TYPE(ATOM):
            return true;
        case AST_NODE_TYPE(LIST):
            return parser_check_list(node, check_array, check_array_idx);
        case AST_NODE_TYPE(OP):
            return parser_check_op(node, check_array, check_array_idx);
        case AST_NODE_TYPE(LEFT):
            return parser_check(node->left, check_array, check_array_idx);
        default:
            break;
    }
    return false;
}
