
#include "./node.h"

MEM_POOL(query_node_pool);

query_node *query_node_init(query_match_fn match_fn, type_class class, uint32_t type_name_mask, ...) {
    query_node *node = mem_alloc(&query_node_pool, sizeof(query_node));
    node->class = class;
    memset(node->type_op_name_mask, 0, TYPE_OP_MASK_BYTE_SIZE);
    node->match_fn = match_fn;
    uint8_t inner_idx;
    for (inner_idx = 0; inner_idx < QUERY_NODE_INNER_SIZE; inner_idx++)
        node->inner[inner_idx] = nullptr;
    inner_idx = 0;
    va_list args;
    va_start(args, type_name_mask);
    query_node *next;
    while (inner_idx < QUERY_NODE_INNER_SIZE && (next = va_arg(args, query_node*)))
        node->inner[inner_idx++] = next;
    va_end(args);
    if (inner_idx > QUERY_NODE_INNER_SIZE) {
        query_node_free(node);
        return nullptr;
    }
    node->type_name_mask = type_name_mask;
    return node;
}

void query_node_free(query_node *node) {
    for (uint8_t inner_idx = 0; inner_idx < QUERY_NODE_INNER_SIZE; inner_idx++)
        if (node->inner[inner_idx])
            query_node_free(node->inner[inner_idx]);
    mem_free(&query_node_pool, node);
}

void query_node_print(const query_node *node, FILE *file, int32_t idnt, query_node_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, GREEN) "%s" COLOR(RESET), type_class_str(node->class));
    if (node->type_name_mask) {
        fprintf(file, COLOR(BOLD) "[" COLOR(RESET));
        bool spacer = false;
        for (uint32_t name_bit_idx = 0; name_bit_idx < TYPE_CLASS_BITS_PER_NAME; name_bit_idx++)
            if (node->type_name_mask & (1 << name_bit_idx)) {
                if (spacer)
                    fprintf(file, COLOR(BOLD) "|" COLOR(RESET));
                else
                    spacer = true;
                fprintf(file, COLOR2(BOLD, LIGHT_YELLOW) "%s" COLOR(RESET),
                        type_name_str(type_class_first_name(node->class) + name_bit_idx));
            }
        if (node->class == TYPE_CLASS(LIST) && (node->type_name_mask & type_name_get_mask(TYPE_NAME(OP)))) {
            fprintf(file, COLOR(BOLD) "::" COLOR(RESET));
            spacer = false;
            for (uint32_t op_name_idx = TYPE_OP_NAME(CMD_IS_MAIN);
                    op_name_idx < TYPE_OP_NAME(INVALID); op_name_idx++)
                if (node->type_op_name_mask[op_name_idx / TYPE_OP_BITS_PER_BYTE] &
                        (1 << (op_name_idx % TYPE_OP_BITS_PER_BYTE))) {
                    if (spacer)
                        fprintf(file, COLOR(BOLD) "|" COLOR(RESET));
                    else
                        spacer = true;
                    fprintf(file, COLOR2(BOLD, LIGHT_BLUE) "%s" COLOR(RESET), type_op_name_str(op_name_idx));
                }
        }
        fprintf(file, COLOR(BOLD) "]" COLOR(RESET));
    }
    for (uint8_t inner_idx = 0; inner_idx < QUERY_NODE_INNER_SIZE; inner_idx++)
        if (node->inner[inner_idx]) {
            fprintf(file, COLOR(BOLD) "{" COLOR(RESET));
            query_node_print(node->inner[inner_idx], file, 0, QUERY_NODE_PRINT(_));
            fprintf(file, COLOR(BOLD) "}" COLOR(RESET));
        }
    if (print_opts & QUERY_NODE_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table query_node_fn_table = {
    .hash_fn = nullptr,
    .cmp_fn = nullptr,
    .eq_fn = nullptr,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = (void*) query_node_print,
    .free_fn = (void*) query_node_free
};
