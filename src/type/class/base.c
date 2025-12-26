
#include "./base.h"

MEM_POOL(base_pool);

type_base *type_base_init(string *key) {
    type_base *base = mem_alloc(&base_pool, sizeof(type_base));
    base->key = key;
    base->type_map = map_init(0, TYPE_PRINT(_), &type_fn_table);
    return base;
}

void type_base_free(type_base *base) {
    if (base->key)
        string_free(base->key);
    map_free(base->type_map);
    mem_free(&base_pool, base);
}

def_status type_base_add(type_base *base, type *inner_type) {
    return map_action(&base->type_map, MAP_MODE(INSERT), DEF_PTR(inner_type), &def_unused);
}

size_t type_base_hash(const type_base *base) {
    return string_hash(base->key);
}

bool type_base_eq(const type_base *base_a, const type_base *base_b) {
    return string_eq(base_a->key, base_b->key);
}

void type_base_print(const type_base *base, FILE *file, int32_t idnt, type_print_opts print_opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    if (base->key)
        string_print(base->key, file, 0, STRING_PRINT(_));
    if (base->type_map->used)
        fprintf(file, COLOR(DARK_GREY) "; " COLOR(RESET));
    map_print(base->type_map, file, 1, MAP_PRINT(NO_FIRST_IDNT) | MAP_PRINT(SEMI_SPACER));
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (print_opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
