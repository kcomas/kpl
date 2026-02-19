
#include "./list.h"

list *type_list_init(void) {
    return list_init(LIST_PRINT(_), &type_fn_table);
}

void type_list_free(list *li) {
    if (!li)
        return;
    list_free(li);
}

void type_list_add(list *li, type *inner_type) {
    return list_add_back(li, def_ptr(inner_type));
}

size_t type_list_hash(const list *li) {
    return list_hash(li);
}

bool type_list_eq(const list *li_a, const list *li_b) {
    return list_eq(li_a, li_b);
}

void type_list_print(const list *li, FILE *file, uint32_t idnt, type_print_opts print_opts) {
    if (!li)
        return;
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    list_print(li, file, 1, LIST_PRINT(NO_FIRST_IDNT) | LIST_PRINT(SEMI_SPACER));
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (print_opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");

}
