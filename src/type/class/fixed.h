
#pragma once

#include "../header.h"

typedef struct _type_fixed {
    MEM_HEADER(_type_fixed);
    uint32_t value;
    type *inner_type;
} type_fixed;

type_fixed *type_fixed_init(type *inner_type, uint32_t value);

void type_fixed_free(type_fixed *fixed);

void type_fixed_print(const type_fixed *fixed, FILE *file, uint32_t idnt, type_print_opts opts);
