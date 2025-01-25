
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
    X64_TYPE(F6),
} x64_type;

// TODO register state
typedef struct {
    tbl *args, *vars, *imm; // map args, imm to regs, stack
} gen_st;

gen *gen_b(gen *g);
