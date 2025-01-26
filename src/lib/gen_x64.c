
#include "gen_x64.h"

void as_gen_p(tbl *ot, bool args, size_t idnt) {

}

gen *gen_b(gen *g) {
    GEN_OP_A1(g, GEN_OP(RET), GEN_CLS(A), U6(X64_TYPE(U6)), NULL);
    return g;
}
