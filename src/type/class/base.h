
#pragma once

#include "../header.h"

typedef struct _type_base {
    MEM_HEADER(_type_base);
    string *key;
    map *type_map;
} type_base;

type_base *type_base_init(string *key);

void type_base_free(type_base *base);

def_status type_base_add(type_base *base, type *inner_type);

size_t type_base_hash(const type_base *base);

bool type_base_eq(const type_base *base_a, const type_base *base_b);

void type_base_print(const type_base *base, FILE *file, int32_t idnt, type_print_opts print_opts);
