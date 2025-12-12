
#pragma once

#include "./header.h"

typedef struct _error {
    RUN_HEADER(_error);
    def_data data;
    const char *msg, *file, *function;
    int32_t line;
} error;

#define ERROR_PRINT(NAME) ERROR_PRINT_##NAME

error *_error_init(def_print_fn print_fn, uint32_t print_opts, def_free_fn free_fn, def_data data,
    const char *msg, const char *file, const char *function, int32_t line);

#define ERROR_INIT(PRINT_FN, DATA_PRINT_OPTS, FREE_FN, DATA, MSG) \
    _error_init(PRINT_FN, DATA_PRINT_OPTS, FREE_FN, DATA, MSG, __FILE__, __FUNCTION__, __LINE__)

void error_free(error *er);

void error_stack_add(error *restrict er, error *restrict next);

typedef enum [[gnu::packed]] {
    ERROR_PRINT(NL_END)     = 1 << 0
} error_print_opts;

void error_print(const error *er, FILE *file, int32_t idnt, error_print_opts opts);
