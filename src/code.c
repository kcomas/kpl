
#include "code.h"

static const char *const css[] = {
    "OK",
    "INV_ER_T",
    "INV_ER_GCR",
    "ER_N_ER_T",
    "INV_TC",
    "INV_L_ASS",
    "INV_R_ASS",
    "INV_STR_ESC",
    "NO_OP_FOR_VAL_T",
    "TBL_FOUND",
    "MOD_FOUND",
    "NO_T_FOR_TE_IDX",
    "NO_T_FOR_VR_GC",
    "NO_T_FOR_ST_IDX",
    "SYM_NO_T_FOR_A",
    "SYM_INV_TBL_R",
    "SYM_INV",
    "NO_T_FOR_IF_COND",
    "NO_T_FOR_LOP_COND",
    "ARG_LEN_GT_LOCAL_LEN",
    "FN_RET_T_INV",
    "FN_RET_ER_T_INV",
    "TC_R_N",
    "ASS_R_N",
    "ASS_TE_INV",
    "INV_TYPE_STORE_VD",
    "VAR_TYPE_U",
    "INV_INT_CST_PUSH",
    "INV_CST_INT_TO_FD",
    "INV_CST_SG",
    "NO_T_VR_GC",
    "INV_CST_FD",
    "INV_CST",
    "NO_TYPE_COR_INT",
    "GC_INV",
    "OP_NO_T_L",
    "OP_NO_T_R",
    "LD_MOD_F",
    "INV_ADD_T_L",
    "INV_SUB_T_R",
    "INV_SUB_VR_T_R",
    "INV_SG_CNCT",
    "INV_CNCT_OP",
    "INV_FD_OP",
    "CALL_RES_NOT_SELF",
    "CALL_T_N_FN",
    "CALL_T_ER_T_INV",
    "CALL_CT_ARG_T_GC_INV",
    "INV_CALL_TGT",
    "NO_OP_FOR_RET_VAL_T"
};

const char *code_stat_str(code_stat cstat) {
    const char *s = "INVALID_CODE_STAT";
    if (cstat >= CODE_STAT(OK) && cstat <= CODE_STAT(NO_OP_FOR_RET_VAL_T)) s = css[cstat];
    return s;
}

extern inline void code_st_i(code_st *const cs, al *const a, er *const e, const char *str);

extern inline code_stat code_er(code_st *const cs, const char *const fnn, code_stat cstat, const ast *const a);

#define CODE_ER(CS, CSTAT, A) code_er(CS, __func__, CODE_STAT(CSTAT), A)

extern inline ctsv *ctsv_i(al *const a, size_t len, mod *const m, code *const gc);

#define CODE_F_T(T, FN, P) case TYPE(T): \
    FN(c->ops[i].od.P); \
    break

void code_f(code *c) {
    for (size_t i = 0; i < c->len; i++) {
        switch (c->ops[i].ot) {
            case TYPE(MOD):
                FNNF(c->ops[i].od.tsv->m->j, jit_f);
                code_f(c->ops[i].od.tsv->m->c);
                ctsv_f(c->ops[i].od.tsv);
                break;
            CODE_F_T(FN, code_f, c);
            CODE_F_T(IF, code_f, c);
            CODE_F_T(COND, op_if_f, of);
            CODE_F_T(LOP, op_if_f, of);
            CODE_F_T(SG, alf, sg);
            CODE_F_T(STR, alf, sg);
            CODE_F_T(VR, ctsv_f, tsv);
            CODE_F_T(TE, ctsv_f, tsv);
            CODE_F_T(ST, ctsv_f, tsv);
            default:
                break;
        }
    }
    alf(c);
}

extern inline void ctsv_f(ctsv *tsv);

extern inline code *code_i(al *const a, size_t size);

extern inline void code_a(al *const a, code **c, op o);

extern inline op_if *op_if_i(al *const a, size_t size);

extern inline void op_if_f(op_if *of);

static const char *op_c_str[] = {
    "EFN", // enter fn
    "PUSH",
    "POP",
    "SWAP",
    "RFN", // return fn
    "CFN", // call fn
    "CS", // call self
    // data
    "AG", // allocate globals
    "SG", // store global
    "LG", // load global
    "LM", // load module
    "AL", // allocate locals
    "SL", // store local
    "LL", // load local
    "SA", // store arg
    "LA", // load arg
    "PV", // push value
    "CTSV", // create tuple from stack u6 is length
    "GIDX", // get index te, vr, st u6 is i
    "SIDX", // set idx te, vr, st u6 is i
    "VRA", // vector add, push
    "VRS", // vector sub, pop
    // control
    "IF",
    "COND", // jmp if false
    "LOP",
    // coalesce
    "ZOO", // convert to zero or one
    // error
    "TE", // throw error
    "CE", // catch error
    "PE", // panic error
    // ops
    "CST",
    "CSTSG", // cast type to string
    "ADD",
    "SUB",
    "MUL",
    "EQ",
    "NOT",
    "GT",
    "LT",
    "OR",
    "CNCTSG", // sg cnct op type is either sg or te
    "WFD", // OP_T is type to be written
    // GC
    "RCI",
    "RCD",
    "RCF", // dec ref count of type ret if gt 0
    "GC", // type is base type
    "GCTSVI", // gc idx in tsv
    "GCVR", // gc vr of type
    "DEL" // delete top of stack free ptr
};

const char *op_c_get_str(op_c oc) {
    const char *s = "INVALID";
    if (oc >= OP_C(EFN) && oc <= OP_C(DEL)) s = op_c_str[oc];
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
            case TYPE(MOD):
            case TYPE(VR):
            case TYPE(TE):
            case TYPE(ST):
                if (c->ops[i].od.tsv->len > 0) printf(",%lu", c->ops[i].od.tsv->len);
                if (c->ops[i].od.tsv->m) printf(",%p", c->ops[i].od.tsv->m);
                putchar('\n');
                if (c->ops[i].od.tsv->gc) code_p(cs, c->ops[i].od.tsv->gc, idnt + 4);
                break;
            case TYPE(FD):
                printf(",%d", c->ops[i].od.fd);
                break;
            case TYPE(FN):
                putchar('\n');
                code_p(cs, c->ops[i].od.c, idnt + 4);
                PCX(' ', idnt + 4);
                break;
            case TYPE(ER):
                printf(",%s", type_get_str(c->ops[i].od.e.t));
                if (c->ops[i].od.e.ec) printf(",EC");
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

static code_stat code_gen_rc(code_st *const cs, const type_node *const tn, code **c, bool inc) {
    switch (tn->t) {
        case TYPE(STR):
        case TYPE(SG):
        case TYPE(VR):
        case TYPE(TE):
        case TYPE(ST):
        case TYPE(ER):
            if (inc) OP_A(cs, c, RCI, OP, { .t = tn->t }, NULL);
            else OP_A(cs, c, RCD, OP, { .t = tn->t }, NULL);
            break;
        default:
            break;
    }
    return CODE_ER(cs, OK, NULL);
}

#define OP_RCI(CS, C, TN) if ((cstat = code_gen_rc(CS, TN, C, true)) != CODE_STAT(OK)) return cstat;

#define OP_RCD(CS, C, TN) if ((cstat = code_gen_rc(CS, TN, C, false)) != CODE_STAT(OK)) return cstat;

static code_stat code_gen_gc(code_st *const cs, const type_node *const tn, const ast *const a, code **c, int16_t id) {
    op_c oc = OP_C(GC);
    type t = TYPE(OP);
    op_d od = (op_d) { .t = tn->t };
    if (id >= 0) {
        oc = OP_C(GCTSVI);
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
        case TYPE(VR):
        case TYPE(TE):
        case TYPE(ST):
        case TYPE(FN):
        case TYPE(ER):
            code_a(cs->a, c, (op) {oc, t, 0, 0, od, a});
            break;
        default:
            return CODE_ER(cs, GC_INV, a);

    }
    return CODE_ER(cs, OK, NULL);
}

#define OP_GC(CS, C, TN, A) if ((cstat = code_gen_gc(CS, TN, A, C, -1)) != CODE_STAT(OK)) return cstat;

#define OP_GCTSVI(CS, C, TN, A, I) if ((cstat = code_gen_gc(CS, TN, A, C, I)) != CODE_STAT(OK)) return cstat;

#define IFCGEN(FN, CS, A, C) if ((cstat = FN(CS, A, C)) != CODE_STAT(OK)) return cstat

static code_stat code_gen_lst(code_st *const cs, const lst_node *const lst, code **c) {
    code_stat cstat;
    lst_itm *h = lst->h;
    code *gc = NULL;
    type_node *th;
    int16_t id = -1;
    if (lst->tn->t == TYPE(TE)) {
        gc = code_i(cs->a, CODE_I_SIZE);
        OP_A(cs, &gc, EFN, CODE, { .t = lst->tn->t }, NULL);
        OP_A(cs, &gc, LA, VAR, { SLV(0, lst->tn->t) }, NULL);
        OP_A(cs, &gc, RCF, OP, { .t = lst->tn->t }, NULL);
    }
    while (h) {
        if (lst->tn->t == TYPE(STMT)) { // check for vars with only types
            if (h->a->at == AST_TYPE(OP) && h->a->n.op->ot == OP_TYPE(ASS)) {
                if (h->a->n.op->l->at == AST_TYPE(VAR) &&  h->a->n.op->r->at == AST_TYPE(TYPE)) {
                    h = h->next;
                    continue;
                }
            }
        }
        IFCGEN(code_gen, cs, h->a, c);
        if (lst->tn->t == TYPE(TE)) {
            if (!(th = ast_gtn(h->a))) return CODE_ER(cs, NO_T_FOR_TE_IDX, h->a);
            OP_RCD(cs, &gc, th);
            OP_GCTSVI(cs, &gc, th, h->a, ++id); // h->a node tkn in gc fn
        }
        h = h->next;
    }
    if (lst->tn->t == TYPE(TE)) {
        OP_A(cs, &gc, DEL, OP, { .t = lst->tn->t }, NULL);
        OP_A(cs, &gc, RFN, CODE, { .t = TYPE(VD) }, NULL);
    }
    if (lst->tn->t == TYPE(TE) || lst->tn->t == TYPE(VR)) {
        code_a(cs->a, c, (op) {OP_C(CTSV), lst->tn->t, 0, 0, (op_d) { .tsv = ctsv_i(cs->a, lst->len, NULL, gc) }, NULL});
    }
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_hsh(code_st *const cs, const hsh_node *const hsh, code **c) {
    code_stat cstat;
    kv_itm *h = hsh->h;
    code *gc;
    type_node *th;
    int16_t id = -1;
    if (hsh->tn->t == TYPE(ST)) {
        gc = code_i(cs->a, CODE_I_SIZE);
        OP_A(cs, &gc, EFN, CODE, { .t = TYPE(VD) }, NULL);
        OP_A(cs, &gc, LA, VAR, { SLV(0, TYPE(ST)) }, NULL);
        OP_A(cs, &gc, RCF, OP, { .t = TYPE(ST) }, NULL);
    }
    while (h) {
        IFCGEN(code_gen, cs, h->a, c);
        if (hsh->tn->t == TYPE(ST)) {
            if (!(th = ast_gtn(h->a))) return CODE_ER(cs, NO_T_FOR_ST_IDX, h->a);
            OP_RCD(cs, &gc, th);
            OP_GCTSVI(cs, &gc, th, h->a, ++id); // h->a node tkn in gc fn
        }
        h = h->next;
    }
    if (hsh->tn->t == TYPE(ST)) {
        OP_A(cs, &gc, DEL, OP, { . t = TYPE(ST) }, NULL);
        OP_A(cs, &gc, RFN, CODE, { .t = TYPE(VD) }, NULL);
        OP_A(cs, c, CTSV, ST, { .tsv = ctsv_i(cs->a, hsh->len, NULL, gc) }, NULL);
    }
    return CODE_ER(cs, OK, NULL);
}

static code_stat sym_get_hd(code_st *const cs, const sym_node *const sym, hsh_data **hd) {
    type_node *tn;
    if (!(tn = ast_gtn(sym->a))) return CODE_ER(cs, SYM_NO_T_FOR_A, sym->a);
    if (tn->t == TYPE(ST) && tn->a && tn->a->at == AST_TYPE(TBL)) {
        tbl_itm *ti;
        if (tbl_op(cs->a, &tn->a->n.tl, sym->s, NULL, &ti, NULL, TBL_OP_FLG(FD)) != TBL_STAT(OK)) return CODE_ER(cs, SYM_INV_TBL_R, sym->a);
        *hd = (hsh_data*) ti->data;
        return CODE_ER(cs, OK, NULL);
    }
    return CODE_ER(cs, SYM_INV, NULL);
}

static code_stat code_gen_sym(code_st *const cs, const sym_node *const sym, code **c) {
    code_stat cstat;
    // TODO no ast with symbol
    IFCGEN(code_gen, cs, sym->a, c);
    hsh_data *hd;
    if ((cstat = sym_get_hd(cs, sym, &hd)) != CODE_STAT(OK)) return cstat;
    OP_A(cs, c, GIDX, U6, { .u6 = (uint64_t) hd->id }, sym->a);
    OP_RCI(cs, c, hd->tn);
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_if(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    code *ifc = code_i(cs->a, CODE_I_SIZE);
    if_itm *h = a->n.in->h;
    type_node *tc;
    while (h) {
        op_if *of = op_if_i(cs->a, CODE_I_SIZE);
        if (h->cond) {
            IFCGEN(code_gen, cs, h->cond, &of->cond);
            if (!(tc = ast_gtn(h->cond))) return CODE_ER(cs, NO_T_FOR_IF_COND, h->cond);
        }
        IFCGEN(code_gen_lst, cs, h->body, &of->body);
        OP_A(cs, &ifc, COND, COND, { .of = of }, a);
        h = h->next;
    }
    OP_A(cs, c, IF, IF, { .c = ifc }, a);
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_lop(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    op_if *of = op_if_i(cs->a, CODE_I_SIZE);
    type_node *tc;
    IFCGEN(code_gen, cs, a->n.lop->cond, &of->cond);
    if (!(tc = ast_gtn(a->n.lop->cond))) return CODE_ER(cs, NO_T_FOR_LOP_COND, a);
    IFCGEN(code_gen_lst, cs, a->n.lop->body, &of->body);
    OP_A(cs, c, LOP, LOP, { .of = of }, a);
    return CODE_ER(cs, OK, NULL);
}

static code_stat p_int(code_st *const cs, type t, const ast *const a, code **c) {
    int64_t i = tkn_to_int64_t(&a->t, cs->str);
    switch (t) {
        // TODO
        case TYPE(U6):
            OP_A(cs, c, PV, U6, { .u6 = (uint64_t) i }, a);
            break;
        // TODO
        case TYPE(I6):
            OP_A(cs, c, PV, I6, { .i6 = i }, a);
            break;
        default:
            return CODE_ER(cs, INV_INT_CST_PUSH, a);
    }
    return CODE_ER(cs, OK, NULL);
}

#define OP_P_INT_RET(OPN, CS, TGT, C) if (OPN->TGT->at == AST_TYPE(VAL) && OPN->TGT->n.val->tn->t == TYPE(INT) && (cstat = p_int(cs, OPN->ret->t, OPN->TGT, C)) != CODE_STAT(OK)) return cstat

static code_stat cor_int(code_st *const cs, const ast *const a, const ast *const b, code **c) {
    if (a->at != AST_TYPE(VAL)) return CODE_ER(cs, OK, NULL);
    if (a->n.val->tn->t != TYPE(INT)) return CODE_ER(cs, OK, NULL);
    if (b->at == AST_TYPE(VAL) && b->n.val->tn->t == TYPE(INT)) return p_int(cs, TYPE(I6), a, c);
    type_node *tb;
    if (!(tb = ast_gtn(b))) return CODE_ER(cs, NO_TYPE_COR_INT, a);
    return p_int(cs, tb->t, a, c);
}

#define OP_P_INT_COR(CS, A, B, C) if ((cstat = cor_int(CS, A, B, C)) != CODE_STAT(OK)) return cstat;

#define OP_ZOO(CS, TN, C) if (TN->t != TYPE(BL)) OP_A(CS, C, ZOO, OP, { .t = TN->t }, a);

static code_stat store_var(code_st *const cs, const ast *const a, code **c, var_node *const var) {
    if (var->tn->t == TYPE(VD)) return CODE_ER(cs, INV_TYPE_STORE_VD, a);
    code_stat cstat;
    OP_RCI(cs, c, var->tn);
    switch (var->vt) {
        case VAR_TYPE(U):
            return CODE_ER(cs, VAR_TYPE_U, a);
        case VAR_TYPE(G):
            OP_A(cs, c, SG, VAR, { SLV(var->id, var->tn->t) }, a);
            break;
        case VAR_TYPE(L):
            OP_A(cs, c, SL, VAR, { SLV(var->id - var->fns->args->len, var->tn->t) }, a);
            break;
        case VAR_TYPE(A):
            OP_A(cs, c, SA, VAR, { SLV(var->fns->args->len - 1 - var->id, var->tn->t) }, a);
            break;
    }
    return CODE_ER(cs, OK, a);
}

static code_stat load_var(code_st *const cs, const ast *const a, code **c, bool gc) {
    code_stat cstat;
    switch (a->n.var->vt) {
        case VAR_TYPE(U):
            return CODE_ER(cs, VAR_TYPE_U, a);
        case VAR_TYPE(G):
            OP_A(cs, c, LG, VAR, { SLV(a->n.var->id, a->n.var->tn->t) }, a);
            break;
        case VAR_TYPE(L):
            OP_A(cs, c, LL, VAR, { SLV(a->n.var->id - a->n.var->fns->args->len, a->n.var->tn->t) }, a);
            break;
        case VAR_TYPE(A):
            // reverse count to get stack
            OP_A(cs, c, LA, VAR, { SLV(a->n.var->fns->args->len - 1 - a->n.var->id, a->n.var->tn->t) }, a);
            break;
    }
    if (gc) {
        OP_RCD(cs, c, a->n.var->tn);
        OP_GC(cs, c, a->n.var->tn, a);
    } else {
        OP_RCI(cs, c, a->n.var->tn);
    }
    return CODE_ER(cs, OK, a);
}

static int64_t te_call_idx(code_st *const cs, const call_node *const cn) {
    if (cn->tgt->n.var->tn->t == TYPE(TE) && cn->args->len == 1 && cn->args->h->a->at == AST_TYPE(VAL) && cn->args->h->a->n.val->tn->t == TYPE(INT)) {
        return tkn_to_int64_t(&cn->args->h->a->t, cs->str);
    }
    return -1;
}
static code_stat er_rer(code_st *const cs, type_node *tn, rer *const erer) {
    if (tn->t != TYPE(ER)) return CODE_ER(cs, INV_ER_T, NULL);
    erer->ec = NFEC(tn->flgs);
    if (!(tn = ast_gtn(tn->a))) return CODE_ER(cs, INV_ER_T, tn->a);
    erer->t = tn->t;
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_op(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    op_node *opn = a->n.op;
    type_node *tl, *tr;
    int64_t i6, tidx;
    hsh_data *hd;
    code *gc;
    code_st ldcs;
    rer erer;
    switch (opn->ot) {
        case OP_TYPE(TC):
            if (!opn->l) { // throw
                IFCGEN(code_gen, cs, opn->r, c);
                if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, TC_R_N, opn->r);
                OP_A(cs, c, TE, ER, { RER(tr->t, false) }, opn->r);
                return CODE_ER(cs, OK, NULL); // stop panic
            } else { // catch
                IFCGEN(code_gen, cs, opn->r, c);
                if (opn->l->n.lst->len == 2) {
                    if ((cstat = store_var(cs, a, c, opn->l->n.lst->h->a->n.var)) != CODE_STAT(OK)) return cstat;
                }
                OP_A(cs, c, CE, ER, { RER(TYPE(ER), true) }, opn->r);
                if ((cstat = store_var(cs, a, c, opn->l->n.lst->t->a->n.var)) != CODE_STAT(OK)) return cstat;
                break;
            }
            return CODE_ER(cs, INV_TC, a);
        case OP_TYPE(ASS):
            IFCGEN(code_gen, cs, opn->r, c);
            if (opn->l->at == AST_TYPE(VAR)) {
                if ((opn->flgs & NODE_FLG(GCV)) && (cstat = load_var(cs, opn->l, c, true)) != CODE_STAT(OK)) return cstat;
                if ((cstat = store_var(cs, a, c, opn->l->n.var)) != CODE_STAT(OK)) return cstat;
                break;
            } else if (opn->l->at == AST_TYPE(SYM)) {
                if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, ASS_R_N, opn->r);
                OP_RCI(cs, c, tr);
                IFCGEN(code_gen, cs, opn->l->n.sym->a, c);
                if ((cstat = sym_get_hd(cs, opn->l->n.sym, &hd)) != CODE_STAT(OK)) return cstat;
                OP_A(cs, c, GIDX, U6, { .u6 = (uint64_t) hd->id }, opn->l);
                OP_RCD(cs, c, hd->tn);
                OP_GC(cs, c, hd->tn, opn->l);
                IFCGEN(code_gen, cs, opn->l->n.sym->a, c);
                OP_A(cs, c, SIDX, U6, { .u6 = (uint64_t) hd->id }, opn->l);
                break;
            } else if (opn->l->at == AST_TYPE(CALL)) {
                if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, ASS_R_N, opn->r);
                OP_RCI(cs, c, tr);
                if ((tidx = te_call_idx(cs, opn->l->n.cn)) < 0) return CODE_ER(cs, ASS_TE_INV, opn->l);
                IFCGEN(code_gen, cs, opn->l->n.cn->tgt, c);
                OP_A(cs, c, GIDX, U6, { .u6 = (uint64_t) tidx }, opn->l);
                OP_RCD(cs, c, opn->l->n.cn->ret);
                OP_GC(cs, c, opn->l->n.cn->ret, opn->l);
                IFCGEN(code_gen, cs, opn->l->n.cn->tgt, c);
                OP_A(cs, c, SIDX, U6, { .u6 = (uint64_t) tidx }, opn->l);
                break;
            }
            return CODE_ER(cs, INV_L_ASS, a);
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
                    if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
                    if (tr->t >= TYPE(U3) && tr->t <= TYPE(F6)) {
                        OP_A(cs, c, CSTSG, OP, { .t = tr->t }, a);
                        return CODE_ER(cs, OK, NULL);
                    }
                    return CODE_ER(cs, INV_CST_SG, a);
                case TYPE(VR):
                    IFCGEN(code_gen, cs, opn->r, c);
                    if (!(tr = ast_gtn(opn->l->n.tn->a))) return CODE_ER(cs, NO_T_VR_GC, opn->l);
                    gc = code_i(cs->a, CODE_I_SIZE);
                    OP_A(cs, &gc, EFN, CODE, { .t = TYPE(VD) }, NULL);
                    OP_A(cs, &gc, LA, VAR, { SLV(0, TYPE(VR)) }, NULL);
                    OP_A(cs, &gc, RCF, OP, { .t = TYPE(VR) }, NULL);
                    switch (tr->t) {
                        case TYPE(STR):
                        case TYPE(SG):
                        case TYPE(VR):
                        case TYPE(TE):
                        case TYPE(ST):
                        case TYPE(ER):
                            OP_A(cs, &gc, GCVR, OP, { .t = tr->t }, NULL);
                        default:
                            break; // no gc for these types
                    }
                    OP_A(cs, &gc, DEL, OP, { . t = TYPE(VR) }, NULL);
                    OP_A(cs, &gc, RFN, CODE, { .t = TYPE(VD) }, NULL);
                    (*c)->ops[(*c)->len - 1].od.tsv->gc = gc;
                    return CODE_ER(cs, OK, NULL);
                case TYPE(FD):
                        if (opn->r->at == AST_TYPE(VAL) && opn->r->n.val->tn->t == TYPE(INT)) {
                            i6 = tkn_to_int64_t(&opn->r->t, cs->str);
                            if (i6 >= 0 && i6 <= 2) {
                                OP_A(cs, c, PV, FD, { .fd = (int) i6 }, opn->r);
                                return CODE_ER(cs, OK, a);
                            }
                        return CODE_ER(cs, INV_CST_INT_TO_FD, a);
                    }
                    return CODE_ER(cs, INV_CST_FD, a);
                default:
                    break;
            }
            return CODE_ER(cs, INV_CST, a);
        case OP_TYPE(DEL):
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
            OP_A(cs, c, DEL, OP, { .t = tr->t }, a);
            break;
        case OP_TYPE(LD):
            if (opn->r->at != AST_TYPE(MOD) || !opn->r->n.m) return CODE_ER(cs, LD_MOD_F, a);
            code_st_i(&ldcs, cs->a, cs->e, opn->r->n.m->src.str);
            opn->r->n.m->c = code_i(cs->a, CODE_I_SIZE);
            if (code_gen_fn(&ldcs, opn->r->n.m->fns, &opn->r->n.m->c) != CODE_STAT(OK)) return CODE_ER(cs, LD_MOD_F, opn->r);
            gc = code_i(cs->a, CODE_I_SIZE);
            OP_A(cs, &gc, EFN, CODE, { .t = TYPE(VD) }, NULL);
            OP_A(cs, &gc, LA, VAR, { SLV(0, TYPE(ST)) }, NULL);
            OP_A(cs, &gc, DEL, OP, { . t = TYPE(ST) }, NULL);
            OP_A(cs, &gc, RFN, CODE, { .t = TYPE(VD) }, NULL);
            OP_A(cs, c, LM, MOD, { .tsv = ctsv_i(cs->a, 0, opn->r->n.m, gc) }, opn->r);
            break;
        case OP_TYPE(VH):
            break; // no code
        case OP_TYPE(ADD):
            IFCGEN(code_gen, cs, opn->l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_ER(cs, INV_ADD_T_L, opn->r);
            if (tl->t == TYPE(VR)) {
                OP_A(cs, c, VRA, OP, { .t = opn->ret->t }, a);
                OP_GC(cs, c, tl, opn->l);
                break;
            }
            OP_P_INT_RET(opn, cs, l, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(cs, c, ADD, OP, { .t = opn->ret->t }, a);
            break;

        case OP_TYPE(SUB):
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, INV_SUB_T_R, opn->r);
            if (tr->t == TYPE(VR)) {
                IFCGEN(code_gen, cs, opn->r, c);
                if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, INV_SUB_VR_T_R, opn->r);
                if ((cstat = er_rer(cs, opn->ret, &erer)) != CODE_STAT(OK)) return cstat;
                OP_A(cs, c, VRS, ER, { .e = erer }, a);
                OP_A(cs, c, SWAP, VD, {}, NULL);
                OP_GC(cs, c, tr, opn->r);
                break;
            }
            IFCGEN(code_gen, cs, opn->l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, l, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(cs, c, SUB, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(MUL):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(cs, c, MUL, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(EQ):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_COR(cs, opn->l, opn->r, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_COR(cs, opn->r, opn->l, c);
            OP_A(cs, c, EQ, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(NOT):
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
            OP_ZOO(cs, tr, c);
            OP_A(cs, c, NOT, OP, { .t = TYPE(BL) }, a);
            break;
        case OP_TYPE(GT):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(cs, c, GT, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(LT):
            IFCGEN(code_gen, cs, opn->l, c);
            OP_P_INT_RET(opn, cs, l, c);
            IFCGEN(code_gen, cs, opn->r, c);
            OP_P_INT_RET(opn, cs, r, c);
            OP_A(cs, c, LT, OP, { .t = opn->ret->t }, a);
            break;
        case OP_TYPE(OR):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_ER(cs, OP_NO_T_L, a);
            OP_ZOO(cs, tl, c);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
            OP_ZOO(cs, tr, c);
            OP_A(cs, c, OR, OP, { .t = TYPE(BL) }, a);
            break;
        case OP_TYPE(CNCT):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_ER(cs, OP_NO_T_L, a);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
            switch  (opn->ret->t) {
                case TYPE(SG):
                    if (tr->t == TYPE(TE) || tr->t == TYPE(VR)|| tr->t == TYPE(STR) || tr->t == TYPE(SG)) {
                        OP_A(cs, c, CNCTSG, OP, { .t = tr->t }, a);
                        OP_GC(cs, c, tl, opn->l);
                        OP_GC(cs, c, tr, opn->r);
                    } else return CODE_ER(cs, INV_SG_CNCT, a);
                    break;
                default: return CODE_ER(cs, INV_CNCT_OP, a);
            }
            break;
        case OP_TYPE(RW):
            IFCGEN(code_gen, cs, opn->l, c);
            if (!(tl = ast_gtn(opn->l))) return CODE_ER(cs, OP_NO_T_L, a);
            IFCGEN(code_gen, cs, opn->r, c);
            if (!(tr = ast_gtn(opn->r))) return CODE_ER(cs, OP_NO_T_R, a);
            if (tl->t == TYPE(FD) && tr->t != TYPE(FD)) {
                OP_A(cs, c, WFD, OP, { .t = tr->t }, a);
                OP_GC(cs, c, tr, a);
            }
            else return CODE_ER(cs, INV_FD_OP, a);
            break;
    }
    if (opn->ret->t == TYPE(ER) && !NFEC(opn->ret->flgs)) {
        OP_A(cs, c, PE, ER, { RER(TYPE(ER), false) }, a);
        if (opn->flgs & NODE_FLG(GCR)) {
            if (!(tr = ast_gtn(opn->ret->a))) return CODE_ER(cs, INV_ER_GCR, a);
            OP_GC(cs, c, tr, opn->ret->a);
        }
    }
    else if (opn->ret->t != TYPE(ER) && opn->ret->t != TYPE(VD) && (opn->flgs & NODE_FLG(GCR))) OP_GC(cs, c, opn->ret, a);
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_call_ftn(code_st *const cs, const ast *const a, code **c, const type_node *const ftn, bool ires) {
    code_stat cstat;
    call_node *cn = a->n.cn;
    type_node *tn;
    lst_itm *ch, *ah, *ct;
    if (ftn->t != TYPE(FN)) return CODE_ER(cs, CALL_T_N_FN, a);
    ch = ftn->a->n.lst->h;
    ah = cn->args->h;
    while (ah) {
        IFCGEN(code_gen, cs, ah->a, c);
        OP_P_INT_COR(cs, ah->a, ch->a, c);
        ah = ah->next;
        ch = ch->next;
    }
    IFCGEN(code_gen, cs, cn->tgt, c);
    tn = cn->ret;
    if (tn->t == TYPE(ER)) {
        if (!(tn = ast_gtn(tn->a))) return CODE_ER(cs, CALL_T_ER_T_INV, a);
        if (ires) OP_A(cs, c, CS, ER, { RER(tn->t, NFEC(tn->flgs)) }, a);
        else OP_A(cs, c, CFN, ER, { RER(tn->t, NFEC(tn->flgs)) }, a);
    } else {
        if (ires) OP_A(cs, c, CS, OP, { .t = tn->t }, a);
        else OP_A(cs, c, CFN, OP, { .t = tn->t }, a);
    }
    if (tn->t != TYPE(VD)) {
        OP_A(cs, c, PUSH, U3, { .u3 = 0 }, a); // TODO xmm
        if (cn->flgs & NODE_FLG(GCR)) {
            OP_GC(cs, c, tn, a);
        }
    }
    ct = ftn->a->n.lst->t->prev;
    while (ct) {
        if (!(tn = ast_gtn(ct->a))) return CODE_ER(cs, CALL_CT_ARG_T_GC_INV, a);
        if (cn->ret->t != TYPE(VD)) OP_A(cs, c, SWAP, OP, { .t = tn->t }, a); // swap ret and gc args
        OP_GC(cs, c, tn, ct->a);
        ct = ct->prev;
    }
    if (cn->ret->t == TYPE(ER) && !NFEC(cn->ret->flgs)) OP_A(cs, c, PE, ER, { RER(TYPE(ER), false) }, a);
    return CODE_ER(cs, OK, NULL);
}

code_stat code_gen_call(code_st *const cs, const ast *const a, code **c) {
    code_stat cstat;
    call_node *cn = a->n.cn;
    op_node *opn;
    int64_t tidx;
    switch (cn->tgt->at) {
        case AST_TYPE(RES):
            if (cn->tgt->n.rn->rt != RES_TYPE(SELF)) return CODE_ER(cs, CALL_RES_NOT_SELF, a);
            return code_gen_call_ftn(cs, a, c, cn->tgt->n.rn->tn, true);
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
        case AST_TYPE(SYM):
            return code_gen_call_ftn(cs, a, c, cn->tgt->n.sym->tn, false);
        case AST_TYPE(VAR):
            if ((tidx = te_call_idx(cs, cn)) > -1) {
                IFCGEN(code_gen, cs, cn->tgt, c);
                OP_A(cs, c, GIDX, U6, { .u6 = (uint64_t) tidx }, a);
                OP_RCI(cs, c, cn->ret);
                break;
            }
            return code_gen_call_ftn(cs, a, c, cn->tgt->n.var->tn, false);
        default:
            return CODE_ER(cs, INV_CALL_TGT, a);
    }
    return CODE_ER(cs, OK, NULL);
}

static code_stat code_gen_ret(code_st *const cs, const fn_node *const fn, code **c) {
    code_stat cstat;
    uint8_t ngl = fn->idc - fn->args->len;
    type t = TYPE(VD);
    type_node *tn;
    if (fn->sig->t == TYPE(FN)) {
        if (!(tn = ast_gtn(fn->sig->a->n.lst->t->a))) return CODE_ER(cs, FN_RET_T_INV, NULL);
        t = tn->t;
    }
    if (ngl && fn->par) {
        tbl_itm *ti = fn->tl->t;
        while (ngl > 0) {
            if (t != TYPE(VD)) OP_A(cs, c, SWAP, VD, {}, NULL);
            OP_GC(cs, c, ((var_node*) ti->data)->tn, NULL);
            ti = ti->prev;
            ngl--;
        }
    }
    if (t == TYPE(ER)) {
        if (!(tn = ast_gtn(tn->a))) return CODE_ER(cs, FN_RET_ER_T_INV, NULL);
        if (tn->t != TYPE(VD)) OP_A(cs, c, POP, U3, { .u3 = 0 }, NULL); // TODO xmm
        OP_A(cs, c, RFN, ER, { RER(tn->t, NFEC(tn->flgs)) }, NULL);
    } else {
        if (t != TYPE(VD)) OP_A(cs, c, POP, U3, { .u3 = 0 }, NULL); // TODO xmm
        OP_A(cs, c, RFN, CODE, { .t = t }, NULL);
    }
    return CODE_ER(cs, OK, NULL);
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
                    sg = ala(cs->a, (a->t.len - 1) * sizeof(char));
                    for (size_t i = 0; i < a->t.len - 2; i++) {
                        if (cs->str[a->t.pos + 1 + i] == '\\') {
                            i++;
                            switch (cs->str[a->t.pos + 1 + i]) {
                                case 'n':
                                    sg[sgi++] = '\n';
                                    break;
                                default:
                                    return CODE_ER(cs, INV_STR_ESC, a);
                            }
                            break;
                        } else sg[sgi++] = cs->str[a->t.pos + 1 + i];
                    }
                    OP_A(cs, c, PV, STR, { .sg = sg }, a);
                    break;
                default:
                    return CODE_ER(cs, NO_OP_FOR_VAL_T, a);
            }
            break;
        case AST_TYPE(OP): return code_gen_op(cs, a, c);
        case AST_TYPE(LST): return code_gen_lst(cs, a->n.lst, c);
        case AST_TYPE(HSH): return code_gen_hsh(cs, a->n.hsh, c);
        case AST_TYPE(TBL): return CODE_ER(cs, TBL_FOUND, a);
        case AST_TYPE(SYM): return code_gen_sym(cs, a->n.sym, c);
        case AST_TYPE(IF): return code_gen_if(cs, a, c);
        case AST_TYPE(LOP): return code_gen_lop(cs, a, c);
        case AST_TYPE(MOD): return CODE_ER(cs, MOD_FOUND, a);
        case AST_TYPE(FN):
            cfn = code_i(cs->a, CODE_I_SIZE);
            IFCGEN(code_gen_fn, cs, a->n.fn, &cfn);
            OP_A(cs, c, PV, FN, { .c = cfn }, a);
            break;
        case AST_TYPE(CALL): return code_gen_call(cs, a, c);
        case AST_TYPE(RET):
            // TODO possible dead code
            if (a->n.ret->a->at == AST_TYPE(VAL)) {
                switch (a->n.ret->a->n.val->tn->t) {
                    case TYPE(INT):
                        if ((cstat = p_int(cs, a->n.ret->tn->t, a->n.ret->a, c)) != CODE_STAT(OK)) return cstat;
                        break;
                    case TYPE(FLT):
                    case TYPE(STR):
                        break; // TODO
                    default:
                        return CODE_ER(cs, NO_OP_FOR_RET_VAL_T, a);
                }
            } else IFCGEN(code_gen, cs, a->n.ret->a, c);
            return code_gen_ret(cs, a->n.ret->fn, c);
        case AST_TYPE(VAR): return load_var(cs, a, c, false);
    }
    return CODE_ER(cs, OK, NULL);
}

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code **c) {
    code_stat cstat;
    if (fn->idc < fn->args->len) return CODE_ER(cs, ARG_LEN_GT_LOCAL_LEN, NULL);
    OP_A(cs, c, EFN, CODE, { .t = TYPE(VD) }, NULL);
    uint8_t ngl = fn->idc - fn->args->len;
    if (ngl) {
        if (!fn->par) OP_A(cs, c, AG, U3, { .u3 = ngl }, NULL);
        else OP_A(cs, c, AL, U3, { .u3 = ngl }, NULL);
    }
    IFCGEN(code_gen_lst, cs, fn->body, c);
    return code_gen_ret(cs, fn, c);
}
