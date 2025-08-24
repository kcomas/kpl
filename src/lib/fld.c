
#include "fld.h"

fld *fld_i(const alfr *af, const alfr *ta, const alfr *ea, fld_tbl_i *fti, fld_lst_i *fli, ast *a, tbl *ft) {
    fld *f = af->a(sizeof(fld));
    f->tvc = 0;
    f->r = 1;
    f->af = af;
    f->ta = ta;
    f->ea = ea;
    f->fti = fti;
    f->fli = fli;
    f->a = a ? ast_c(a) : a;
    f->ft = ft;
    return f;
}

fld *fld_i_fld(const fld *f, ast *a) {
    return fld_i(f->af, f->ta, f->ea, f->fti, f->fli, a, tbl_c(f->ft));
}

static void fld_entry_f(void *p) {
    te *e = p;
    lst_f(e->d[1].p);
    e->af->f(e);
}

void fld_a(fld *f, ast_cls cls, fld_test_fn test, fld_fn fn) {
    te *kv;
    if (tbl_g_i(f->ft, U6(cls), &kv) == TBL_STAT(NF)) {
        kv = te_i(2, f->ta, fld_entry_f);
        kv->d[0] = U6(cls);
        kv->d[1] = P(f->fli());
        tbl_a(f->ft, kv);
    }
    lst *l = kv->d[1].p;
    te *fns = te_i(2, f->ta, NULL);
    fns->d[0] = P(test);
    fns->d[1] = P(fn);
    lst_ab(l, P(fns));
}

static fld_stat fld_lst_n(fld *f, lst *l, err **e, bool rr) {
    fld_stat stat = FLD_STAT(OK);
    if (!l) return stat;
    te *h = l->h;
    while (h) {
        if ((stat = fld_n(f, (te**) &h->d[0].p, e, rr)) != FLD_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

fld_stat fld_n(fld *f, te **an, err **e, bool rr) {
    fld_stat stat = FLD_STAT(OK);
    if (!*an) return stat;
    switch ((*an)->d[2].u4) {
        case AST_CLS(R):
            if ((stat = fld_n(f, (te**) &(*an)->d[4].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(T):
        case AST_CLS(E):
        case AST_CLS(I):
        case AST_CLS(S):
            break;
        case AST_CLS(V):
            if ((stat = fld_lst_n(f, (*an)->d[4].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(O):
            if ((stat = fld_n(f, (te**) &(*an)->d[5].p, e, rr)) != FLD_STAT(OK) || (stat = fld_n(f, (te**) &(*an)->d[6].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(Z):
            if ((stat = fld_n(f, (te**) &(*an)->d[4].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(A):
            if ((stat = fld_n(f, (te**) &(*an)->d[4].p, e, rr)) != FLD_STAT(OK) || (stat = fld_lst_n(f, (*an)->d[5].p, e, rr))) return stat;
            break;
        case AST_CLS(L):
            if ((stat = fld_lst_n(f, (*an)->d[4].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(C):
            if ((stat = fld_n(f, (te**) &(*an)->d[4].p, e, rr)) != FLD_STAT(OK)) return stat;
            break;
        case AST_CLS(X):
            break;
        default:
            return FLD_STAT(INV);
    }
    te *pan, *kv, *h, *fns;
    do {
        pan = *an;
        uint16_t cls = (*an)->d[2].u4;
        if (tbl_g_i(f->ft, (*an)->d[2], &kv) == TBL_STAT(OK)) {
            h = ((lst*) kv->d[1].p)->h;
            while (h) {
                fns = h->d[0].p;
                if (((fld_test_fn*) fns->d[0].p)(*an) && (stat = ((fld_fn*) fns->d[1].p)(f, an, e)) != FLD_STAT(OK)) return stat;
                if ((*an)->d[2].u4 != cls) break;
                h = h->d[2].p;
            }
        }
    } while (rr && pan != *an); // run again if node changes
    return stat;
}

void fld_f(fld *f) {
    if (!f || --f->r > 0) return;
    ast_f(f->a);
    tbl_f(f->ft);
    f->af->f(f);
}
