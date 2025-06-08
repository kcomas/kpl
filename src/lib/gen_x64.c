
#include "gen_x64.h"

const char *x64_type_str(x64_type xt) {
    switch (xt) {
        case X64_TYPE(N): return "N";
        case X64_TYPE(M): return "M";
        case X64_TYPE(U3): return "U3";
        case X64_TYPE(U4): return "U4";
        case X64_TYPE(U5): return "U5";
        case X64_TYPE(U6): return "U6";
        case X64_TYPE(I3): return "I3";
        case X64_TYPE(I4): return "I4";
        case X64_TYPE(I5): return "I5";
        case X64_TYPE(I6): return "I6";
        case X64_TYPE(F5): return "F5";
        case X64_TYPE(F6): return "F6";
        default:
            break;
    }
    return "INV";
}

void gen_op_p(tbl *ot, bool args, size_t idnt) {

}

gen *gen_b(gen *g) {
    GEN_OP_A1(g, GEN_OP(RET), GEN_CLS(A), U6(X64_TYPE(U6)), NULL);
    return g;
}
