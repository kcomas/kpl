
#pragma once

#include "./error.h"

typedef struct {
    def_data value;
    error *er;
} result;

inline result result_value(def_data value) {
    return (result) { .value = value, .er = nullptr };
}

inline result result_error(error *er) {
    return (result) { .value = DEF(_), .er = er };
}
