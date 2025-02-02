
#include "../gen_x64.h"

static gen_stat call_auml_fn(alfn *al, frfn fr, gen *g, void *s, te *ci, as *a) {

}

void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), U3(X64_TYPE(U6)), GEN_CLS(M), U3(X64_TYPE(N)), GEN_CLS(L), U3(X64_TYPE(N)), &call_auml_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), U3(X64_TYPE(U6)), GEN_CLS(M), U3(X64_TYPE(N)), GEN_CLS(L), U3(X64_TYPE(M)), &call_auml_fn);
}
