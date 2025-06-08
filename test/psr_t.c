
#include "psr_t.h"

static const alfr pm = { .a = malloc, .f = free };

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&pm, &pm, (void*) te_f);
    te *b = te_i(10, &pm, NULL);
    tbl *t = tbl_i(&pm, tkn_sh, c4_eq, tl, b);
    return t;
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&pm, &pm, (void*) te_f);
    te *b = te_i(10, &pm, NULL);
    tbl *t = tbl_i(&pm, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

psr *bpsr(const char *pgm) {
    tkn *t = tkn_i(&pm, &pm, tkn_mktbl, tkn_df, mc_i_cstr(pgm, &pm));
    tkn_b(t);
    vr *v = vr_i(10, &pm, (void*) te_f);
    psr *p = psr_i(&pm, &pm, &pm, psr_entry_f, mktbl, t, v);
    return psr_b(p);
}

const char *aplyopadd = "+(1;2)";

const char *typetype = "FN(I6`x;F6`y;FN(U6`z;I6))";

const char *fnadd3 = "{f::FN(I6`a;I6`b;I6`c;I6)${-a+b+c};f(1;2;3)/p1}()";

const char *fnf6muli6cstdiv = "{f::FN(F6`x;F6`y;U6`z;F6)${/(x*y;F6$z)};f(4.4;6.6;U6$2)/p1}()";
