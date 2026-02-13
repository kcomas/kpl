
#include "../test.h"

TEST(parser_op) {
    string *str = string_init_c_str("1+2*3");
    token_state state;
    token_state_init(&state, str);
    token_slice slice;
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(NL_END));
    type *ty = parser_op(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(OP) && ty->op_name == TYPE_OP_NAME(ADD), "invalid op");
    type_free(ty);
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    ASSERT(token_next(&state, &slice) == DEF_STATUS(OK), "invalid token next");
    token_slice_print(&slice, stdout, 0, TOKEN_SLICE_PRINT(NL_END));
    ty = parser_op(&slice);
    type_print(ty, stdout, 0, TYPE_PRINT(NL_END));
    ASSERT(ty && ty->name == TYPE_NAME(OP) && ty->op_name == TYPE_OP_NAME(MUL), "invalid op");
    type_free(ty);
    string_free(str);
}
