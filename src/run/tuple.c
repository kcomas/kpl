
#include "./tuple.h"

MEM_POOL(tuple_pool);

tuple *tuple_init(uint32_t size) {
    if (size < 2)
        return nullptr;
    tuple *tu = mem_alloc(&tuple_pool, sizeof(tuple) + sizeof(tuple_item) * size);
    tu->size = size;
    for (uint32_t tu_idx = 0; tu_idx < size; tu_idx++)
        tu->items[tu_idx] = (tuple_item) { .print_opts = 0, .fn_table = &def_unused_fn_table, .data = def() };
    return tu;
}

void tuple_free(tuple *tu) {
    for (uint32_t tu_idx = 0; tu_idx < tu->size; tu_idx++)
        if (tu->items[tu_idx].fn_table->free_fn)
            tu->items[tu_idx].fn_table->free_fn(tu->items[tu_idx].data.ptr);
    mem_free(&tuple_pool, tu);
}

def_status tuple_set(tuple *tu, uint32_t print_opts, def_fn_table *fn_table, def_data data, uint32_t idx) {
    if (idx >= tu->size)
        return DEF_STATUS(ERROR);
    if (tu->items[idx].fn_table->free_fn)
        tu->items[idx].fn_table->free_fn(tu->items[idx].data.ptr);
    tu->items[idx] = (tuple_item) { .print_opts = print_opts, .fn_table = fn_table, .data = data };
    return DEF_STATUS(OK);
}

tuple_item *tuple_get(tuple *tu, uint32_t idx) {
    if (idx >= tu->size)
        return nullptr;
    return &tu->items[idx];
}

void tuple_print(const tuple *tu, FILE *file, uint32_t idnt, tuple_print_opts print_opts) {
    for (uint32_t tu_idx = 0; tu_idx < tu->size; tu_idx++) {
        int32_t data_idnt = idnt;
        if (!tu_idx && (print_opts & TUPLE_PRINT_OPTS(NO_FIRST_IDNT)))
            data_idnt = 0;
        tu->items[tu_idx].fn_table->print_fn(tu->items[tu_idx].data, file, data_idnt,
                tu->items[tu_idx].print_opts);
        if (print_opts & TUPLE_PRINT_OPTS(NL_EACH))
            fprintf(file, "\n");
    }
    if (print_opts & TUPLE_PRINT_OPTS(NL_END))
        fprintf(file, "\n");
}
