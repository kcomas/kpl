
#include "code.h"

extern inline void code_st_i(code_st *const cs, const char *str);

extern inline code *code_i(size_t size);

extern inline void code_a(code **c, op o);

extern inline void code_f(code *c);

static const char *op_c_str[] = {
    "EFN",
    "RFN",
    "AG",
    "SG",
    "LG",
    "AL",
    "FL",
    "SL",
    "LL",
    "SA",
    "LA",
    "PG",
    "PL",
    "PV",
    "JMPF",
    "CST"
};

const char *op_c_get_str(op_c oc) {
    const char *s = "INVALID";
    if (oc >= OP_C(EFN) && oc <= OP_C(CST)) s = op_c_str[oc];
    return s;
};

void code_p(const code_st *const cs, const code *const c, size_t idnt) {
    for (size_t i = 0; i < c->len; i++) {
        PCX(' ', idnt);
        printf("C:%s,T:%s", op_c_get_str(c->ops[i].oc), type_get_str(c->ops[i].ot));
        switch (c->ops[i].ot) {
            case TYPE(U3):
                printf(",%d", c->ops[i].od.u3);
            default:
                break; // TODO
        }
        if (c->ops[i].a) tkn_p(&c->ops[i].a->t, cs->str);
        // TODO jit
        putchar('\n');
    }
}

#define IFCGEN(FN, CS, A, C) if ((cstat = FN(CS, A, C)) != CODE_STAT(OK)) return cstat

static code_gen_lst(code_st *const cs, const lst_node *const lst, code *c) {
    code_stat cstat;
    lst_itm *h = lst->h;
    while (h) {
        IFCGEN(code_gen, cs, h->a, c);
        h = h->next;
    }
    return CODE_STAT(OK);
}

static code_gen_op(code_st *const cs, const ast *const a, code *c) {
    op_node *op = a->n.op;
    switch (op->ot) {
        case AST_TYPE(VAR):
            break;
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
       case AST_TYPE(LST): return code_gen_lst(cs, a->n.lst, c);
    }
    return CODE_STAT(OK);
}

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code *c) {
    code_stat cstat;
    if (fn->idc < fn->args->len) return CODE_STAT(ARG_LEN_GT_LOCAL_LEN);
    OP_A(c, EFN, VD, {}, NULL);
    uint8_t ngl = fn->idc - fn->args->len;
    if (ngl) {
        if (!fn->par) OP_A(c, AG, U3, { .u3 = ngl }, NULL);
        else OP_A(c, AL, U3, { .u3 = ngl }, NULL);
    }
    IFCGEN(code_gen_lst, cs, fn->body, c);
    if (ngl && fn->par) OP_A(c, FL, U3, { .u3 = ngl }, NULL);
    OP_A(c, RFN, VD, {}, NULL);
    return CODE_STAT(OK);
}
