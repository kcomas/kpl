
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

static type_stat type_chk_if(fn_node *const fns, if_node *const in) {
    type_stat tstat;
    if_itm *h = in->h;
    while (h) {
        IFTCHK(type_chk, fns, h->cond);
        IFTCHK(type_chk_lst, fns, h->body);
        h = h->next;
    }
    return TYPE_STAT(OK);
}

#define ASTGTN(T, N, E) if (!(T = ast_gtn(N))) return TYPE_STAT(E);

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
                    ASTGTN(rt, op->r, INV_VAR_ASS);
                    op->ret = type_node_c(rt);
                    op->l->n.var->tn = type_node_c(rt);
                }
                return TYPE_STAT(OK);
            }
            return TYPE_STAT(INV_ASS_TO);
        case OP_TYPE(CST):
            if (!(op->l->at == AST_TYPE(TYPE) || op->l->at == AST_TYPE(VAR))) return TYPE_STAT(INV_CST_L_A);
            ASTGTN(lt, op->l, INV_CST_L_T_N);
            if (lt->t == TYPE(FN)) {
                if (op->r->at != AST_TYPE(FN)) return TYPE_STAT(INV_FN_CST);
                fn_node *fn = op->r->n.fn;
                if (fn->ret) return TYPE_STAT(FN_CST_T_NN);
                if (fn->args->len != lt->a->n.lst->len - 1) return TYPE_STAT(INV_FN_CST_ARGS_LEN);
                lst_itm *th = lt->a->n.lst->h;
                lst_itm *fh = fn->args->h;
                type_node *tmpt = NULL;
                while (fh) {
                    if (fh->a->at != AST_TYPE(VAR)) return TYPE_STAT(INV_FN_ARG_T);
                    ASTGTN(tmpt, th->a, INV_FN_T_ARG);
                    fh->a->n.var->tn = type_node_c(tmpt);
                    fh = fh->next;
                    th = th->next;
                }
                ASTGTN(tmpt, lt->a->n.lst->t->a, INV_FN_T_RET);
                fn->ret = type_node_c(tmpt);
                return type_chk_fn(fn);
            }
            ASTGTN(rt, op->r, INV_CST_R_T_N);
            if (rt->t == TYPE(INT)) {
                if (lt->t >= TYPE(U3) && lt->t <= TYPE(I6)) {
                    op->ret = type_node_c(lt);
                    break;
                }
            }
            // TODO cst
            break;
        // TODO ops
        case OP_TYPE(EQ):
            // TODO
            break;
        case OP_TYPE(NOT):
            // TODO check left
            ASTGTN(rt, op->r, INV_NOT_R_T_N);
            op->ret = type_node_c(rt);
            break;
        // TODO ops
        case OP_TYPE(OR):
            // TODO
            break;

    }
    return TYPE_STAT(OK);
}

type_stat type_chk_call(fn_node *const fns, call_node *const cn) {
    type_stat tstat;
    IFTCHK(type_chk, fns, cn->tgt);
    IFTCHK(type_chk_lst, fns, cn->args);
    if (cn->tgt->at == AST_TYPE(OP)) {
        if (cn->args->len > 2) return TYPE_STAT(INV_ARGS_OP_CALL);
        op_node *op = cn->tgt->n.op;
        if (op->ret || op->l || op->r) return TYPE_STAT(INV_OP_CALL_LRR_N_N);
        if ((tstat = type_chk_op(fns, op)) != TYPE_STAT(OK)) return tstat;
        op->l = cn->args->h->a;
        op->r = cn->args->h->next->a;
        cn->ret = op->ret;
        op->ret = NULL;
        op->l = op->r = NULL;
        return TYPE_STAT(OK);
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
        case AST_TYPE(IF): return type_chk_if(fns, a->n.in);
        case AST_TYPE(FN): return type_chk_fn(a->n.fn);
        case AST_TYPE(CALL): return type_chk_call(fns, a->n.cn);
    }
    return TYPE_STAT(OK);
}

type_stat type_chk_fn(fn_node *const fn) {
    type_stat tstat;
    IFTCHK(type_chk_lst, fn, fn->args);
    IFTCHK(type_chk_lst, fn, fn->body);
    return TYPE_STAT(OK);
}
