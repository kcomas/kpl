
#pragma once

#include "./table.h"

error *parser_run(ast_container *cont, const string *str, type_base *base, ast_node *wrapper);
