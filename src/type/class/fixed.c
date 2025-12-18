
#include "./fixed.h"

MEM_POOL(fixed_pool);

type_fixed *type_fixed_init(type *inner_type, uint32_t value) {
    type_fixed *fixed = mem_alloc(&fixed_pool, sizeof(type_fixed));
    fixed->inner_type = inner_type;
    fixed->value = value;
    return fixed;
}

void type_fixed_free(type_fixed *fixed) {
    if (fixed->inner_type)
        type_free(fixed->inner_type);
    mem_free(&fixed_pool, fixed);
}

void type_fixed_print(const type_fixed *fixed, FILE *file, uint32_t idnt, type_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    if (fixed->inner_type)
        type_print(fixed->inner_type, file, 0, opts);
    fprintf(file, COLOR(DARK_GREY) "; " COLOR2(BOLD, MAGENTA) "%u" COLOR2(BOLD, WHITE) "]" COLOR(RESET),
        fixed->value);
    if (opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
