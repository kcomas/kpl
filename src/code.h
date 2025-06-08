
#pragma once

#include "kpl.h"
#include "type.h"

#define CODE_STAT(N) CODE_STAT_##N

typedef enum {
    CODE_STAT(OK),
    CODE_STAT(INV_ER_T),
    CODE_STAT(INV_ER_GCR),
    CODE_STAT(ER_N_ER_T),
    CODE_STAT(INV_TC),
    CODE_STAT(INV_L_ASS), // left side : invalid
    CODE_STAT(INV_R_ASS), // right side : invalid
    CODE_STAT(INV_STR_ESC), /* invalid \ */
    CODE_STAT(NO_OP_FOR_VAL_T), // no type for val, should not happen
    CODE_STAT(TBL_FOUND),
    CODE_STAT(MOD_FOUND),
    CODE_STAT(NO_T_FOR_TE_IDX),
    CODE_STAT(NO_T_FOR_VR_GC),
    CODE_STAT(NO_T_FOR_ST_IDX),
    CODE_STAT(SYM_NO_T_FOR_A),
    CODE_STAT(SYM_INV_TBL_R),
    CODE_STAT(SYM_INV),
    CODE_STAT(NO_T_FOR_IF_COND), // cannot not get if conds type
    CODE_STAT(NO_T_FOR_LOP_COND),
    CODE_STAT(ARG_LEN_GT_LOCAL_LEN), // should not happen
    CODE_STAT(FN_RET_T_INV), // cannot get ret type of fn
    CODE_STAT(FN_RET_ER_T_INV), // cannot get ret type of fn
    CODE_STAT(TC_R_N), // right side for try catch null
    CODE_STAT(ASS_R_N),
    CODE_STAT(ASS_TE_INV),
    CODE_STAT(INV_TYPE_STORE_VD),
    CODE_STAT(VAR_TYPE_U),
    CODE_STAT(INV_INT_CST_PUSH),
    CODE_STAT(INV_CST_INT_TO_FD),
    CODE_STAT(INV_CST_SG),
    CODE_STAT(NO_T_VR_GC),
    CODE_STAT(INV_CST_FD),
    CODE_STAT(INV_CST),
    CODE_STAT(NO_TYPE_COR_INT),
    CODE_STAT(GC_INV),
    CODE_STAT(OP_NO_T_L), // op no left type
    CODE_STAT(OP_NO_T_R), // op no right type
    CODE_STAT(LD_MOD_F),
    CODE_STAT(INV_ADD_T_L),
    CODE_STAT(INV_SUB_T_R),
    CODE_STAT(INV_SUB_VR_T_R),
    CODE_STAT(INV_SG_CNCT),
    CODE_STAT(INV_CNCT_OP),
    CODE_STAT(INV_FD_OP),
    CODE_STAT(CALL_RES_NOT_SELF),
    CODE_STAT(CALL_T_N_FN),
    CODE_STAT(CALL_T_ER_T_INV),
    CODE_STAT(CALL_CT_ARG_T_GC_INV),
    CODE_STAT(INV_CALL_TGT),
    CODE_STAT(NO_OP_FOR_RET_VAL_T)
} code_stat;

const char *code_stat_str(code_stat cstat);

typedef struct {
    al *a;
    er *e;
} code_st;

inline void code_st_i(code_st *const cs, al *const a, er *const e) {
    cs->a = a;
    cs->e = e;
}

inline code_stat code_er(code_st *const cs, const char *const fnn, code_stat cstat, const ast *const a) {
    if (cstat == CODE_STAT(OK)) return cstat;
    er_itm *ei = er_itm_i(cs->a, ER(CODE), fnn, code_stat_str(cstat));
    if (a) {
        ei->lno = a->t.lno;
        ei->cno = a->t.cno;
    }
    er_a(cs->e, ei);
    return cstat;
}

#define OP_C(N) OP_C_##N

typedef enum {
    OP_C(EFN), // enter fn
    OP_C(PUSH),
    OP_C(POP),
    OP_C(SWAP),
    OP_C(RFN), // return fn
    OP_C(CFN), // call fn
    OP_C(CS), // call self
    // data
    OP_C(AG), // allocate globals
    OP_C(SG), // store global
    OP_C(LG), // load global
    OP_C(LM), // load module
    OP_C(AL), // allocate locals
    OP_C(SL), // store local
    OP_C(LL), // load local
    OP_C(SA), // store arg
    OP_C(LA), // load arg
    OP_C(PV), // push value
    OP_C(CTSV), // create tuple from stack u6 is length
    OP_C(GIDX), // get index te, vr, st u6 is i
    OP_C(SIDX), // set idx te, vr, st u6 is i
    OP_C(VRA), // vector add, push
    OP_C(VRS), // vector sub, pop
    // control
    OP_C(IF),
    OP_C(COND), // jmp if false
    OP_C(LOP),
    // coalesce
    OP_C(ZOO), // convert to zero or one
    // error
    OP_C(TE), // throw error
    OP_C(CE), // catch error
    OP_C(PE), // panic error
    // ops
    OP_C(CST),
    OP_C(CSTSG), // cast type to string
    OP_C(ADD),
    OP_C(SUB),
    OP_C(MUL),
    OP_C(EQ),
    OP_C(NOT),
    OP_C(GT),
    OP_C(LT),
    OP_C(OR),
    OP_C(CNCTSG), // sg cnct op type is either sg or te
    OP_C(WFD), // OP_T is type to be written
    // GC
    OP_C(RCI),
    OP_C(RCD),
    OP_C(RCF), // dec ref count of type ret if gt 0
    OP_C(GC), // type is base type
    OP_C(GCTSVI), // gc idx in tsv
    OP_C(GCVR), // gc vr of type
    OP_C(DEL) // delete top of stack free ptr
} op_c;

const char *op_c_get_str(op_c oc);

typedef struct {
   code *cond, *body;
} op_if;

typedef struct {
    uint8_t id;
    type t;
} slv; // store load var

#define SLV(ID, T) .v = (slv) {ID, T}

typedef struct {
    size_t len;
    mod *m;
    code *gc;
} ctsv; // create tuple, strut, vec

inline ctsv *ctsv_i(al *const a, size_t len, mod *const m, code *const gc) {
    ctsv *tsv = ala(a, sizeof(ctsv));
    tsv->len = len;
    tsv->m = m;
    tsv->gc = gc;
    return tsv;
}

void code_f(code *c);

inline void ctsv_f(ctsv *tsv) {
    FNNF(tsv->gc, code_f);
    alf(tsv);
}

typedef struct {
    type t;
    bool ec;
} rer;

#define RER(T, EC) .e = (rer) {T, (bool) EC}

typedef union _op_d {
    type t;
    bool bl;
    uint8_t u3;
    uint16_t u4;
    uint32_t u5;
    uint64_t u6;
    int8_t i3;
    int16_t i4;
    int32_t i5;
    int64_t i6;
    float f;
    double d;
    int fd;
    slv v;
    code *c;
    rer e;
    op_if *of;
    char *sg; // null term
    ctsv *tsv;
} op_d;

typedef struct {
    op_c oc;
    type ot;
    uint16_t jidx, jlen; // idx and len in jit code of this op
    op_d od;
    const ast *a; // freed in mod
} op;

typedef struct _code {
    size_t len, size;
    jit_fn *jf; // jit fn
    op ops[];
} code;

#ifndef CODE_I_SIZE
    #define CODE_I_SIZE 20
#endif

inline code *code_i(al *const a, size_t size) {
    code *c = ala(a, sizeof(code) + sizeof(op) * size);
    c->size = size;
    return c;
}

#ifndef CODE_R_SIZE
    #define CODE_R_SIZE 2
#endif

inline void code_a(al *const a, code **c, op o) {
    if ((*c)->len == (*c)->size) {
        code *nc = code_i(a, (*c)->size * CODE_R_SIZE);
        for (size_t i = 0; i < (*c)->len; i++) nc->ops[nc->len++] = (*c)->ops[i];
        alf(*c);
        *c = nc;
    }
    (*c)->ops[(*c)->len++] = o;
}

inline op_if *op_if_i(al *const a, size_t size) {
    op_if *of = ala(a, sizeof(op_if));
    of->cond = code_i(a, size);
    of->body = code_i(a, size);
    return of;
}

inline void op_if_f(op_if *of) {
    code_f(of->cond);
    code_f(of->body);
    alf(of);
}

void code_p(const code_st *const cs, const code *const c, size_t idnt);

#define OP_A(CS, C, OC, OT, OD, A) code_a(CS->a, C, (op) {OP_C(OC), TYPE(OT), 0, 0, (op_d) OD, A})

code_stat code_gen(code_st *const cs, const ast *const a, code **c);

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code **c);
