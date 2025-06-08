
#include "type.h"

static const char *const tss[] = {
    "OK",
    "SELF_CALL_IN_MOD",
    "VAL_UT",
    "TC_FN_N_TC",
    "BLTS_INV_T",
    "BTTS_INV_T",
    "HSH_TBL_INS_F",
    "SYM_INV",
    "SYM_INV_A_T",
    "SYM_INV_TBL_R",
    "SYM_HSH_DATA_T_INV",
    "INV_TC_R",
    "TC_ER_N_STR_SG",
    "TC_ER_L_LST_INV",
    "TC_ER_R_NE",
    "INV_TC_NE_T",
    "TC_ER_L_H_N_VAR",
    "TC_VAR_FN_T_NEQ",
    "TC_ER_L_T_N_VAR",
    "TC_VAR_LT_N_T_ER",
    "INV_TC",
    "INV_ASS",
    "INV_ASS_ER",
    "INV_VAR_ASS",
    "VAR_ASS_N_T_M",
    "INV_SYM_ASS",
    "SYM_ASS_N_T_M",
    "INV_CALL_ASS",
    "CALL_ASS_N_T_M",
    "INV_ASS_TO",
    "INV_CST",
    "INV_CST_L_A",
    "INV_CST_L_T_N",
    "INV_CST_R_T_N",
    "INV_VR_T",
    "INV_VR_PUSH_R_T",
    "INV_VR_PUSH_T_NEQ",
    "INV_TE_2_VR",
    "INV_FN_CST",
    "FN_CST_T_NN",
    "INV_FN_CST_ARGS_LEN",
    "INV_DEL_L_NN",
    "INV_DEL_R_NG",
    "INV_LD_L_NN",
    "INV_LD_ME",
    "LD_MOD_TBL_AD_F",
    "INV_LD",
    "INV_VH_L_NN",
    "INV_VH_R_NG",
    "INV_ADD_L_T_N",
    "INV_ADD_R_T_N",
    "INV_ADD",
    "INV_UNARY_SUB_T",
    "INV_UNARY_SUB",
    "INV_SUB_L_T_N",
    "INV_SUB_R_T_N",
    "INV_SUB",
    "INV_MUL_L_T_N",
    "INV_MUL_R_T_N",
    "TD_TE_FN_T_NEQ",
    "INV_MUL",
    "INV_EQ_L_T_N",
    "INV_EQ_R_T_N",
    "INV_EQ",
    "INV_NOT_R_T_N",
    "INV_GT_L_T_N",
    "INV_GT_R_T_N",
    "INV_GT",
    "INV_LT",
    "INV_LT_L_T_N",
    "INV_LT_R_T_N",
    "INV_OR_L_T_N",
    "INV_OR_R_T_N",
    "INV_CNCT_L_T_N",
    "INV_CNCT_R_T_N",
    "INV_STR_CNCT",
    "INV_CNCT",
    "INV_RW_L_T_N",
    "INV_RW_R_T_N",
    "INV_RW",
    "INV_FN_ARG_T",
    "INV_FN_T_ARG",
    "INV_FN_T_RET",
    "INV_ARGS_OP_CALL",
    "INV_OP_CALL_LRR_N_N",
    "INV_CALL_TGT",
    "INV_CALL_TGT_T",
    "INV_TE_CALL",
    "INV_TE_CALL_IDX",
    "INV_TE_CALL_IDX_T",
    "INV_CALL_ARGS_LEN",
    "INV_CALL_TGT_ARG_T",
    "INV_CALL_ARG_T",
    "CALL_ARG_T_NEQ",
    "INV_RET_T",
    "INV_RET_FNS",
    "RET_T_NEQ",
    "INV_IRET_T",
    "INV_IRET_FNS",
    "IRET_T_NEQ",
    "VAR_UT",
    "TBL_FOUND",
    "MOD_FOUND"
};

const char *type_stat_str(type_stat tstat) {
    const char *s = "INVALID_TYPE_STAT";
    if (tstat >= TYPE_STAT(OK) && tstat <= TYPE_STAT(MOD_FOUND)) s = tss[tstat];
    return s;
}

extern inline void type_st_i(type_st *const ts, al *const a, er *const e, const char *mp);

extern inline type_stat type_er(type_st *const ts, const char *const fnn, type_stat tstat);

#define TYPE_ER(TS, TSTAT) type_er(TS, __func__, TYPE_STAT(TSTAT))

static bool type_eq(const type_node *const ta, const type_node *tb);

static bool lst_node_eq(const lst_node *const la, const lst_node *const lb) {
    if (la->len != lb->len) return false;
    type_node *ta, *tb;
    lst_itm *ha = la->h, *hb = lb->h;
    while (ha && hb) {
        if (!(ta = ast_gtn(ha->a))) return false;
        if (!(tb = ast_gtn(hb->a))) return false;
        if (!type_eq(ta, tb)) return false;
        ha = ha->next;
        hb = hb->next;
    }
    if (ha || hb) return false;
    return true;
}

static bool type_eq(const type_node *const ta, const type_node *tb) {
    if (!ta || !tb) return false;
    if ((ta->t == TYPE(SG) && tb->t == TYPE(STR)) || (ta->t == TYPE(STR) && tb->t == TYPE(SG))) return true;
    if (ta->t != tb->t) return false;
    switch (ta->t) {
        case TYPE(TE):
        case TYPE(TD):
            return lst_node_eq(ta->a->n.lst, tb->a->n.lst);
        default:
            break; // TODO specify all types
    }
    return true;
}

typedef bool type_eq_fn(const type_node *const ta, const type_node *const tb);

// all elements are the same type
static bool type_lst_contig(const type_node *const tlst, const type_node *const tn, type_eq_fn *const fn) {
    type_node *cmp;
    lst_itm *h = tlst->a->n.lst->h;
    while (h) {
        if (!(cmp = ast_gtn(h->a))) return false;
        if (!fn(cmp, tn)) return false;
        h = h->next;
    }
    return true;
}

#define IFTCHK(FN, TS, FNS, N) if ((tstat = FN(TS, FNS, N)) != TYPE_STAT(OK)) return tstat

#define ASTGTN(T, N, E) if (!(T = ast_gtn(N))) return TYPE_ER(ts, E);

static type_stat type_chk_lst(type_st *const ts, fn_node *const fns, lst_node *const lst) {
    type_stat tstat;
    lst_itm *h = lst->h;
    bool blts = false; // build list type sig
    type_node *bs;
    if (lst->tn->t == TYPE(TE) && !lst->tn->a) {
        blts = true;
        lst->tn->a = ast_i(ts->a, AST_TYPE(LST), (node) { .lst = lst_node_i(ts->a, TYPE(STMT)) }, NULL);
    }
    while (h) {
        if (lst->tn->t == TYPE(STMT) && (h != lst->t || !fns->par)) { // gc value on stack
            if (h->a->at == AST_TYPE(CALL)) h->a->n.cn->flgs |= NODE_FLG(GCR);
            if (h->a->at == AST_TYPE(OP)) h->a->n.op->flgs |= NODE_FLG(GCR);
        }
        IFTCHK(type_chk, ts, fns, h->a);
        if (blts) {
            ASTGTN(bs, h->a, BLTS_INV_T);
            lst_node_a(ts->a, lst->tn->a->n.lst, ast_i(ts->a, AST_TYPE(TYPE), (node) { .tn = type_node_c(ts->a, bs) }, NULL));
        }
        h = h->next;
    }
    return TYPE_ER(ts, OK);
}

static type_stat type_chk_hsh(type_st *const ts, fn_node *const fns, hsh_node *const hsh) {
    type_stat tstat;
    kv_itm *h = hsh->h;
    bool btts = false; // build table type sig
    type_node *bs;
    if (hsh->tn->t == TYPE(ST) && !hsh->tn->a) {
        btts = true;
        hsh->tn->a = ast_i(ts->a, AST_TYPE(TBL), (node) { .tl = tbl_i(ts->a, TBL_I_SIZE) }, NULL);
    }
    while (h) {
        IFTCHK(type_chk, ts, fns, h->a);
        if (btts) {
            tbl_itm *ti;
            ASTGTN(bs, h->a, BTTS_INV_T);
            hsh_data *hd = hsh_data_i(ts->a, h->id, type_node_c(ts->a, bs));
            if (tbl_op(ts->a, &hsh->tn->a->n.tl, h->k, hd, &ti, NULL, TBL_OP_FLG(AD)) != TBL_STAT(OK)) {
                hsh_data_f(hd);
                return TYPE_ER(ts, HSH_TBL_INS_F);
            }
        }
        h = h->next;
    }
    return TYPE_ER(ts, OK);
}

static type_stat type_chk_sym(type_st *const ts, fn_node *const fns, sym_node *const sym) {
    if (!sym->a) {
        sym->tn = type_node_i(ts->a, TYPE(SL), NULL);
        return TYPE_ER(ts, OK);
    }
    type_stat tstat;
    IFTCHK(type_chk, ts, fns, sym->a);
    type_node *tn;
    ASTGTN(tn, sym->a, SYM_INV_A_T);
    if (tn->t == TYPE(ST) && tn->a && tn->a->at == AST_TYPE(TBL)) {
        tbl_itm *ti;
        if (tbl_op(ts->a, &tn->a->n.tl, sym->s, NULL, &ti, NULL, TBL_OP_FLG(FD)) != TBL_STAT(OK)) return TYPE_ER(ts, SYM_INV_TBL_R);
        hsh_data *hd = (hsh_data*) ti->data;
        if (!hd->tn) return TYPE_ER(ts, SYM_HSH_DATA_T_INV);
        sym->tn = type_node_c(ts->a, hd->tn);
        return TYPE_ER(ts, OK);
    }
    // TODO tuple index
    return TYPE_ER(ts, SYM_INV);
}

static type_stat type_chk_if(type_st *const ts, fn_node *const fns, if_node *const in) {
    type_stat tstat;
    if_itm *h = in->h;
    while (h) {
        IFTCHK(type_chk, ts, fns, h->cond);
        IFTCHK(type_chk_lst, ts, fns, h->body);
        h = h->next;
    }
    return TYPE_ER(ts, OK);
}

static type_stat type_check_lop(type_st *const ts, fn_node *const fns, if_itm *const lop) {
    type_stat tstat;
    IFTCHK(type_chk, ts, fns, lop->cond);
    IFTCHK(type_chk_lst, ts, fns, lop->body);
    return TYPE_ER(ts, OK);
}

static bool type_int_is(const type_node *const tn, const type_node *const dnu) {
    (void) dnu;
    return tn->t >= TYPE(U3) && tn->t <= TYPE(I6);
}

static bool type_int_cor(type_st *const ts, type_node **tgt, const type_node *const a, const type_node *const b) {
    if (tgt && *tgt) return true;
    if ((a->t == TYPE(INT) && type_int_is(b, NULL)) || (type_int_is(a, NULL) && type_int_is(b, NULL) && a->t == b->t)) {
        if (tgt) *tgt = type_node_c(ts->a, b);
        return true;
    }
    if (a->t == TYPE(INT) && b->t == TYPE(INT)) {
        if (tgt) *tgt = type_node_i(ts->a, TYPE(I6), NULL);
        return true;
    }
    return false;
}

static bool type_str_is(const type_node *const tn, const type_node *const dnu) {
    (void) dnu;
    return tn->t == TYPE(STR) || tn->t == TYPE(SG);
}

static type_stat mod_tn_i(type_st *const ts, mod *const m) {
    tbl_itm *h = m->fns->tl->h;
    char vstr[MAX_VAR_LEN + 1]; // for `
    m->tn = type_node_i(ts->a, TYPE(ST), ast_i(ts->a, AST_TYPE(TBL), (node) { .tl = tbl_i(ts->a, TBL_I_SIZE) }, NULL));
    while (h) {
        var_node *var = (var_node*) h->data;
        if (var->flgs & NODE_FLG(VH)) {
            h = h->next;
            continue;
        }
        memset(vstr, '\0', MAX_VAR_LEN + 1);
        vstr[0] = '`';
        strcpy(vstr + 1, var->str);
        tbl_itm *ti;
        hsh_data *hd = hsh_data_i(ts->a, var->id, type_node_c(ts->a, var->tn));
        if (tbl_op(ts->a, &m->tn->a->n.tl, vstr, hd, &ti, NULL, TBL_OP_FLG(AD)) != TBL_STAT(OK)) {
            hsh_data_f(hd);
            return TYPE_ER(ts, LD_MOD_TBL_AD_F);
        }
        h = h->next;
    }
    return TYPE_ER(ts, OK);
}

#define ASTGTNBOP(OP) ASTGTN(lt, op->l, INV_##OP##_L_T_N); \
            ASTGTN(rt, op->r, INV_##OP##_R_T_N)

static type_stat type_chk_op(type_st *const ts, fn_node *const fns, op_node *const op) {
    type_stat tstat;
    type_node *lt = NULL, *ltvr = NULL, *rt = NULL, *tn = NULL;
    ast *atmp;
    lst_node *lst;
    ast_st ldas;
    type_st ldts;
    if (op->ot != OP_TYPE(TC) && op->ot != OP_TYPE(ASS)) if (op->l) IFTCHK(type_chk, ts, fns, op->l);
    if (op->ot != OP_TYPE(CST)) if (op->r) IFTCHK(type_chk, ts, fns, op->r);
    switch (op->ot) {
        case OP_TYPE(TC):
            if (!op->l) { // throw
                    if (!fn_node_tc(fns)) return TYPE_ER(ts, TC_FN_N_TC);
                    ASTGTN(rt, op->r, INV_TC_R);
                    if (!(rt->t == TYPE(STR) || rt->t == TYPE(SG))) return TYPE_ER(ts, TC_ER_N_STR_SG);
                    op->ret = type_node_i(ts->a, TYPE(ER), NULL);
                    break;
            } else { // catch
                if (op->l->at != AST_TYPE(LST)) return TYPE_ER(ts, TC_ER_L_LST_INV);
                lst = op->l->n.lst;
                ASTGTN(rt, op->r, INV_TC_R);
                if (rt->t != TYPE(ER)) return TYPE_ER(ts, TC_ER_R_NE);
                ASTGTN(tn, rt->a, INV_TC_NE_T);
                if (tn->t != TYPE(VD)) {
                    if (lst->len != 2) return TYPE_ER(ts, TC_ER_L_LST_INV);
                    if (lst->h->a->at != AST_TYPE(VAR)) return TYPE_ER(ts, TC_ER_L_H_N_VAR);
                    if (!lst->h->a->n.var->tn) {
                        lst->h->a->n.var->tn = type_node_c(ts->a, tn);
                        op->flgs |= NODE_FLG(GCV);
                    } else if (!type_eq(lst->h->a->n.var->tn, tn)) {
                        return TYPE_ER(ts, TC_VAR_FN_T_NEQ);
                    }
                } else if (lst->len != 1) return TYPE_ER(ts, TC_ER_L_LST_INV);
                if (lst->t->a->at != AST_TYPE(VAR)) return TYPE_ER(ts, TC_ER_L_T_N_VAR);
                if (!lst->t->a->n.var->tn) {
                    lst->t->a->n.var->tn = type_node_i(ts->a, TYPE(ER), NULL);
                } else if (lst->t->a->n.var->tn->t != TYPE(ER)) {
                    return TYPE_ER(ts, TC_VAR_LT_N_T_ER);
                }
                rt->flgs |= NODE_FLG(EC);
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            }
            return TYPE_ER(ts, INV_TC);
        case OP_TYPE(ASS):
            if (!op->r) return TYPE_ER(ts, INV_ASS);
            ASTGTN(rt, op->r, INV_ASS);
            if (rt->t == TYPE(ER)) ASTGTN(rt, rt->a, INV_ASS_ER);
            if (op->l->at == AST_TYPE(VAR)) {
                if (op->l->n.var->tn) {
                    ASTGTN(lt, op->l, INV_VAR_ASS);
                    if (!type_eq(lt, rt)) return TYPE_ER(ts, VAR_ASS_N_T_M);
                    op->flgs |= NODE_FLG(GCV);
                } else op->l->n.var->tn = rt->t == TYPE(STR) ? type_node_i(ts->a, TYPE(SG), NULL) : type_node_c(ts->a, rt);
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            } else if (op->l->at == AST_TYPE(SYM)) {
                if (!op->l->n.sym->a) return TYPE_ER(ts, INV_SYM_ASS);
                IFTCHK(type_chk, ts, fns, op->l);
                ASTGTN(lt, op->l, INV_SYM_ASS);
                if (!type_eq(lt, rt)) return TYPE_ER(ts, SYM_ASS_N_T_M);
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            } else if (op->l->at == AST_TYPE(CALL)) {
                ASTGTN(lt, op->l->n.cn->tgt, INV_CALL_ASS);
                if (lt->t != TYPE(TE)) return TYPE_ER(ts, INV_CALL_ASS);
                IFTCHK(type_chk, ts, fns, op->l);
                if (!type_eq(op->l->n.cn->ret, rt)) return TYPE_ER(ts, CALL_ASS_N_T_M);
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            }
            return TYPE_ER(ts, INV_ASS_TO);
        case OP_TYPE(CST):
            if (!(op->l->at == AST_TYPE(TYPE) || op->l->at == AST_TYPE(VAR))) return TYPE_ER(ts, INV_CST_L_A);
            ASTGTN(lt, op->l, INV_CST_L_T_N);
            if (lt->t == TYPE(FN)) {
                if (op->r->at != AST_TYPE(FN)) return TYPE_ER(ts, INV_FN_CST);
                fn_node *fn = op->r->n.fn;
                if (fn->sig) return TYPE_ER(ts, FN_CST_T_NN);
                if (fn->args->len != lt->a->n.lst->len - 1) return TYPE_ER(ts, INV_FN_CST_ARGS_LEN);
                lst_itm *th = lt->a->n.lst->h, *fh = fn->args->h;
                type_node *tmpt = NULL;
                while (fh) {
                    if (fh->a->at != AST_TYPE(VAR)) return TYPE_ER(ts, INV_FN_ARG_T);
                    ASTGTN(tmpt, th->a, INV_FN_T_ARG);
                    fh->a->n.var->tn = type_node_c(ts->a, tmpt);
                    fh = fh->next;
                    th = th->next;
                }
                fn->sig = type_node_c(ts->a, lt);
                op->ret = type_node_c(ts->a, lt);
                return type_chk_fn(ts, fn);
            }
            if (op->r) IFTCHK(type_chk, ts, fns, op->r);
            ASTGTN(rt, op->r, INV_CST_R_T_N);
            if (lt->t == TYPE(VR) && rt->t == TYPE(TE)) {
                ASTGTN(ltvr, lt->a, INV_VR_T);
                if (!type_lst_contig(rt, ltvr, type_eq)) return TYPE_ER(ts, INV_TE_2_VR);
                rt->t = TYPE(VR);
                op->ret = type_node_c(ts->a, lt);
                break;
            }
            if (type_int_cor(ts, &op->ret, rt, lt)) break;
            if (lt->t == TYPE(SG) || lt->t == TYPE(FD)) {
                op->ret = type_node_i(ts->a, lt->t, NULL);
                break;
            }
            // TODO cst
            return TYPE_ER(ts, INV_CST);
        // TODO ops
        case OP_TYPE(DEL):
            if (op->l) return TYPE_ER(ts, INV_DEL_L_NN);
            if (op->r->at == AST_TYPE(VAR) && op->r->n.var->vt != VAR_TYPE(G)) return TYPE_ER(ts, INV_DEL_R_NG);
            op->ret = type_node_i(ts->a, TYPE(VD), NULL);
            break;
        case OP_TYPE(LD):
            if (op->l) return TYPE_ER(ts, INV_LD_L_NN);
            if (op->r->at == AST_TYPE(VAL) && op->r->n.val->tn->t == TYPE(STR)) {
                mod *m = mod_i(ts->a, ts->e);
                if (mod_lfile_tkn(m, ts->mp, str_dir_len(ts->mp), &op->r->t) != MOD_STAT(OK)) return TYPE_ER(ts, INV_LD_ME);
                m->fns = fn_node_i(ts->a, NULL);
                m->fns->sig = type_node_i(ts->a, TYPE(MOD), NULL);
                ast_st_i(&ldas, ts->a, ts->e, m->src.str);
                if (ast_parse_stmts(&ldas, m->fns, m->fns->body, TFLS, TKN_FLG(NB)) != AST_STAT(END)) return TYPE_ER(ts, INV_LD_ME);
                type_st_i(&ldts, ts->a, ts->e, m->src.path);
                if (type_chk_fn(&ldts, m->fns) != TYPE_STAT(OK)) return TYPE_ER(ts, INV_LD_ME);
                if ((tstat = mod_tn_i(ts, m)) != TYPE_STAT(OK)) return tstat;
                atmp = op->r;
                op->r = ast_i(ts->a, AST_TYPE(MOD), (node) { .m = m }, &op->r->t);
                ast_f(atmp);
                op->ret = type_node_c(ts->a, op->r->n.m->tn);
                break;
            }
            return TYPE_ER(ts, INV_LD);
        case OP_TYPE(VH):
            if (op->l) return TYPE_ER(ts, INV_VH_L_NN);
            if (op->r->at == AST_TYPE(VAR) && op->r->n.var->vt != VAR_TYPE(G)) return TYPE_ER(ts, INV_VH_R_NG);
            op->r->n.var->flgs |= NODE_FLG(VH);
            op->ret = type_node_i(ts->a, TYPE(VD), NULL);
            break;
        case OP_TYPE(ADD):
            ASTGTNBOP(ADD);
            if (lt->t == TYPE(VR)) {
                ASTGTN(ltvr, lt->a, INV_VR_T);
                ASTGTN(rt, op->r, INV_VR_PUSH_R_T);
                if (!type_eq(ltvr, rt)) return TYPE_ER(ts, INV_VR_PUSH_T_NEQ);
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            }
            if (type_int_cor(ts, &op->ret, lt, rt) || type_int_cor(ts, &op->ret, rt, lt)) break;
            return TYPE_ER(ts, INV_ADD);
        case OP_TYPE(SUB):
            if (!op->l) {
                ASTGTN(rt, op->r, INV_UNARY_SUB_T);
                if (rt->t == TYPE(VR)) {
                    ASTGTN(rt, rt->a, INV_VR_T);
                    op->ret = type_node_i(ts->a, TYPE(ER), ast_i(ts->a, AST_TYPE(TYPE), (node) { .tn = type_node_c(ts->a, rt) }, &op->r->t));
                    break;
                }
                // TODO negate
                return TYPE_ER(ts, INV_UNARY_SUB);
            }
            ASTGTNBOP(SUB);
            if (type_int_cor(ts, &op->ret, lt, rt) || type_int_cor(ts, &op->ret, rt, lt)) break;
            // TODO
            return TYPE_ER(ts, INV_SUB);
        case OP_TYPE(MUL):
            ASTGTNBOP(MUL);
            if (lt->t == TYPE(TE) && rt->t == TYPE(FN)) {
                if (!lst_node_eq(lt->a->n.lst, rt->a->n.lst)) return TYPE_ER(ts, TD_TE_FN_T_NEQ);
                op->ret = type_lst_i(ts->a, TYPE(TD), 2, lt, rt);
                break;
            }
            if (type_int_cor(ts, &op->ret, lt, rt) || type_int_cor(ts, &op->ret, rt, lt)) break;
            return TYPE_ER(ts, INV_MUL);
        case OP_TYPE(EQ):
            ASTGTNBOP(EQ);
            if (type_int_cor(ts, NULL, lt, rt) || type_int_cor(ts, NULL, rt, lt)) {
                if (lt->t != TYPE(INT)) op->ret = type_node_i(ts->a, lt->t, NULL);
                else op->ret = type_node_i(ts->a, rt->t, NULL);
                break;
            }
            // TODO
            return TYPE_ER(ts, INV_EQ);
        case OP_TYPE(NOT):
            // TODO check left
            ASTGTN(rt, op->r, INV_NOT_R_T_N);
            // TODO check for val
            op->ret = type_node_i(ts->a, TYPE(BL), NULL);
            break;
        // TODO ops
        case OP_TYPE(GT):
            ASTGTNBOP(GT);
            if (type_int_cor(ts, &op->ret, lt, rt) || type_int_cor(ts, &op->ret, rt, lt)) break;
            op->ret = type_node_i(ts->a, TYPE(BL), NULL);
            return TYPE_ER(ts, INV_GT);
        case OP_TYPE(LT):
            ASTGTNBOP(LT);
            if (type_int_cor(ts, &op->ret, lt, rt) || type_int_cor(ts, &op->ret, rt, lt)) break;
            op->ret = type_node_i(ts->a, TYPE(BL), NULL);
            return TYPE_ER(ts, INV_LT);
        case OP_TYPE(OR):
            ASTGTNBOP(OR);
            op->ret = type_node_i(ts->a, TYPE(BL), NULL);
            break;
        case OP_TYPE(CNCT):
            ASTGTNBOP(CNCT);
            if (type_str_is(lt, NULL)) {
                if (!type_str_is(rt, NULL)) {
                    // TODO check if vr of sg or cr
                    if (rt->t == TYPE(TE)) {
                        if (!type_lst_contig(rt, NULL, type_str_is)) return TYPE_ER(ts, INV_STR_CNCT);
                    } else if (rt->t == TYPE(VR)) {
                        ASTGTN(rt, rt->a, INV_STR_CNCT);
                        if (rt->t != TYPE(SG)) return TYPE_ER(ts, INV_STR_CNCT);
                    } else {
                        return TYPE_ER(ts, INV_STR_CNCT);
                    }
                }
                op->ret = type_node_i(ts->a, TYPE(SG), NULL);
                break;
            }
            // TODO
            return TYPE_ER(ts, INV_CNCT);
        case OP_TYPE(RW):
            ASTGTNBOP(RW);
            if (lt->t == TYPE(FD)) {
                op->ret = type_node_i(ts->a, TYPE(VD), NULL);
                break;
            }
            // TODO
            return TYPE_ER(ts, INV_RW);

    }
    return TYPE_STAT(OK);
}

type_stat type_chk_call(type_st *const ts, fn_node *const fns, call_node *const cn) {
    type_stat tstat;
    if (!cn->tgt) {
        if (cn->args->len > 2) return TYPE_ER(ts, INV_ARGS_OP_CALL);
        op_node *op = cn->args->h->a->n.op;
        if (op->ret || op->l || op->r) return TYPE_ER(ts, INV_OP_CALL_LRR_N_N);
        op->r = cn->args->h->next->a;
        if ((tstat = type_chk_op(ts, fns, op)) != TYPE_STAT(OK)) return tstat;
        cn->ret = op->ret;
        op->ret = NULL;
        op->l = op->r = NULL;
        return TYPE_ER(ts, OK);
    } else if (cn->tgt->at == AST_TYPE(OP)) {
        if (cn->args->len > 2) return TYPE_ER(ts, INV_ARGS_OP_CALL);
        op_node *op = cn->tgt->n.op;
        if (op->ret || op->l || op->r) return TYPE_ER(ts, INV_OP_CALL_LRR_N_N);
        if (cn->args->h) {
            op->l = cn->args->h->a;
            if (cn->args->h->next) op->r = cn->args->h->next->a;
        }
        if ((tstat = type_chk_op(ts, fns, op)) != TYPE_STAT(OK)) return tstat;
        cn->ret = op->ret;
        op->ret = NULL;
        op->l = op->r = NULL;
        return TYPE_ER(ts, OK);
    }
    IFTCHK(type_chk, ts, fns, cn->tgt);
    IFTCHK(type_chk_lst, ts, fns, cn->args);
    type_node *tt, *ta;
    ASTGTN(tt, cn->tgt, INV_CALL_TGT);
    lst_itm *th, *ah;
    int64_t tidx;
    switch (tt->t) {
        case TYPE(TE):
            if (cn->args->len != 1 && cn->args->h->a->at != AST_TYPE(VAL) && cn->args->h->a->n.val->tn->t != TYPE(INT)) return TYPE_ER(ts, INV_TE_CALL);
            tidx = tkn_to_int64_t(&cn->args->h->a->t);
            if (tidx < 0 || tidx > ((int64_t) tt->a->n.lst->len - 1)) return TYPE_ER(ts, INV_TE_CALL_IDX);
            th = tt->a->n.lst->h;
            while (tidx > 0) {
               tidx--;
               th = th->next;
            }
            ASTGTN(tt, th->a, INV_TE_CALL_IDX_T);
            cn->ret = type_node_c(ts->a, tt);
            break;
        case TYPE(FN):
            if (tt->a->n.lst->len - 1 != cn->args->len) return TYPE_ER(ts, INV_CALL_ARGS_LEN);
            cn->ret = type_node_c(ts->a, tt->a->n.lst->t->a->n.tn);
            th = tt->a->n.lst->h;
            ah = cn->args->h;
            while (ah) {
                ASTGTN(tt, th->a, INV_CALL_TGT_ARG_T);
                ASTGTN(ta, ah->a, INV_CALL_ARG_T);
                if (!((ta->t == TYPE(INT) && type_int_is(tt, NULL)) || (type_eq(tt, ta)))) return TYPE_ER(ts, CALL_ARG_T_NEQ);
                th = th->next;
                ah = ah->next;
            }
            break;
        default:
            return TYPE_ER(ts, INV_CALL_TGT_T);
    }
    return TYPE_ER(ts, OK);
}

static type_stat type_chk_ret(type_st *const ts, fn_node *const fns, ret_node *const ret) {
    type_stat tstat;
    IFTCHK(type_chk, ts, fns, ret->a);
    type_node *tmpr, *tmpf;
    ASTGTN(tmpr, ret->a, INV_RET_T);
    if (!(tmpf = fn_node_ret_type(fns))) return TYPE_ER(ts, INV_RET_FNS);
    if (tmpf->t == TYPE(VD)) return TYPE_ER(ts, OK);
    if (type_int_cor(ts, &ret->tn, tmpr, tmpf)) return TYPE_ER(ts, OK);
    if (!type_eq(tmpf, tmpr)) return TYPE_ER(ts, RET_T_NEQ);
    ret->tn = type_node_c(ts->a, tmpf);
    return TYPE_ER(ts, OK);
}

type_stat type_chk(type_st *const ts, fn_node *const fns, ast *const a) {
    switch (a->at) {
        case AST_TYPE(TYPE): break;
        case AST_TYPE(RES):
            if (a->n.rn->rt == RES_TYPE(SELF) && !a->n.rn->tn) {
                if (!fns->par) return TYPE_ER(ts, SELF_CALL_IN_MOD);
                a->n.rn->tn = type_node_c(ts->a, fns->sig);
            }
            break;
        case AST_TYPE(VAL):
            if (a->n.val->tn->t >= TYPE(STMT) && a->n.val->tn->t <= TYPE(MOD)) return TYPE_ER(ts, VAL_UT);
            break;
        case AST_TYPE(OP): return type_chk_op(ts, fns, a->n.op);
        case AST_TYPE(LST): return type_chk_lst(ts, fns, a->n.lst);
        case AST_TYPE(HSH): return type_chk_hsh(ts, fns, a->n.hsh);
        case AST_TYPE(TBL): return TYPE_ER(ts, TBL_FOUND);
        case AST_TYPE(SYM): return type_chk_sym(ts, fns, a->n.sym);
        case AST_TYPE(IF): return type_chk_if(ts, fns, a->n.in);
        case AST_TYPE(LOP): return type_check_lop(ts, fns, a->n.lop);
        case AST_TYPE(MOD): return TYPE_ER(ts, MOD_FOUND);
        case AST_TYPE(FN): return type_chk_fn(ts, a->n.fn);
        case AST_TYPE(CALL): return type_chk_call(ts, fns, a->n.cn);
        case AST_TYPE(RET): return type_chk_ret(ts, fns, a->n.ret);
        case AST_TYPE(VAR):
            if (!a->n.var->tn) return TYPE_ER(ts, VAR_UT);
            break;
    }
    return TYPE_ER(ts, OK);
}

type_stat type_chk_fn(type_st *const ts, fn_node *const fn) {
    type_stat tstat;
    IFTCHK(type_chk_lst, ts, fn, fn->args);
    IFTCHK(type_chk_lst, ts, fn, fn->body);
    if (fn->par) {
        type_node *tmpr, *tmpf;
        ASTGTN(tmpr, fn->body->t->a, INV_IRET_T);
        if (!(tmpf = fn_node_ret_type(fn))) return TYPE_ER(ts, INV_IRET_FNS);
        if (tmpf->t == TYPE(VD)) return TYPE_ER(ts, OK);
        if (!type_eq(tmpf, tmpr)) return TYPE_ER(ts, IRET_T_NEQ);
    }
    return TYPE_ER(ts, OK);
}
