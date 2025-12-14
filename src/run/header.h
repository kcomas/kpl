
#pragma once

#include "../lib/lib.h"

#define RUN_HEADER(STRUCT_NAME) MEM_HEADER(STRUCT_NAME); uint32_t print_opts; def_fn_table *fn_table;

typedef struct _run_obj {
    RUN_HEADER(_run_obj);
} run_obj;
