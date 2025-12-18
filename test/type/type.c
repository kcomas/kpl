
#include "../test.h"

#define TYPE_STRUCT_EXPECTED_SIZE (8 + 8 + 4 + 2 + 1 + 1 + 8)

TEST(type_struct_size) {
    size_t size = sizeof(type);
    printf("type struct size: " COLOR(BOLD) "%lu\n" COLOR(RESET), size);
    ASSERT(sizeof(type) == TYPE_STRUCT_EXPECTED_SIZE, "invalid type struct size");
}

TEST(type_scalar) {
    type *scalar = type_init(TYPE_NAME(I64), TYPE_QUALIFIER_FLAG(_), TYPE_CLASS_UNION_EMPTY);
    type_print(scalar, stdout, 0, TYPE_PRINT(NL_END));
    type_free(scalar);
}
