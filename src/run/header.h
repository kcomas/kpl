
#pragma once

#include "../lib/lib.h"
#include "../run/run.h"

#define RUN_HEADER(STRUCT_NAME) MEM_HEADER(STRUCT_NAME); def_print_fn *print_fn; def_free_fn *free_fn;
