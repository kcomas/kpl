
#pragma once

#include "./error.h"

typedef struct {
    def_data value;
    error *er;
} result;

result result_value(def_data value);

result result_error(error *er);
