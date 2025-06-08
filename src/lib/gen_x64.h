
#include "vr.h"
#include "gen.h"
#include "as_x64.h"

#define GEN_OP(N) GEN_OP_##N

typedef enum {
    GEN_OP(_START),
    GEN_OP(ADD),
    GEN_OP(RET),
    GEN_OP(_END)
} gen_op; // not x64 opcodes, pseudo codes

const char *gen_op_str(gen_op go);

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
    vr *rstk, *xstk;
} gen_st;

// pass 1 - try to give everything a reg and avoid stack
gen_st *gen_st_i(gen *g, alfn *sa, frfn *sf, tbl *args, tbl *vars, tbl *tmp, vr *rstk, vr *xstk);

void gen_op_p(tbl *ot, bool ci, size_t idnt);

void gen_p(gen *g, uint8_t *m);

gen *gen_b(gen *g);
