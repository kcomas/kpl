
#pragma once

#include "../test.h"

bool parser_check(const ast_node *node, const type_name check_array[], uint32_t *check_array_idx);

#define NONE TYPE_NAME(_)

#define ATOM(NAME) TYPE_NAME(NAME)

#define LIST(NAME, LEFT_NODE, ...) TYPE_NAME(NAME), LEFT_NODE, __VA_ARGS__

#define OP(LEFT_NODE, RIGHT_NODE) TYPE_NAME(OP), LEFT_NODE, RIGHT_NODE

#define LEFT(NAME, LEFT_NODE) TYPE_NAME(NAME), LEFT_NODE
