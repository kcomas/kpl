
#pragma once

#include "../test.h"

bool parser_test(const char *c_str, const type_name type_check_array[]);

#define NONE TYPE_NAME(_)

#define ATOM(NAME) TYPE_NAME(NAME)

#define LIST(NAME, LEFT_NODE, ...) TYPE_NAME(NAME), LEFT_NODE, __VA_ARGS__

#define OP(LEFT_NODE, RIGHT_NODE) TYPE_NAME(OP), LEFT_NODE, RIGHT_NODE

#define LEFT(NAME, LEFT_NODE) TYPE_NAME(NAME), LEFT_NODE
