
#include "../test.h"

TEST(x64_inst_size) {
    printf("X64 Inst Size: " COLOR(BOLD) "%lu\n" COLOR(RESET), sizeof(x64_inst));
    ASSERT(sizeof(x64_inst) == 24, "invalid X64 Inst Size");
}
