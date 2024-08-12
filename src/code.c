
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
    "CST",
    "ADD"
};

const char *op_c_get_str(op_c oc) {
    const char *s = "INVALID";
    if (oc >= OP_C(EFN) && oc <= OP_C(ADD)) s = op_c_str[oc];
    return s;
};

void code_p(const code_st *const cs, const code *const c, size_t idnt) {
    for (size_t i = 0; i < c->len; i++) {
        PCX(' ', idnt);
        printf("C:%s,T:%s", op_c_get_str(c->ops[i].oc), type_get_str(c->ops[i].ot));
        switch (c->ops[i].ot) {
            // TODO
            case TYPE(STMT):
                printf(",%s", type_get_str(c->ops[i].od.t));
                break;
            case TYPE(U3):
                printf(",%d", c->ops[i].od.u3);
                break;
            case TYPE(U6):
                printf(",%lu", c->ops[i].od.u6);
                break;
            case TYPE(FN):
                putchar('\n');
                code_p(cs, c->ops[i].od.c, idnt + 4);
                PCX(' ', idnt + 4);
                break;
            default:
                break; // TODO
        }
        if (c->ops[i].a) {
            putchar('|');
            tkn_p(&c->ops[i].a->t, cs->str);
            putchar('|');
        }
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

#ifndef MAX_INT_LEN
    #define MAX_INT_LEN 20
#endif

static int64_t tkn_to_int64_t(const tkn *const t, const char *const str) {
    char istr[MAX_INT_LEN], *eptr;
    memset(istr, '\0', MAX_INT_LEN);
    memcpy(istr, str + t->pos, t->len);
    return strtoll(istr, &eptr, 10);
}

static code_stat p_int(const code_st *const cs, type t, const ast *const a, code *c) {
    int64_t i = tkn_to_int64_t(&a->t, cs->str);
    switch (t) {
        // TODO
        case TYPE(U6):
            OP_A(c, PV, U6, { .u6 = (uint64_t) i }, a);
            break;
        default: return CODE_STAT(INV_INT_CST_PUSH);
    }
    return CODE_STAT(OK);
}

#define OP_P_INT(OPN, CS, TGT, C) if (OPN->TGT->at == AST_TYPE(VAL) && OPN->TGT->n.val->tn->t == TYPE(INT)) if ((cstat = p_int(cs, OPN->ret->t, OPN->TGT, C)) != CODE_STAT(OK)) return cstat

static code_gen_op(code_st *const cs, const ast *const a, code *c) {
    code_stat cstat;
    op_node *opn = a->n.op;
    var_node *var;
    if (opn->ot != OP_TYPE(ASS)) if (opn->l) IFCGEN(code_gen, cs, opn->l, c);
    if (opn->ot != OP_TYPE(CST)) if (opn->r) IFCGEN(code_gen, cs, opn->r, c);
    switch (opn->ot) {
        case OP_TYPE(ASS):
            var = opn->l->n.var;
            switch (var->vt) {
                case VAR_TYPE(U): return CODE_STAT(VAR_TYPE_U);
                case VAR_TYPE(G):
                    OP_A(c, SG, U3, { .u3 = var->id }, opn->l);
                    break;
                case VAR_TYPE(L):
                    OP_A(c, SL, U3, { .u3 = a->n.var->fns->idc - a->n.var->id }, a);
                    break;
                case VAR_TYPE(A):
                    OP_A(c, SA, U3, { .u3 = a->n.var->id }, a);
                    break;

            }
            break;
        case OP_TYPE(CST):
            OP_P_INT(opn, cs, r, c);
            if (opn->r->at == AST_TYPE(FN)) return code_gen(cs, opn->r, c);
            // TODO dynamic cast
            break;
        case OP_TYPE(ADD):
            OP_P_INT(opn, cs, l, c);
            OP_P_INT(opn, cs, r, c);
            OP_A(c, ADD, STMT, { .t = opn->ret->t }, a);
            break;
    }
    return CODE_STAT(OK);
}

code_stat code_gen(code_st *const cs, const ast *const a, code *c) {
    code_stat cstat;
    code *cfn;
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
        case AST_TYPE(IF):
            // TODO
            break;
        case AST_TYPE(FN):
            cfn = code_i(CODE_I_SIZE);
            IFCGEN(code_gen_fn, cs, a->n.fn, cfn);
            OP_A(c, PV, FN, { .c = cfn }, a);
            break;
        // TODO
        case AST_TYPE(VAR):
            switch (a->n.var->vt) {
                case VAR_TYPE(U): return CODE_STAT(VAR_TYPE_U);
                case VAR_TYPE(G):
                    OP_A(c, LG, U3, { .u3 = a->n.var->id }, a);
                    break;
                case VAR_TYPE(L):
                    OP_A(c, LL, U3, { .u3 = a->n.var->fns->idc - a->n.var->id }, a);
                    break;
                case VAR_TYPE(A):
                    OP_A(c, LA, U3, { .u3 = a->n.var->id }, a);
                    break;
            }
            break;
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
