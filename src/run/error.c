
#include "./error.h"

MEM_POOL(error_pool)

error *_error_init(def_print_fn print_fn, uint32_t data_print_opts, def_free_fn free_fn, def_data data,
    const char *msg, const char *file, const char *function, int32_t line) {
    error *er = mem_alloc(&error_pool, sizeof(error));
    er->print_fn = print_fn;
    er->data_print_opts = data_print_opts;
    er->free_fn = free_fn;
    er->data = data;
    er->msg = msg;
    er->file = file;
    er->function = function;
    er->line = line;
    return er;
}

void error_free(error *er) {
    error *tmp;
    while (er) {
        tmp = er;
        er = er->next;
        if (tmp->free_fn && tmp->data.ptr)
            tmp->free_fn(tmp->data.ptr);
        mem_free(&error_pool, tmp);
    }
}

void error_stack_add(error *restrict er, error *restrict next) {
    while (er->next)
        er = er->next;
    er->next = next;
}

void error_print(const error *er, FILE *file, int32_t idnt, error_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR(MAGENTA) "%s" COLOR(RESET) ":", er->file);
    fprintf(file, COLOR(CYAN) "%s" COLOR(RESET) ":", er->function);
    fprintf(file, COLOR(GREEN) "%d" COLOR(RESET) ":", er->line);
    if (er->msg)
        fprintf(file, COLOR(LIGHT_RED) "%s" COLOR(RESET), er->msg);
    if (er->print_fn)
        er->print_fn(er->data, file, idnt, er->data_print_opts);
    if (er->next)
        error_print(er->next, file, idnt, opts);
    if (opts & ERROR_PRINT(NL_END))
        fprintf(file, "\n");
}
