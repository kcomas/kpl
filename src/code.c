
#include "code.h"

extern inline void code_st_i(code_st *const cs, const char *str);

extern inline cte *cte_i(size_t len, code *const gc);

#define CODE_F_T(T, FN, P) if (c->ops[i].ot == TYPE(T)) FN(c->ops[i].od.P)

void code_f(code *c) {
    for (size_t i = 0; i < c->len; i++) {
        CODE_F_T(FN, code_f, c);
        CODE_F_T(IF, code_f, c);
        CODE_F_T(COND, op_if_f, of);
        CODE_F_T(LOP, op_if_f, of);
        CODE_F_T(SG, free, sg);
        CODE_F_T(STR, free, sg);
        CODE_F_T(TE, cte_f, te);
    }
    free(c);
}

extern inline void cte_f(cte *te);

extern inline code *code_i(size_t size);

extern inline void code_a(code **c, op o);

extern inline op_if *op_if_i(size_t size);

extern inline void op_if_f(op_if *of);

static const char *op_c_str[] = {
    "EFN",
    "PUSH",
    "RFN",
    "CFN",
    "CS",
    "AG",
    "SG",
    "LG",
    "AL",
    "FL",
    "SL",
    "LL",
    "SA",
    "LA",
    "PV",
    "CTE",
    "IF",
    "COND",
    "LOP",
    "ZOO",
    "CST",
    "CSTSG",
    "ADD",
    "SUB",
    "EQ",
    "NOT",
    "GT",
    "LT",
    "OR",
    "CNCTSG",
    "WFD",
    "GC",
    "GCTEI"
};

const char *op_c_get_str(op_c oc) {
    const char *s = "INVALID";
    if (oc >= OP_C(EFN) && oc <= OP_C(GCTEI)) s = op_c_str[oc];
    return s;
};

void code_p(const code_st *const cs, const code *const c, size_t idnt) {
    for (size_t i = 0; i < c->len; i++) {
        PCX(' ', idnt);
        printf("%lu: C:%s,T:%s", i, op_c_get_str(c->ops[i].oc), type_get_str(c->ops[i].ot));
        switch (c->ops[i].ot) {
            // TODO
            case TYPE(OP):
                printf(",%s", type_get_str(c->ops[i].od.t));
                break;
            case TYPE(IF):
                putchar('\n');
                code_p(cs, c->ops[i].od.c, idnt + 1);
                PCX(' ', idnt + 1);
                break;
            case TYPE(COND):
            case TYPE(LOP):
                putchar('\n');
                code_p(cs, c->ops[i].od.of->cond, idnt + 1);
                code_p(cs, c->ops[i].od.of->body, idnt + 2);
                PCX(' ', idnt + 1);
                break;
            case TYPE(VAR):
                printf(",%d", c->ops[i].od.v.id);
                printf(",%s", type_get_str(c->ops[i].od.v.t));
                break;
            case TYPE(CODE):
                printf(",%s", type_get_str(c->ops[i].od.t));
                break;
            case TYPE(U3):
                printf(",%d", c->ops[i].od.u3);
                break;
            case TYPE(U6):
                printf(",%lu", c->ops[i].od.u6);
                break;
            // TODO
            case TYPE(I6):
                printf(",%ld", c->ops[i].od.i6);
                break;
            case TYPE(STR):
            case TYPE(SG):
                printf(",%s", c->ops[i].od.sg);
                break;
            case TYPE(TE):
                printf(",%lu", c->ops[i].od.te->len);
                putchar('\n');
                code_p(cs, c->ops[i].od.te->gc, idnt + 4);
                break;
            case TYPE(FD):
                printf(",%d", c->ops[i].od.fd);
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
        if (c->ops[i].jlen > 0) printf("$%d,%d", c->ops[i].jidx, c->ops[i].jlen);
        putchar('\n');
    }
}

static code_stat code_gen_gc(const type_node *const tn, const ast *const a, code **c, int16_t id) {
    op_c oc = OP_C(GC);
    type t = TYPE(OP);
    op_d od = (op_d) { .t = tn->t };
    if (id >= 0) {
        oc = OP_C(GCTEI);
        t = TYPE(VAR);
        od = (op_d) { SLV((uint8_t) id, tn->t) };
    }
    switch (tn->t) {
        case TYPE(STR):
        case TYPE(U3):
        case TYPE(U4):
        case TYPE(U5):
        case TYPE(U6):
        case TYPE(I3):
        case TYPE(I4):
        case TYPE(I5):
        case TYPE(I6):
        case TYPE(SG):
        case TYPE(TE):
            // TODO idx in TE
            code_a(c, (op) {oc, t, 0, 0, od,  a});
            break;
        default:
            return CODE_STAT(GC_INV);
    }
    return CODE_STAT(OK);
}

#define OP_GC(C, TN, A) if ((cstat = code_gen_gc(TN, A, C, -1)) != CODE_STAT(OK)) return cstat;

#define OP_GCTEI(C, TN, A, I) if ((cstat = code_gen_gc(TN, A, C, I)) != CODE_STAT(OK)) return cstat;

#define IFCGEN(FN, CS, A, C) if ((cstat = FN(CS, A, C)) != CODE_STAT(OK)) return cstat

static code_stat code_gen_lst(code_st *const cs, const lst_node *const lst, code **c) {
    code_stat cstat;
    lst_itm *h = lst->h;
    code *gc;
    type_node *th;
    int16_t id = -1;
    if (lst->tn->t == TYPE(TE)) {
        gc = code_i(CODE_I_SIZE);
        OP_A(&gc, EFN, CODE, { .t = TYPE(VD) }, NULL);
        OP_A(&gc, LA, VAR, { SLV(0, TYPE(TE)) }, NULL);
    }
    while (h) {
        IFCGEN(code_gen, cs, h->a, c);
        if (lst->tn->t == TYPE(TE)) {
            if (!(th = ast_gtn(h->a))) return CODE_STAT(NO_T_FOR_TE_IDX);
            OP_GCTEI(&gc, th, h->a, ++id);
        }
        h = h->next;
    }
    if (lst->tn->t == TYPE(TE)) {
        OP_A(&gc, RFN, CODE, { .t = TYPE(VD) }, NULL);
        OP_A(c, CTE, TE, { .te = cte_i(lst->len, gc) }, NULL);
    }
    return CODE_STAT(OK);
}

#define OP_ZOO(TN, C) if (TN->t != TYPE(BL)) OP_A(C, ZOO, OP, { .t = TN->t }, a);

static code_stat code_gen_if(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    code *ifc = code_i(CODE_I_SIZE);
    if_itm *h = a->n.in->h;
    type_node *tc;
    while (h) {
        op_if *of = op_if_i(CODE_I_SIZE);
        if (h->cond) {
            IFCGEN(code_gen, cs, h->cond, &of->cond);
            if (!(tc = ast_gtn(h->cond))) return CODE_STAT(NO_T_FOR_IF_COND);
            OP_ZOO(tc, &of->cond);
        }
        IFCGEN(code_gen_lst, cs, h->body, &of->body);
        OP_A(&ifc, COND, COND, { .of = of }, a);
        h = h->next;
    }
    OP_A(c, IF, IF, { .c = ifc }, a);
    return CODE_STAT(OK);
}

static code_stat code_gen_lop(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    op_if *of = op_if_i(CODE_I_SIZE);
    type_node *tc;
    IFCGEN(code_gen, cs, a->n.lop->cond, &of->cond);
    if (!(tc = ast_gtn(a->n.lop->cond))) return CODE_STAT(NO_T_FOR_LOP_COND);
    IFCGEN(code_gen_lst, cs, a->n.lop->body, &of->body);
    OP_A(c, LOP, LOP, { .of = of }, a);
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

static code_stat p_int(const code_st *const cs, type t, const ast *const a, code **c) {
    int64_t i = tkn_to_int64_t(&a->t, cs->str);
    switch (t) {
        // TODO
        case TYPE(U6):
            OP_A(c, PV, U6, { .u6 = (uint64_t) i }, a);
            break;
        // TODO
        case TYPE(I6):
            OP_A(c, PV, I6, { .i6 = i }, a);
            break;
        default: return CODE_STAT(INV_INT_CST_PUSH);
    }
    return CODE_STAT(OK);
}

#define OP_P_INT_RET(OPN, CS, TGT, C) if (OPN->TGT->at == AST_TYPE(VAL) && OPN->TGT->n.val->tn->t == TYPE(INT) && (cstat = p_int(cs, OPN->ret->t, OPN->TGT, C)) != CODE_STAT(OK)) return cstat

static code_stat cor_int(const code_st *const cs, const ast *const a, const ast *const b, code **c) {
    if (a->at != AST_TYPE(VAL)) return CODE_STAT(OK);
    if (a->n.val->tn->t != TYPE(INT)) return CODE_STAT(OK);
    if (b->at == AST_TYPE(VAL) && b->n.val->tn->t == TYPE(INT)) return p_int(cs, TYPE(I6), a, c);
    type_node *tb;
    if (!(tb = ast_gtn(b))) return CODE_STAT(NO_TYPE_COR_INT);
    return p_int(cs, tb->t, a, c);
}

#define OP_P_INT_COR(CS, A, B, C) if ((cstat = cor_int(CS, A, B, C)) != CODE_STAT(OK)) return cstat;

static code_stat code_gen_op(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    op_node *opn = a->n.op;
    var_node *var;
    type_node *tl, *tr;
    int64_t i6;
    switch (opn->ot) {
        case OP_TYPE(ASS):
            IFCGEN(code_gen, cs, opn->r, c);
            if (opn->l->at == AST_TYPE(VAR)) {
                var = opn->l->n.var;
                switch (var->vt) {
                    case VAR_TYPE(U): return CODE_STAT(VAR_TYPE_U);
                    case VAR_TYPE(G):
                        OP_A(c, SG, VAR, { SLV(var->id, opn->ret->t) }, opn->l);
                        break;
                    case VAR_TYPE(L):
                        OP_A(c, SL, VAR, { SLV(a->n.var->fns->idc - a->n.var->id, opn->ret->t) }, a);
                        break;
                    case VAR_TYPE(A):
                        OP_A(c, SA, VAR, { SLV(a->n.var->id, opn->ret->t) }, a);
                        break;
                }
                break;
            }
            return CODE_STAT(INV_L_ASS);
        case OP_TYPE(CST):
            IFCGEN(code_gen, cs, opn->l, c);
            if (opn->r->at == AST_TYPE(FN)) return code_gen(cs, opn->r, c);
            // TODO dynamic cast
            switch (opn->l->n.tn->t) {
                case TYPE(U3):
                case TYPE(U4):
                case TYPE(U5):
                case TYPE(U6):
                case TYPE(I3):
                case TYPE(I4):
                case TYPE(I5):
                case TYPE(I6):
                    if (opn->r->at == AST_TYPE(VAL) && opn->r->n.val->tn->t == TYPE(INT)) return p_int(cs, opn->l->n.tn->t, opn->r, c);
                    // TODO dynamic case
                    break;
                // TODO
                case TYPE(SG):
                    IFCGEN(code_gen, cs, opn->r, c);
                    if (!(tr = ast_gtn(opn->r))) return CODE_STAT(OP_NO_T_R);
                    if (tr->t >= TYPE(U3) && tr->t <= TYPE(F6)) {
                        OP_A(c, CSTSG, OP, { .t = tr->t }, a);
                        return CODE_STAT(OK);
                    }
                    return CODE_STAT(INV_CST_SG);
                case TYPE(FD):
                        if (opn->r->at == AST_TYPE(VAL) && opn->r->n.val->tn->t == TYPE(INT)) {
                            i6 = tkn_to_int64_t(&opn->r->t, cs->str);
                            if (i6 >= 0 && i6 <= 2) {
                                OP_A(c, PV, FD, { .fd = (int) i6 }, opn->r);
                                return CODE_STAT(OK);
                            }
                        return CODE_STAT(INV_CST_INT_TO_FD);
                    }
                    return CODE_STAT(INV_CST_FD);
                default:
                    break;
            }
            return CODE_STAT(INV_CST);
        case OP_TYPE(ADD):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(c, ADD, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(SUB):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(c, SUB, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(EQ):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_COR(cs, opn->l, opn->r, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_COR(cs, opn->r, opn->l, c);
            OP_A(c, EQ, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(NOT):
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_STAT(OP_NO_T_R);
            OP_ZOO(tr, c);
            OP_A(c, NOT, OP, { .t = TYPE(BL) }, a);
            break;
        case OP_TYPE(GT):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(c, GT, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(LT):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(c, LT, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(OR):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_STAT(OP_NO_T_L);
            OP_ZOO(tl, c);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_STAT(OP_NO_T_R);
            OP_ZOO(tr, c);
            OP_A(c, OR, OP, { .t = TYPE(BL) }, a);
            break;
        case OP_TYPE(CNCT):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_STAT(OP_NO_T_L);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_STAT(OP_NO_T_R);
            switch  (opn->ret->t) {
                case TYPE(SG):
                    if (tr->t == TYPE(TE) || tr->t == TYPE(STR) || tr->t == TYPE(SG)) {
                        OP_A(c, CNCTSG, OP, { .t = tr->t }, a);
                        OP_GC(c, tr, opn->r);
                        OP_GC(c, tl, opn->l);
                    } else return CODE_STAT(INV_SG_CNCT);
                    break;
                default: return CODE_STAT(INV_CNCT_OP);
            }
            break;
        case OP_TYPE(RW):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_STAT(OP_NO_T_L);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_STAT(OP_NO_T_R);
            if (tl->t == TYPE(FD) && tr->t != TYPE(FD)) {
                OP_A(c, WFD, OP, { .t = tr->t }, a);
                OP_GC(c, tr, opn->r);
            }
            else return CODE_STAT(INV_FD_OP);
            break;
    }
    return CODE_STAT(OK);
}

code_stat code_gen_call(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    call_node *cn = a->n.cn;
    op_node *opn;
    type_node *tn;
    lst_itm *ch, *ah, *ct;
    switch (cn->tgt->at) {
        case AST_TYPE(RES):
            // TODO get fn type sig for self
            if (cn->tgt->n.rn->rt != RES_TYPE(SELF)) return CODE_STAT(CALL_RES_NOT_SELF);
            IFCGEN(code_gen_lst, cs, cn->args, c);
            OP_A(c, CS, OP, { .t = cn->ret->t }, a);
            break;
        case AST_TYPE(OP):
            opn = cn->tgt->n.op;
            if (cn->ret) opn->ret = cn->ret;
            if (cn->args->h) {
                opn->l = cn->args->h->a;
                if (cn->args->h->next) opn->r = cn->args->h->next->a;
            }
            IFCGEN(code_gen_op, cs, cn->tgt, c);
            opn->ret = NULL;
            opn->l = opn->r = NULL;
            break;
        case AST_TYPE(VAR):
            if (cn->tgt->n.var->tn->t != TYPE(FN)) return CODE_STAT(CALL_VAR_N_FN);
            ch = cn->tgt->n.var->tn->a->n.lst->h;
            ah = cn->args->h;
            while (ah) {
                IFCGEN(code_gen, cs, ah->a, c);
                OP_P_INT_COR(cs, ah->a, ch->a, c);
                ah = ah->next;
                ch = ch->next;
            }
            IFCGEN(code_gen, cs, cn->tgt, c);
            OP_A(c, CFN, OP, { .t = cn->ret->t }, a);
            ct = cn->tgt->n.var->tn->a->n.lst->t->prev;
            while (ct) {
                if (!(tn = ast_gtn(ct->a))) return CODE_STAT(CALL_CT_ARG_T_GC_INV);
                OP_A(c, GC, OP, { .t = tn->t }, ct->a);
                ct = ct->prev;
            }
            if (cn->ret->t != TYPE(VD)) OP_A(c, PUSH, U3, { .u3 = 0 }, a); // TODO xmm
            break;
        default:
            return CODE_STAT(INV_CALL_TGT);
    }
    return CODE_STAT(OK);
}

code_stat code_gen(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    code *cfn;
    char *sg;
    size_t sgi = 0;
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
                    break; // TODO
                case TYPE(STR):
                    sg = calloc(sizeof(char), a->t.len - 1);
                    for (size_t i = 0; i < a->t.len - 2; i++) {
                        if (cs->str[a->t.pos + 1 + i] == '\\') {
                            i++;
                            switch (cs->str[a->t.pos + 1 + i]) {
                                case 'n':
                                    sg[sgi++] = '\n';
                                    break;
                                default:
                                    return CODE_STAT(INV_STR_ESC);
                            }
                            break;
                        } else sg[sgi++] = cs->str[a->t.pos + 1 + i];
                    }
                    OP_A(c, PV, STR, { .sg = sg }, a);
                    break;
                default:
                    return CODE_STAT(NO_OP_FOR_VAL_T);
            }
            break;
        case AST_TYPE(OP): return code_gen_op(cs, a, c);
        case AST_TYPE(LST): return code_gen_lst(cs, a->n.lst, c);
        case AST_TYPE(IF): return code_gen_if(cs, a, c);
        case AST_TYPE(LOP): return code_gen_lop(cs, a, c);
        case AST_TYPE(FN):
            cfn = code_i(CODE_I_SIZE);
            IFCGEN(code_gen_fn, cs, a->n.fn, &cfn);
            OP_A(c, PV, FN, { .c = cfn }, a);
            break;
        case AST_TYPE(CALL): return code_gen_call(cs, a, c);
        case AST_TYPE(RET):
            if (a->n.ret->a->at == AST_TYPE(VAL)) {
                switch (a->n.ret->a->n.val->tn->t) {
                    case TYPE(INT):
                        if ((cstat = p_int(cs, TYPE(I6), a->n.ret->a, c)) != CODE_STAT(OK)) return cstat;
                        break;
                    case TYPE(FLT):
                    case TYPE(STR):
                        break; // TODO
                    default:
                        return CODE_STAT(NO_OP_FOR_RET_VAL_T);
                }
            } else IFCGEN(code_gen, cs, a->n.ret->a, c);
            OP_A(c, RFN, VD, {}, a);
            break;
        case AST_TYPE(VAR):
            switch (a->n.var->vt) {
                case VAR_TYPE(U): return CODE_STAT(VAR_TYPE_U);
                case VAR_TYPE(G):
                    OP_A(c, LG, VAR, { SLV(a->n.var->id, a->n.var->tn->t) }, a);
                    break;
                case VAR_TYPE(L):
                    OP_A(c, LL, VAR, { SLV(a->n.var->fns->idc - a->n.var->id, a->n.var->tn->t) }, a);
                    break;
                case VAR_TYPE(A):
                    // reverse count to get stack
                    OP_A(c, LA, VAR, { SLV(a->n.var->fns->args->len - 1 - a->n.var->id, a->n.var->tn->t) }, a);
                    break;
            }
            break;
    }
    return CODE_STAT(OK);
}

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code **c) {
    code_stat cstat;
    if (fn->idc < fn->args->len) return CODE_STAT(ARG_LEN_GT_LOCAL_LEN);
    OP_A(c, EFN, CODE, { .t = TYPE(VD) }, NULL);
    uint8_t ngl = fn->idc - fn->args->len;
    if (ngl) {
        if (!fn->par) OP_A(c, AG, U3, { .u3 = ngl }, NULL);
        else OP_A(c, AL, U3, { .u3 = ngl }, NULL);
    }
    IFCGEN(code_gen_lst, cs, fn->body, c);
    if (ngl && fn->par) {
        // TODO gc locals
        OP_A(c, FL, U3, { .u3 = ngl }, NULL);
    }
    type t = TYPE(VD);
    type_node *tn;
    if (fn->sig->t == TYPE(FN)) {
        if (!(tn = ast_gtn(fn->sig->a->n.lst->t->a))) return CODE_STAT(FN_RET_T_INV);
        t = tn->t;
    }
    OP_A(c, RFN, CODE, { .t = t }, NULL);
    return CODE_STAT(OK);
}
