
#include "gen_x64.h"

gen_st *gen_st_i(gen *g, alfn *sa, frfn *sf, tbl *args, tbl *vars, tbl *imm) {

}

gen *gen_b(gen *g) {
    GEN_OP_A1(g, GEN_OP(RET), GEN_CLS(A), U6(X64_TYPE(U6)), NULL);
    return g;
}
