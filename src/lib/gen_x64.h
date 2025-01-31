
#include "vr.h"
#include "gen.h"
#include "as_x64.h"

#define GEN_OP(N) GEN_OP_##N

typedef enum {
    GEN_OP(_START),
    GEN_OP(ENTER),
    GEN_OP(LEAVE),
    GEN_OP(ADD),
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

void gen_op_p(const tbl *ot, bool ci, size_t idnt);

void gen_p(const gen *g, const uint8_t *m);

// atm entry te[(id << 8) + cls;te[cls;info;id];reg]

// lat entry te[(id << 8) + cls;te[cls;info;id];code]

typedef struct {
    uint8_t vc, rac, xac; // var count for stk alloc, arg count
    ssize_t r;
    alfn *sa;
    frfn *sf, *atmf, *latf;
    tbl *atm, *lat; // map args, tmp to regs, last code of arg, tmp
    vr *rstk, *xstk; // available regs and xmm
} gen_st;

gen_st *gen_st_i(alfn *sa, frfn *sf, frfn *atmf, frfn *latf, tbl *atm, tbl *lat, vr *rstk, vr *xstk);

void gen_st_p(const gen_st *st);

// pass 1
gen_stat gen_st_p1(gen *g, gen_st *st);

void gen_st_f(gen_st *st);

gen *gen_b(gen *g);
