
#include "./fixed.h"

MEM_POOL(fixed_pool);

type_fixed *type_fixed_init(type *inner_type, uint32_t value) {
    type_fixed *fixed = mem_alloc(&fixed_pool, sizeof(type_fixed));
    fixed->inner_type = inner_type;
    fixed->value = value;
    return fixed;
}

void type_fixed_free(type_fixed *fixed) {
    if (!fixed)
        return;
    if (fixed->inner_type)
        type_free(fixed->inner_type);
    mem_free(&fixed_pool, fixed);
}

size_t type_fixed_hash(const type_fixed *fixed) {
    return fixed->value + type_hash(fixed->inner_type);
}

bool type_fixed_eq(const type_fixed *fixed_a, const type_fixed *fixed_b) {
    if (fixed_a == fixed_b)
        return true;
    if (!fixed_a || !fixed_b)
        return false;
    return fixed_a->value == fixed_b->value && type_eq(fixed_a->inner_type, fixed_b->inner_type);
}

void type_fixed_print(const type_fixed *fixed, FILE *file, uint32_t idnt, type_print_opts print_opts) {
    if (!fixed)
        return;
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    if (fixed->inner_type)
        type_print(fixed->inner_type, file, 0, TYPE_PRINT(_));
    fprintf(file, COLOR(DARK_GREY) "; " COLOR2(BOLD, MAGENTA) "%u" COLOR2(BOLD, WHITE) "]" COLOR(RESET),
        fixed->value);
    if (print_opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
