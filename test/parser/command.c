
#include "../test.h"

TEST(parser_command) {
    string *str = string_init_c_str("\"filename\"\\import\\export");
    token_state state;
    token_state_init(&state, str);
    token_slice slice;
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(NL_END));
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    type *ty = parser_command(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(OP) && ty->op_name == TYPE_OP_NAME(CMD_IMPORT), "invalid command");
    type_free(ty);
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ty = parser_command(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(OP) && ty->op_name == TYPE_OP_NAME(CMD_EXPORT), "invalid command");
    type_free(ty);
    string_free(str);
}
