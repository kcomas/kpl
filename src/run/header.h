
#pragma once

#include "../lib/lib.h"

#define RUN_HEADER(STRUCT_NAME) MEM_HEADER(STRUCT_NAME); uint32_t print_opts; def_print_fn *print_fn; def_free_fn *free_fn;
