
#pragma once

#include "vr.h"
#include "as.h"

#define GEN_STAT(N) GEN_STAT_##N

typedef enum {
    GEN_STAT(OK),
    GEN_STAT(INV)
} gen_stat;

#define GEN_OP(N) GEN_OP_##N

typedef enum {
    GEN_OP(_START),
    GEN_OP(LBL),
    GEN_OP(NOP), // used as tmp vars after mutli cmd gen
    GEN_OP(E),
    GEN_OP(L),
    GEN_OP(ENTER),
    GEN_OP(LEAVE),
    GEN_OP(SET),
    GEN_OP(REF),
    GEN_OP(CALL),
    GEN_OP(CALLNPR), // no preserve registers
    GEN_OP(CALLV), // varardic
    GEN_OP(CALLVNPR),
    GEN_OP(ADD),
    GEN_OP(SUB),
    GEN_OP(NEG),
    GEN_OP(MUL),
    GEN_OP(DIV),
    GEN_OP(MOD),
    GEN_OP(CST),
    GEN_OP(EQ),
    GEN_OP(NE),
    GEN_OP(GT),
    GEN_OP(GTE),
    GEN_OP(LT),
    GEN_OP(LTE),
    GEN_OP(AND),
    GEN_OP(OR),
    GEN_OP(JMP),
    GEN_OP(_END)
} gen_op; // not x64 opcodes, pseudo codes

const char *gen_op_str(gen_op go);

bool gen_is_cond(gen_op op);

#define GEN_CLS(N) GEN_CLS_##N

typedef enum {
    GEN_CLS(N), // none
    GEN_CLS(M), // multiple, vec of vars
    GEN_CLS(W), // (vec, vec)
    GEN_CLS(I), // index tgt, accessors
    GEN_CLS(A), // arg
    GEN_CLS(V), // var local
    GEN_CLS(S), // stack arg
    GEN_CLS(T), // tmp
    GEN_CLS(L), // label
    GEN_CLS(D) // data
} gen_cls;

const char* gen_cls_str(gen_cls cls);

typedef tbl *cls_tbl_i(void);

// oci entry te[op_id;fn;tbl[te[u4(cls)|u4(type);fn;tbl]]]

// code te[op_id;aci;ac2;ac3;fn;as_code_s;as_code_e]

typedef struct {
    uint32_t lbl; // not used internally
    ssize_t r;
    const alfr *af, *ta, *va, *ea; // al for gen, te, vr, er
    cls_tbl_i *cti;
    tbl *oci; // op cls op tbl given at start
    lst *code;
} gen;

gen *gen_i(const alfr *af, const alfr *ta, const alfr *va, const alfr *ea, cls_tbl_i cti, tbl *oci, lst *code);

gen *gen_i_gen(const gen *g);

typedef gen_stat gen_fn(gen *g, void *s, te *ci, as *a, err **e); // state

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, uint16_t type1, gen_cls cls2, uint16_t type2, gen_cls cls3, uint16_t type3, gen_fn *fn);

#define GEN_OP_A3(g, op_id, cls1, type1, cls2, type2, cls3, type3, fn) gen_op_a(g, op_id, cls1, type1, cls2, type2, cls3, type3, fn)
#define GEN_OP_A2(g, op_id, cls1, type1, cls2, type2, fn) GEN_OP_A3(g, op_id, cls1, type1, cls2, type2, GEN_CLS(N), 0, fn)
#define GEN_OP_A1(g, op_id, cls1, type1, fn) GEN_OP_A2(g, op_id, cls1, type1, GEN_CLS(N), 0, fn)
#define GEN_OP_A0(g, op_id, fn) GEN_OP_A1(g, op_id, GEN_CLS(N), 0, fn)

uint16_t gen_var_g_c(const te *v);

void gen_var_s_c(te *v, uint16_t c);

uint16_t gen_var_g_t(const te *v);

void gen_var_s_t(te *v, uint16_t t);

// var te[u4(cls)|u4(type);id]
te *gen_var_i(gen *g, frfn fr, gen_cls cls, uint16_t type, un id);

// u5(id)|u4(cls)|u4(type)
un gen_var_hsh(const te *var);

gen_stat gen_a(gen *g, size_t op_id, te *restrict ac1, te *restrict ac2, te *restrict ac3);

gen_stat gen_u_fn(gen *g, te *ci);

gen_stat gen_n(gen *g, void *st, as *a, err **e);

void gen_f(gen *g);
