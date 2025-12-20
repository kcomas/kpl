
#pragma once

#include "../type/type.h"

string *namespace_util_file_abs_path(const char *resolvepath, const char *filepath); // null for error

string *namespace_util_file_read_string(const char *filepath); // null for error
