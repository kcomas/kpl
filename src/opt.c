
#include "opt.h"

static fld_stat entry_o(fld *f, te **an, err **e) {
    (void) f;
    te *lte = (*an)->d[3].p;
    uint32_t flgs = ast_lst_tbl_e_g_f(lte);
    if (!flgs) return fld_err(f, *an, e, "opt var flgs inv");
    if (flgs & LTE_FLG(A)) return FLD_STAT(OK);
    // TODO chk if unused
    return FLD_STAT(OK);
}

static bool entry_t(const te *an) {
    return an->d[2].u4 == AST_CLS(E);
}

static fld_stat lst_inv_o(fld *f, te **an, err **e) {
    (void) f;
    return fld_err(f, *an, e, "opt inv lst");
}

static void export_tbl_f(void *p) {
    te *t = p;
    mc_f(t->d[0].p);
    // TODO jit destructor
    te_f(t->d[2].p);
    t->af->f(t);
}

static bool lst_inv_t(const te *an) {
    te *p = an->d[0].p;
    return an->d[2].u4 == AST_CLS(L) && (!p || (p->d[2].u4 != AST_CLS(A) && p->d[2].u4 != AST_CLS(O)));
}

static fld_stat lst_le_o(fld *f, te **an, err **e) {
    uint16_t eti = 0;
    if (!(*an)->d[3].p) return FLD_STAT(OK);
    te *h = ((tbl*) (*an)->d[3].p)->i->h, *lte, *kv, *rn = NULL;
    while (h) {
        lte = h->d[0].p;
        uint32_t flgs = ast_lst_tbl_e_g_f(lte);
        if ((flgs & LTE_FLG(O)) && (kv = chk_g_pn_lte((*an)->d[0].p, lte->d[0].p))) lte->d[1] = U6(kv->d[1].u6 | LTE_FLG(O));
        if (!(flgs & LTE_FLG(O)) && (flgs & LTE_FLG(E))) {
            if (!rn && ast_g_pn(AST_CLS(R), *an, &rn) != AST_STAT(OK)) return fld_err(f, *an, e, "opt cannot get root node");
            if (!rn->d[3].p) rn->d[3] = P(f->fti());
            if (tbl_g_i(rn->d[3].p, lte->d[0], &kv) == TBL_STAT(OK)) return fld_err(f, *an, e, "opt export already set");
            kv = te_i(3, f->ta, export_tbl_f);
            kv->d[0] = P(mc_c(lte->d[0].p));
            kv->d[1] = lte->d[1];
            kv->d[1] = u4_s_o(kv->d[1], 1, eti++);
            kv->d[2] = P(te_c(lte->d[2].p));
            tbl_a(rn->d[3].p, kv);
        }
        h = h->d[2].p;
    }
    return FLD_STAT(OK);
}

static bool lst_le_t(const te *an) {
    return an->d[2].u4 == AST_CLS(L);
}

static fld_stat aply_lst_o(fld *f, te **an, err **e) {
    (void) f;
    (void) e;
    te *lp, *ln, *h, *lte;
    if (ast_g_pn(AST_CLS(L), *an, &lp) == AST_STAT(OK)) {
        return fld_err(f, *an, e, "TODO add to parent lst");
    } else {
        ln = te_c((*an)->d[4].p);
        ln->d[0] = (*an)->d[0];
        te_f(*an);
        *an = ln;
        uint32_t lc = 0; // update idxs of locals
        h = ((tbl*) ln->d[3].p)->i->h;
        while (h) {
            lte = h->d[0].p;
            if (ast_lst_tbl_e_g_f(lte) & LTE_FLG(L)) ast_lst_tbl_e_s_i(lte, lc++);
            h = h->d[2].p;
        }
    }
    return FLD_STAT(OK);
}

static bool aply_lst_t(const te *an) {
    return an->d[2].u4 == AST_CLS(A) && an->d[4].p && ((te*) an->d[4].p)->d[2].u4 == AST_CLS(L);
}

static fld_stat cst_s_o(fld *f, te **an, err **e) {
    te *lt = ((te*) (*an)->d[5].p)->d[3].p, *r = (*an)->d[6].p, *rt = r->d[3].p;
    if (lt->d[1].u4 == rt->d[1].u4) return fld_err(f, *an, e, "opt unnecessary cst");
    switch (lt->d[1].u4) {
        case TYPE(U6):
            switch (rt->d[1].u4) {
                case TYPE(I6):
                    rt->d[1].u4 = TYPE(U6);
                    break;
                default:
                    return fld_err(f, *an, e, "opt inv cst U5");
            }
            break;
        default:
            return fld_err(f, *an, e, "opt inv cst s");
    }
    r->d[0] = (*an)->d[0];
    te_c(r);
    te_f(*an);
    *an = r;
    return FLD_STAT(OK);
}

static bool cst_s_t(const te *an) {
    return an->d[2].u4 == AST_CLS(O) && an->d[4].u4 == OC(CST) && ((te*) an->d[5].p)->d[2].u4 == AST_CLS(T) && ((te*) an->d[6].p)->d[2].u4 == AST_CLS(S);
}

#define S_OP(O) static un type_##O##_op(type t, un a, un b) { \
    switch (t) { \
        case TYPE(U3): return u3_##O(a, b); \
        case TYPE(U4): return u4_##O(a, b); \
        case TYPE(U5): return u5_##O(a, b); \
        case TYPE(U6): return u6_##O(a, b); \
        case TYPE(I3): return i3_##O(a, b); \
        case TYPE(I4): return i4_##O(a, b); \
        case TYPE(I5): return i5_##O(a, b); \
        case TYPE(I6): return i6_##O(a, b); \
        case TYPE(F5): return f5_##O(a, b); \
        case TYPE(F6): return f6_##O(a, b); \
        default: \
            break; \
    } \
    return U6(0); \
}

S_OP(add);
S_OP(sub);
S_OP(mul);
S_OP(div);

static fld_stat op_s_s_o(fld *f, te **an, err **e) {
    te *l = (*an)->d[5].p, *r = (*an)->d[6].p, *type = (*an)->d[3].p;
    un v;
    switch ((*an)->d[4].u4) {
        case OC(ADD):
            v = type_add_op(type->d[1].u4, l->d[4], r->d[4]);
            break;
        case OC(SUB):
            v = type_sub_op(type->d[1].u4, l->d[4], r->d[4]);
            break;
        case OC(MUL):
            v = type_mul_op(type->d[1].u4, l->d[4], r->d[4]);
            break;
        case OC(DIV):
            v = type_div_op(type->d[1].u4, l->d[4], r->d[4]);
            break;
        default:
            return fld_err(f, *an, e, "opt inv s inline op");
    }
    // TODO check for overflow/underflow
    te_c(r);
    r->d[0] = (*an)->d[0];
    r->d[1] = (*an)->d[1];
    r->d[4] = v;
    te_f(*an);
    *an = r;
    return FLD_STAT(OK);
}

static bool op_s_s_t(const te *an) {
    if (an->d[2].u4 != AST_CLS(O) || ((te*) an->d[5].p)->d[2].u4 != AST_CLS(S) || ((te*) an->d[6].p)->d[2].u4 != AST_CLS(S)) return false;
    switch (an->d[4].u4) {
        case OC(ADD):
        case OC(SUB):
        case OC(MUL):
        case OC(DIV):
            return true;
        default:
            break;
    }
    return false;
}

uint32_t opt_exp_id(te *x) {
    return u5_g_o(x->d[1], 1);
}

uint16_t opt_exp_eid(te *x) {
    return u4_g_o(x->d[1], 1);
}

uint32_t opt_exp_flgs(te *x) {
    return u4_g_o(x->d[1], 0);
}

fld *opt_b(fld *f) {
    fld_a(f, AST_CLS(E), entry_t, entry_o);
    fld_a(f, AST_CLS(L), lst_inv_t, lst_inv_o);
    fld_a(f, AST_CLS(L), lst_le_t, lst_le_o);
    fld_a(f, AST_CLS(A), aply_lst_t, aply_lst_o);
    fld_a(f, AST_CLS(O), cst_s_t, cst_s_o);
    fld_a(f, AST_CLS(O), op_s_s_t, op_s_s_o);
    return f;
}
