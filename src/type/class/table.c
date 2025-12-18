
#include "./table.h"

MEM_POOL(table_pool);

type_table *type_table_init(type *inner_type) {
    type_table *table = mem_alloc(&table_pool, sizeof(type_table));
    table->fn_idx = -1;
    table->inner_type = inner_type;
    table->tag_map = map_init(0, TYPE_PRINT(_), &type_fn_table);
    return table;
}

void type_table_free(type_table *table) {
    if (table->inner_type)
        type_free(table->inner_type);
    map_free(table->tag_map);
    mem_free(&table_pool, table);
}

void type_table_print(const type_table *table, FILE *file, int32_t idnt, type_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    map_print(table->tag_map, file, 1, MAP_PRINT(SEMI_SPACER));
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
