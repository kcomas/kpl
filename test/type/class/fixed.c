
#include "../../test.h"

TEST(type_fixed) {
    type *inner_type = type_init(TYPE_NAME(U64), TYPE_QUALIFIER_FLAG(_), type_class_union_empty());
    type_fixed *fixed = type_fixed_init(inner_type, 4);
    type_fixed_print(fixed, stdout, 0, TYPE_PRINT(CLASS_NL_END));
    type_fixed_free(fixed);
}
