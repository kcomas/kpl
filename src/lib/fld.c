
#include "fld.h"

fld *fld_i(const alfr *af, const alfr *ta, fld_tbl_i *fti, fld_lst_i *fli, tbl *ft) {
    fld *f = af->a(sizeof(fld));
    f->af = af;
    f->ta = ta;
    f->fti = fti;
    f->fli = fli;
    f->ft = ft;
    return f;
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

static fld_stat fld_lst_n(fld *f, lst *l) {
    fld_stat stat = FLD_STAT(OK);
    te *h = l->h;
    while (h) {
        if ((stat = fld_n(f, (te**) &h->d[0].p)) != FLD_STAT(OK)) return stat;
        h = h->d[2].p;
    }
    return stat;
}

fld_stat fld_n(fld *f, te **an) {
    fld_stat stat = FLD_STAT(OK);
    if (!*an) return stat;
    ast_cls cls = (*an)->d[2].u6;
    te *kv;
    if (tbl_g_i(f->ft, U6(cls), &kv) == TBL_STAT(OK)) {
        te *h = ((lst*) kv->d[1].p)->h;
        while (h) {
            te *fns = h->d[0].p;
            if (((fld_test_fn*) fns->d[0].p)(*an) && (stat = ((fld_fn*) fns->d[1].p)(f, an)) != FLD_STAT(OK)) return stat;
            h = h->d[2].p;
        }
    }
    switch (cls) {
        case AST_CLS(R):
            return fld_n(f, (te**) &(*an)->d[4].p);
        case AST_CLS(T):
            break;
        case AST_CLS(I):
            break;
        case AST_CLS(S):
            break;
        case AST_CLS(V):
            break;
        case AST_CLS(O):
            if ((stat = fld_n(f, (te**) &(*an)->d[5].p)) != FLD_STAT(OK)) return stat;
            return fld_n(f, (te**) &(*an)->d[6].p);
        case AST_CLS(Z):
            return fld_n(f, (te**) &(*an)->d[5].p);
        case AST_CLS(A):
            if ((stat = fld_n(f, (te**) &(*an)->d[4].p)) != FLD_STAT(OK)) return stat;
            return fld_lst_n(f, (*an)->d[5].p);
        case AST_CLS(L):
            return fld_lst_n(f, (*an)->d[4].p);
        default:
            return FLD_STAT(INV);
    }
    return stat;
}

void fld_f(fld *f) {
    if (!f || --f->r > 0) return;
    tbl_f(f->ft);
    f->af->f(f);
}
