
#pragma once

#include "../header.h"

list *type_list_init(void);

void type_list_free(list *li);

void type_list_add(list *li, type *inner_type);

size_t type_list_hash(const list *li);

bool type_list_eq(const list *li_a, const list *li_b);

void type_list_print(const list *li, FILE *file, uint32_t idnt, type_print_opts print_opts);
