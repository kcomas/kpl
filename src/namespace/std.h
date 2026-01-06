
#pragma once

#include "./type.h"

#ifndef NAMESPACE_STD_FN_COUNT
#define NAMESPACE_STD_FN_COUNT 100
#endif

// c strings with last string null

void namespace_std_add(type *ty, ...);

void namespace_std_add_fn(type *ty, void *fn, ...);

void *namespace_std_get_fn(type *ty);

void namespace_std_print(FILE *file, int32_t idnt, uint32_t print_opts);
