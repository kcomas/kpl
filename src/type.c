
#include "type.h"

#define IFTCHK(FN, FNS, N) if ((tstat = FN(FNS, N)) != TYPE_STAT(OK)) return tstat

static type_stat type_chk_lst(fn_node *const fns, lst_node *const lst) {
    type_stat tstat;
    lst_itm *h = lst->h;
    while (h) {
        IFTCHK(type_chk, fns, h->a);
        h = h->next;
    }
    return TYPE_STAT(OK);
}

static type_stat type_chk_op(fn_node *const fns, op_node *const op) {
    type_stat tstat;
    type_node *lt = NULL, *rt = NULL;
    if (op->l) IFTCHK(type_chk, fns, op->l);
    if (op->r) IFTCHK(type_chk, fns, op->r);
    switch (op->ot) {
        case OP_TYPE(ASS):
            if (op->l->at == AST_TYPE(VAR)) {
                if (!op->r) return TYPE_STAT(INV_VAR_ASS);
                if (op->l->n.var->tn) {
                    // TODO type check
                } else {
                    if (!(rt = ast_gtn(op->r))) return TYPE_STAT(INV_VAR_ASS);
                    op->ret = type_node_c(rt);
                    op->l->n.var->tn = type_node_c(rt);
                }
                return TYPE_STAT(OK);
            }
            return TYPE_STAT(INV_ASS_TO);
        case OP_TYPE(CST):
            if (!(op->l->at == AST_TYPE(TYPE) || op->l->at == AST_TYPE(VAR))) return TYPE_STAT(INV_CST_L_A);
            if (!(lt = ast_gtn(op->l))) return TYPE_STAT(INV_CST_L_T_N);
            if (!(rt = ast_gtn(op->r))) return TYPE_STAT(INV_CST_R_T_N);
            if (rt->t == TYPE(INT)) {
                if (lt->t >= TYPE(U3) && lt->t <= TYPE(I6)) {
                    op->ret = type_node_c(lt);
                    break;
                }
            }
            // TODO resolve
            exit(345);
            break;
    }
    return TYPE_STAT(OK);
}

type_stat type_chk(fn_node *const fns, ast *const a) {
    switch (a->at) {
        case AST_TYPE(TYPE): break;
        case AST_TYPE(VAL):
            if (a->n.val->tn->t >= TYPE(STMT) && a->n.val->tn->t <= TYPE(MOD)) return TYPE_STAT(VAL_UT);
            break;
        case AST_TYPE(OP): return type_chk_op(fns, a->n.op);
        case AST_TYPE(LST): return type_chk_lst(fns, a->n.lst);
    }
    return TYPE_STAT(OK);
}

type_stat type_chk_fn(fn_node *const fn) {
    type_stat tstat;
    IFTCHK(type_chk_lst, fn, fn->args);
    IFTCHK(type_chk_lst, fn, fn->body);
    return TYPE_STAT(OK);
}
