
#include "gen.h"
#include "as_x64.h"

#define GEN_OP(N) GEN_OP_##N

typedef enum {
    GEN_OP(RET)
} gen_op;

#define X64_TYPE(N) X64_TYPE_##N

typedef enum {
    X64_TYPE(N), // none
    X64_TYPE(M), // mem
    X64_TYPE(U3),
    X64_TYPE(U4),
    X64_TYPE(U5),
    X64_TYPE(U6),
    X64_TYPE(I3),
    X64_TYPE(I4),
    X64_TYPE(I5),
    X64_TYPE(I6),
    X64_TYPE(F5),
    X64_TYPE(F6)
} x64_type;

const char *x64_type_str(x64_type xt);

typedef struct {
    ssize_t r;
    alfn *sa;
    frfn *sf;
    tbl *args, *tmp; // map args, tmp to regs
} gen_st;

// pass 1 - try to give everything a reg and avoid stack
gen_st *gen_st_i(gen *g, alfn *sa, frfn *sf, tbl *args, tbl *vars, tbl *imm);

void gen_op_p(tbl *ot, bool args, size_t idnt);

gen *gen_b(gen *g);
