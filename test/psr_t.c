
#include "psr_t.h"

static const alfr pm = { .a = malloc, .f = free };

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tkn_sh, c4_eq, tl, b);
    return t;
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

static psr *bp = NULL;

static __attribute__((constructor)) void psr_con(void) {
    tkn *t = tkn_i(&pm, &al_te, &pm, tkn_mktbl, tkn_df, mc_i(0, &pm));
    tkn_b(t);
    vr *v = vr_i(10, &pm, (void*) te_f);
    bp = psr_i(&pm, &al_te, &al_lst, &pm, psr_entry_f, mktbl, t, v);
    psr_a(bp, PARSER(UN), PSR_MODE(ONCE), NULL, NULL, psr_op_m, psr_op_i, 1, tkn_a(bp->tt, TOKEN(UN), "Σ", tkn_ft));
    psr_b(bp);
}

static __attribute__((destructor)) void psr_des(void) {
    psr_f(bp);
}

psr *bpsr(const char *pgm) {
    return psr_i_psr(bp, mc_i_cstr(pgm, &pm));
}

const char *aplyopadd = "+(1;2)";

const char *typetype = "FN(I6`x;F6`y;FN(U6`z;I6))";

const char *fnadd3 = "{f::FN(I6`a;I6`b;I6`c;I6)${-a+b+c};f(1;2;3)/p1}()";

const char *fnf6muli6cstdiv = "{f::FN(F6`x;F6`y;U6`z;F6)${/(x*y;F6$z)};f(4.4;6.6;U6$2)/p1}()";

const char *apltypefn = "{f::FN${I6(a)+I6(b)*a};f(3;4)/p1}()";

const char *facloop = "{\n\tv::5\n\tf/e::1\n\t>(v;1)@{\n\t\tf*:v\n\t\tv-:1\n\t}\n\tf/p1\n}()";
