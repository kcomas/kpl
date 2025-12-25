
#include "./table.h"

MEM_POOL(table_pool);

type_table *type_table_init(type *inner_type) {
    type_table *table = mem_alloc(&table_pool, sizeof(type_table));
    table->fn_idx = -1;
    table->inner_type = inner_type;
    table->type_map = map_init(0, TYPE_PRINT(_), &type_fn_table);
    return table;
}

void type_table_free(type_table *table) {
    if (table->inner_type)
        type_free(table->inner_type);
    map_free(table->type_map);
    mem_free(&table_pool, table);
}

def_status type_table_add(type_table *table, type *inner_type) {
    return map_action(&table->type_map, MAP_MODE(INSERT), DEF_PTR(inner_type), &def_unused);
}

size_t type_table_hash(const type_table *table) {
    return type_hash(table->inner_type) + map_hash(table->type_map);
}

bool type_table_eq(const type_table *table_a, const type_table *table_b) {
    if (table_a == table_b)
        return true;
    if (!table_a || !table_b)
        return false;
    return type_eq(table_a->inner_type, table_b->inner_type) && map_eq(table_a->type_map, table_b->type_map);
}

void type_table_print(const type_table *table, FILE *file, int32_t idnt, type_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    if (table->inner_type) {
        type_print(table->inner_type, file, 0, TYPE_PRINT(_));
        if (table->type_map->used)
            fprintf(file, COLOR(DARK_GREY) "; " COLOR(RESET));
    }
    map_print(table->type_map, file, 1, MAP_PRINT(NO_FIRST_IDNT) | MAP_PRINT(SEMI_SPACER));
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (print_opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
