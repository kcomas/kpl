
#pragma once

#include "./type.h"

type_base *namespace_import_insert(const string *filename); // null for error

type_base *namespace_import_get(const string *filename); // null for error

void namespace_import_print(FILE *file, int32_t idnt, uint32_t print_opts);
