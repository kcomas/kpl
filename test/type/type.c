
#include "../test.h"

constexpr size_t type_struct_expected_size = 8 + 8 + 4 + 2 + 1 + 1 + 8;

TEST(type_struct_size) {
    size_t size = sizeof(type);
    printf("type struct size: " COLOR(BOLD) "%lu\n" COLOR(RESET), size);
    ASSERT(sizeof(type) == type_struct_expected_size, "invalid type struct size");
}

TEST(type_shared_scalar) {
    type *scalar = type_init(TYPE_NAME(I64), TYPE_QUALIFIER(SHARED), TYPE_CLASS_UNION_EMPTY);
    type_print(scalar, stdout, 0, TYPE_PRINT(NL_END));
    type_free(scalar);
}
