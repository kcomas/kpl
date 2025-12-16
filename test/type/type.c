
#include "../test.h"

#define TYPE_STRUCT_EXPECTED_SIZE (8 + 8 + 4 + 2 + 1 + 1 + 8)

TEST(type_struct_size) {
    size_t size = sizeof(type);
    printf("type struct size: " COLOR(BOLD) "%lu\n" COLOR(RESET), size);
    ASSERT(sizeof(type) == TYPE_STRUCT_EXPECTED_SIZE, "invalid type struct size");
}
