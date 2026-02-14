
#include "./node.h"

void ast_container_init(ast_container *cont, const string *str, type_base *base, ast_node *wrapper) {
    cont->str = str;
    cont->base = base;
    cont->root = nullptr;
    wrapper->ty = nullptr;
    wrapper->children.cont = cont;
    wrapper->position = ast_position_init(UINT32_MAX, UINT16_MAX, UINT16_MAX);
}

extern inline ast_position ast_position_init(uint32_t str_start, int16_t str_len, int16_t line_no);

extern inline void ast_container_set_root_parent(ast_container* cont, ast_node *wrapper);

extern inline ast_node_children ast_node_children_empty(void);

extern inline ast_node_children ast_node_children_stmts(list *stmts);

extern inline ast_node_children ast_node_children_op(tuple *op);

MEM_POOL(ast_node_pool);

ast_node *ast_node_init(type *ty, ast_node *parent, ast_node_children children, ast_position pos) {
    ast_node *node = mem_alloc(&ast_node_pool, sizeof(ast_node));
    node->ty = ty;
    node->parent = parent;
    node->children = children;
    node->position = pos;
    return node;
}

void ast_node_free(ast_node *node) {
    switch (ast_node_get_ast_type(node->ty)) {
        case AST_NODE_TYPE(ATOM):
        case AST_NODE_TYPE(_):
            break;
        case AST_NODE_TYPE(LIST):
            if (node->children.stmts)
                list_free(node->children.stmts);
            break;
        case AST_NODE_TYPE(OP):
            if (node->children.op)
                tuple_free(node->children.op);
            break;
    }
    type_free(node->ty);
    mem_free(&ast_node_pool, node);
}

ast_container *ast_node_get_container(const ast_node *node) {
    while (node->parent)
        node = node->parent;
    return node->children.cont;
}

void ast_node_print(const ast_node *node, FILE *file, int32_t idnt, ast_node_print_opts print_opts) {
    if (!node)
        return;
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(BOLD) "❲" COLOR(RESET));
    if (node->position.str_len) {
        if (print_opts & AST_NODE_PRINT(POSITION))
            fprintf(file, COLOR(BOLD) "%u::%u+%u" COLOR(RESET),
                    node->position.line_no, node->position.str_start, node->position.str_len);
        if (print_opts & AST_NODE_PRINT(STRING)) {
            fprintf(file, "|" COLOR(GREEN));
            const string *str = ast_node_get_container(node)->str;
            for (uint32_t str_idx = node->position.str_start;
                    str_idx < node->position.str_start + node->position.str_len; str_idx++)
                fprintf(file, "%c", str->data[str_idx]);
            fprintf(file, COLOR(RESET) "|");
        }
    }
    type_print(node->ty, file, 0, TYPE_PRINT(_));
    switch (ast_node_get_ast_type(node->ty)) {
        case AST_NODE_TYPE(ATOM):
        case AST_NODE_TYPE(_):
            break;
        case AST_NODE_TYPE(LIST):
            if (!node->children.stmts)
                break;
            fprintf(file, COLOR(BOLD) "{" COLOR(RESET));
            if (print_opts & AST_NODE_PRINT(NL_END))
                fprintf(file, "\n");
            list_print(node->children.stmts, file, idnt + 1, LIST_PRINT(_));
            fprintf(file, COLOR(BOLD) "}" COLOR(RESET));
            break;
        case AST_NODE_TYPE(OP):
            if (!node->children.op)
                break;
            if (print_opts & AST_NODE_PRINT(NL_END))
                fprintf(file, "\n");
            tuple_print(node->children.op, file, idnt + 1, TUPLE_PRINT(_));
            break;
    }
    fprintf(file, COLOR(BOLD) "❳" COLOR(RESET));
    if (print_opts & AST_NODE_PRINT(NL_END))
        fprintf(file, "\n");
}

static size_t ast_node_hash_fn(const def_data data) {
    const ast_node *node = data.ptr;
    size_t hash = 0;
    if (!node)
        return hash;
    switch (ast_node_get_ast_type(node->ty)) {
        case AST_NODE_TYPE(ATOM):
        case AST_NODE_TYPE(_):
            break;
        case AST_NODE_TYPE(LIST):
            if (!node->children.stmts)
                break;
            hash += list_hash(node->children.stmts);
            break;
        case AST_NODE_TYPE(OP):
            if (!node->children.op)
                break;
            hash += tuple_hash(node->children.op);
            break;
    }
    return hash + type_hash(node->ty);
}

static bool ast_node_eq_fn(const def_data data_a, const def_data data_b) {
    const ast_node *node_a = data_a.ptr, *node_b = data_b.ptr;
    if (node_a == node_b)
        return true;
    if (!node_a || !node_b)
        return false;
    if (!type_eq(node_a->ty, node_b->ty))
        return false;
    switch (ast_node_get_ast_type(node_a->ty)) {
        case AST_NODE_TYPE(ATOM):
        case AST_NODE_TYPE(_):
            return true;
        case AST_NODE_TYPE(LIST):
            return list_eq(node_a->children.stmts, node_b->children.stmts);
        case AST_NODE_TYPE(OP):
            return tuple_eq(node_a->children.op, node_b->children.op);
        default:
            break;
    }
    return false;
}

def_fn_table ast_node_fn_table = {
    .hash_fn = ast_node_hash_fn,
    .cmp_fn = nullptr,
    .eq_fn = ast_node_eq_fn,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = (void*) ast_node_print,
    .free_fn = (void*) ast_node_free
};

def_status ast_node_op_set_side(ast_node *restrict op_node, ast_node_op_side side, ast_node *restrict child) {
    if (!op_node->ty || op_node->ty->name != TYPE_NAME(OP) || side > AST_NODE_OP_SIDE(RIGHT))
        return DEF_STATUS(ERROR);
    if (!op_node->children.op)
        op_node->children.op = tuple_init(AST_NODE_OP_SIZE);
    if (tuple_set(op_node->children.op, AST_NODE_PRINT(NL_END), &ast_node_fn_table, def_ptr(child), side) !=
            DEF_STATUS(OK))
        return DEF_STATUS(ERROR);
    child->parent = op_node;
    list_item *item = op_node->ty->class_union.li->head->next;
    if (side == AST_NODE_OP_SIDE(RIGHT))
        item = item->next;
    item->data.ptr = type_copy(child->ty);
    return DEF_STATUS(OK);
}

def_status ast_node_list_add_back(ast_node *restrict list_node, ast_node *restrict child) {
    if (!list_node->ty || type_name_get_class(list_node->ty->name) != TYPE_CLASS(LIST))
        return DEF_STATUS(ERROR);
    if (!list_node->children.stmts)
        list_node->children.stmts = list_init(AST_NODE_PRINT(NL_END), &ast_node_fn_table);
    child->parent = list_node;
    list_add_back(list_node->children.stmts, def_ptr(child));
    return DEF_STATUS(OK);
}
