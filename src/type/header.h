
#pragma once

#include "../run/run.h"
#include "./qualifier_flags.h"
#include "./class_name.h"

typedef struct _type type;

void type_free(type *ty);

size_t type_hash(const type *ty);

bool type_eq(const type *ty_a, const type *ty_b);

type *type_copy(type *ty);

#define TYPE_PRINT(NAME) TYPE_PRINT_OPTS_##NAME

typedef enum [[gnu::packed]] {
    TYPE_PRINT(NL_END)         = 1 << 0,
    TYPE_PRINT(CLASS_NL_END)   = 1 << 1,
    TYPE_PRINT(_)              = 0
} type_print_opts;

void type_print(const type *ty, FILE *file, int32_t idnt, type_print_opts opts);

extern def_fn_table type_fn_table;
