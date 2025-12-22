
#include "./result.h"

result result_value(def_data value) {
    return (result) { .value = value, .er = NULL };
}

result result_error(error *er) {
    return (result) { .value = DEF(_), .er = er };
}
