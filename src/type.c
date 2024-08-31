
#include "type.h"

static bool type_eq(const type_node *const ta, const type_node *tb) {
    if (!ta || !tb) return false;
    if (ta->t != tb->t) return false;
    // TODO deep check
    return true;
}

typedef bool type_eq_fn(const type_node *const ta, const type_node *const tb);

static bool type_lst_contig(const lst_node *const lst, const type_node *const tn, type_eq_fn *const fn) {
    type_node *cmp;
    lst_itm *h = lst->h;
    while (h) {
        if (!(cmp = ast_gtn(h->a))) return false;
        if (!fn(cmp, tn)) return false;
        h = h->next;
    }
    return true;
}

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

static type_stat type_check_lop(fn_node *const fns, if_itm *const lop) {
    type_stat tstat;
    IFTCHK(type_chk, fns, lop->cond);
    IFTCHK(type_chk_lst, fns, lop->body);
    return TYPE_STAT(OK);
}

static bool type_int_is(const type_node *const tn, const type_node *const dnu) {
    (void) dnu;
    return tn->t >= TYPE(U3) && tn->t <= TYPE(I6);
}

static bool type_int_cor(type_node **tgt, const type_node *const a, const type_node *const b) {
    if (tgt && *tgt) return true;
    if ((a->t == TYPE(INT) && type_int_is(b, NULL)) || (type_int_is(a, NULL) && type_int_is(b, NULL) && a->t == b->t)) {
        if (tgt) *tgt = type_node_c(b);
        return true;
    }
    if (a->t == TYPE(INT) && b->t == TYPE(INT)) {
        if (tgt) *tgt = type_node_i(TYPE(I6), NULL);
        return true;
    }
    return false;
}

static bool type_str_is(const type_node *const tn, const type_node *const dnu) {
    (void) dnu;
    return tn->t == TYPE(STR) || tn->t == TYPE(SG);
}

#define ASTGTN(T, N, E) if (!(T = ast_gtn(N))) return TYPE_STAT(E);

#define ASTGTNBOP(OP) ASTGTN(lt, op->l, INV_##OP##_L_T_N); \
            ASTGTN(rt, op->r, INV_##OP##_R_T_N)

static type_stat type_chk_op(fn_node *const fns, op_node *const op) {
    type_stat tstat;
    type_node *lt = NULL, *rt = NULL;
    if (op->ot != OP_TYPE(ASS)) if (op->l) IFTCHK(type_chk, fns, op->l);
    if (op->ot != OP_TYPE(CST)) if (op->r) IFTCHK(type_chk, fns, op->r);
    switch (op->ot) {
        case OP_TYPE(ASS):
            if (op->l->at == AST_TYPE(VAR)) {
                if (!op->r) return TYPE_STAT(INV_VAR_ASS);
                ASTGTN(rt, op->r, INV_VAR_ASS);
                if (op->l->n.var->tn) {
                    ASTGTN(lt, op->l, INV_VAR_ASS);
                    if (!type_eq(lt, rt)) return TYPE_STAT(VAR_ASS_N_T_M);
                } else op->l->n.var->tn = rt->t == TYPE(STR) ? type_node_i(TYPE(SG), NULL) : type_node_c(rt);
                op->ret = rt->t == TYPE(STR) ? type_node_i(TYPE(SG), NULL) : type_node_c(rt);
                break;
            }
            return TYPE_STAT(INV_ASS_TO);
        case OP_TYPE(CST):
            if (!(op->l->at == AST_TYPE(TYPE) || op->l->at == AST_TYPE(VAR))) return TYPE_STAT(INV_CST_L_A);
            ASTGTN(lt, op->l, INV_CST_L_T_N);
            if (lt->t == TYPE(FN)) {
                if (op->r->at != AST_TYPE(FN)) return TYPE_STAT(INV_FN_CST);
                fn_node *fn = op->r->n.fn;
                if (fn->sig) return TYPE_STAT(FN_CST_T_NN);
                if (fn->args->len != lt->a->n.lst->len - 1) return TYPE_STAT(INV_FN_CST_ARGS_LEN);
                lst_itm *th = lt->a->n.lst->h, *fh = fn->args->h;
                type_node *tmpt = NULL;
                while (fh) {
                    if (fh->a->at != AST_TYPE(VAR)) return TYPE_STAT(INV_FN_ARG_T);
                    ASTGTN(tmpt, th->a, INV_FN_T_ARG);
                    fh->a->n.var->tn = type_node_c(tmpt);
                    fh = fh->next;
                    th = th->next;
                }
                fn->sig = type_node_c(lt);
                op->ret = type_node_c(lt);
                return type_chk_fn(fn);
            }
            if (op->r) IFTCHK(type_chk, fns, op->r);
            ASTGTN(rt, op->r, INV_CST_R_T_N);
            if (type_int_cor(&op->ret, rt, lt)) break;
            if (lt->t == TYPE(SG)) {
                op->ret = type_node_i(TYPE(SG), NULL);
                break;
            }
            if (lt->t == TYPE(FD)) {
                op->ret = type_node_i(TYPE(FD), NULL);
                break;
            }
            // TODO cst
            return TYPE_STAT(INV_CST);
        // TODO ops
        case OP_TYPE(ADD):
            ASTGTNBOP(ADD);
            if (type_int_cor(&op->ret, lt, rt) || type_int_cor(&op->ret, rt, lt)) break;
            return TYPE_STAT(INV_ADD);
        case OP_TYPE(SUB):
            // TODO negate
            ASTGTNBOP(SUB);
            if (type_int_cor(&op->ret, lt, rt) || type_int_cor(&op->ret, rt, lt)) break;
            // TODO
            return TYPE_STAT(INV_SUB);
        case OP_TYPE(EQ):
            ASTGTNBOP(EQ);
            if (type_int_cor(NULL, lt, rt) || type_int_cor(NULL, rt, lt)) {
                if (lt->t != TYPE(INT)) op->ret = type_node_i(lt->t, NULL);
                else op->ret = type_node_i(rt->t, NULL);
                break;
            }
            // TODO
            return TYPE_STAT(INV_EQ);
        case OP_TYPE(NOT):
            // TODO check left
            ASTGTN(rt, op->r, INV_NOT_R_T_N);
            // TODO check for val
            op->ret = type_node_i(TYPE(BL), NULL);
            break;
            // TODO ops
        case OP_TYPE(GT):
            ASTGTNBOP(GT);
            if (type_int_cor(&op->ret, lt, rt) || type_int_cor(&op->ret, rt, lt)) break;
            // TODO
            return TYPE_STAT(INV_GT);
        case OP_TYPE(LT):
            ASTGTNBOP(LT);
            if (type_int_cor(&op->ret, lt, rt) || type_int_cor(&op->ret, rt, lt)) break;
            // TODO
            return TYPE_STAT(INV_LT);
        case OP_TYPE(OR):
            ASTGTNBOP(OR);
            op->ret = type_node_i(TYPE(BL), NULL);
            break;
        case OP_TYPE(CNCT):
            ASTGTNBOP(CNCT);
            if (type_str_is(lt, NULL)) {
                if (!type_str_is(rt, NULL)) {
                    // TODO check if vr of sg or cr
                    if (rt->t == TYPE(TE))
                        if (!type_lst_contig(op->r->n.lst, NULL, type_str_is)) return TYPE_STAT(INV_STR_CNCT);
                }
                op->ret = type_node_i(TYPE(SG), NULL);
                break;
            }
            // TODO
            return TYPE_STAT(INV_CNCT);
        case OP_TYPE(RW):
            ASTGTNBOP(RW);
            if (lt->t == TYPE(FD)) {
                op->ret = type_node_i(TYPE(VD), NULL);
                break;
            }
            // TODO
            return TYPE_STAT(INV_RW);

    }
    return TYPE_STAT(OK);
}

type_stat type_chk_call(fn_node *const fns, call_node *const cn) {
    type_stat tstat;
    if (cn->tgt->at == AST_TYPE(OP)) {
        if (cn->args->len > 2) return TYPE_STAT(INV_ARGS_OP_CALL);
        op_node *op = cn->tgt->n.op;
        if (op->ret || op->l || op->r) return TYPE_STAT(INV_OP_CALL_LRR_N_N);
        if (cn->args->h) {
            op->l = cn->args->h->a;
            if (cn->args->h->next) op->r = cn->args->h->next->a;
        }
        if ((tstat = type_chk_op(fns, op)) != TYPE_STAT(OK)) return tstat;
        cn->ret = op->ret;
        op->ret = NULL;
        op->l = op->r = NULL;
        return TYPE_STAT(OK);
    }
    IFTCHK(type_chk, fns, cn->tgt);
    IFTCHK(type_chk_lst, fns, cn->args);
    type_node *tt, *ta;
    ASTGTN(tt, cn->tgt, INV_CALL_TGT);
    lst_itm *th, *ah;
    switch (tt->t) {
        // TODO vr and hh
        case TYPE(FN):
            if (tt->a->n.lst->len - 1 != cn->args->len) return TYPE_STAT(INV_CALL_ARGS_LEN);
            cn->ret = type_node_c(tt->a->n.lst->t->a->n.tn);
            th = tt->a->n.lst->h;
            ah = cn->args->h;
            while (ah) {
                ASTGTN(tt, th->a, INV_CALL_TGT_ARG_T);
                ASTGTN(ta, ah->a, INV_CALL_ARG_T);
                if (!((ta->t == TYPE(INT) && type_int_is(tt, NULL)) || (type_eq(tt, ta)))) return TYPE_STAT(CALL_ARG_T_NEQ);
                th = th->next;
                ah = ah->next;
            }
            break;
        default:
            return TYPE_STAT(INV_CALL_TGT_T);
    }
    return TYPE_STAT(OK);
}

type_stat type_chk_ret(fn_node *const fns, ret_node *const ret) {
    type_stat tstat;
    IFTCHK(type_chk, fns, ret->a);
    type_node *tmpr, *tmpf;
    ASTGTN(tmpr, ret->a, INV_RET_T);
    if (!(tmpf = fn_node_ret_type(fns))) return TYPE_STAT(INV_RET_FNS);
    if (tmpf->t == TYPE(VD)) return TYPE_STAT(OK);
    if (type_int_cor(&ret->tn, tmpr, tmpf)) return TYPE_STAT(OK);
    if (!type_eq(tmpf, tmpr)) return TYPE_STAT(RET_T_NEQ);
    ret->tn = type_node_c(tmpf);
    return TYPE_STAT(OK);
}

type_stat type_chk(fn_node *const fns, ast *const a) {
    switch (a->at) {
        case AST_TYPE(TYPE): break;
        case AST_TYPE(RES):
            if (a->n.rn->rt == RES_TYPE(SELF) && !a->n.rn->tn) {
                // TODO do not allow in mod
                a->n.rn->tn = type_node_c(fns->sig);
            }
            break;
        case AST_TYPE(VAL):
            if (a->n.val->tn->t >= TYPE(STMT) && a->n.val->tn->t <= TYPE(MOD)) return TYPE_STAT(VAL_UT);
            break;
        case AST_TYPE(OP): return type_chk_op(fns, a->n.op);
        case AST_TYPE(LST): return type_chk_lst(fns, a->n.lst);
        case AST_TYPE(IF): return type_chk_if(fns, a->n.in);
        case AST_TYPE(LOP): return type_check_lop(fns, a->n.lop);
        case AST_TYPE(FN): return type_chk_fn(a->n.fn);
        case AST_TYPE(CALL): return type_chk_call(fns, a->n.cn);
        case AST_TYPE(RET): return type_chk_ret(fns, a->n.ret);
        case AST_TYPE(VAR):
            if (!a->n.var->tn) return TYPE_STAT(VAR_UT);
            break;
    }
    return TYPE_STAT(OK);
}

type_stat type_chk_fn(fn_node *const fn) {
    type_stat tstat;
    IFTCHK(type_chk_lst, fn, fn->args);
    IFTCHK(type_chk_lst, fn, fn->body);
    // TODO check last stmt type
    if (fn->par) {
        type_node *tmpr, *tmpf;
        ASTGTN(tmpr, fn->body->t->a, INV_IRET_T);
        if (!(tmpf = fn_node_ret_type(fn))) return TYPE_STAT(INV_IRET_FNS);
        if (tmpf->t == TYPE(VD)) return TYPE_STAT(OK);
        if (!type_eq(tmpf, tmpr)) return TYPE_STAT(IRET_T_NEQ);
    }
    return TYPE_STAT(OK);
}
