
#include "./token.h"

def_status token_next(token_state *state, token_slice *slice) {
    if (state->line_no < 0)
        return DEF_STATUS(ERROR);
    token_slice_init(state, slice);
    def_status status = token_table_match_next(slice);
    if (status != DEF_STATUS(OK))
        return status;
    switch (slice->class) {
        case TOKEN_CLASS(INVALID):
            return DEF_STATUS(ERROR);
        case TOKEN_CLASS(NEW_LINE):
            state->line_no++;
            break;
        case TOKEN_CLASS(END):
            state->line_no = -1;
            state->str_pos = 0;
            break;
        default:
            break;
    }
    state->str_pos = slice->str_end;
    return DEF_STATUS(OK);
}
