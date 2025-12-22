
#pragma once

#include "../namespace/namespace.h"

string *core_util_file_abs_path(const char *resolvepath, const char *filepath); // null for error

string *core_util_file_read_string(const char *filepath); // null for error
