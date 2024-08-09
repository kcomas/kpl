
#include "code.h"

extern inline code *code_i(size_t size);

extern inline void code_a(code **c, op o);

#define IFCGEN(FN, CS, A, C) if ((cstat = FN(CS, A, C)) != CODE_STAT(OK)) return cstat

static code_stat code_gen_lst(code_st *const cs, const ast *const alst, code *c) {
    code_stat cstat;
    lst_itm *h = alst->n.lst->h;
    while (h) {
        IFCGEN(code_gen, cs, h->a, c);
        h = h->next;
    }
    return CODE_STAT(OK);
}

static code_stat code_gen_op(code_st *const cs, const ast *const aop, code *c) {
    const op_node *op = aop->n.op;
    // TODO check rl
    switch (op->ot) {

    }
    return CODE_STAT(OK);
}

code_stat code_gen(code_st *const cs, const ast *const a, code *c) {
    switch (a->at) {
        case AST_TYPE(TYPE): break;
        case AST_TYPE(RES):
            switch (a->n.rn->rt) {
                case RES_TYPE(TRUE):
                case RES_TYPE(FALSE):
                case RES_TYPE(SELF):
                    break; // TODO
            }
            break;
        case AST_TYPE(VAL):
            switch (a->n.val->tn->t) {
                case TYPE(INT):
                case TYPE(FLT):
                case TYPE(STR):
                    break; // TODO
                default:
                    return CODE_STAT(NO_OP_FOR_VAL_T);
            }
            break;
       case AST_TYPE(OP): return code_gen_op(cs, a, c);
    }
    return CODE_STAT(OK);
}

code_stat code_gen_fn(code_st *const cs, const ast *const afn, code *c) {
    const fn_node *fn = afn->n.fn;
    if (fn->args->len > fn->idc) return CODE_STAT(ARG_LEN_GT_LOCAL_LEN);
    uint8_t nl = fn->idc - fn->args->len;
    OP_A(c, EFNAL, U3, { .u3 = nl }, afn);
    // TODO body
    OP_A(c, RFNFL, U3, { .u3 = nl }, afn);
    return CODE_STAT(OK);
}
