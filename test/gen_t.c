
#include "gen_t.h"


gen *bg = NULL;

static __attribute__((constructor(102))) void gen_c(void) {
    bg = gen_b(gen_i(&am, &al_te, &al_vr, &am, gen_cls_info_tbl, gen_op_tbl(GEN_OP(_END)), gen_mklst()));
}

static __attribute__((destructor)) void gen_d(void) {
    gen_f(bg);
}

lst *gen_mklst(void) {
    return lst_i(&al_lst, &al_te, (void*) te_f);
}

tbl *gen_cls_info_tbl(void) {
    lst *tl = gen_mklst();
    te *b = te_i(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

tbl *gen_op_tbl(size_t bcks) {
    lst *tl = gen_mklst();
    te *b = te_i(bcks, &al_te, NULL);
    tbl *t = tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
    return t;
}

void gen_build(_tests *_t, gen *g, uint8_t *m) {
    E();
    gen_st *st = gen_st_i(&am, &al_te, gen_op_tbl(10), gen_op_tbl(10), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL));
    as *a = as_i_as(ba);
    A(gen_st_p1(g, st) == GEN_STAT(OK), "gen_st_p1");
    gen_st_p(st);
    err *e = NULL;
    gen_stat stat = gen_n(g, st, a, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(stat == GEN_STAT(OK), "gen_n");
    printf("STATE AFTER\n");
    gen_st_p(st);
    p = 0;
    e = NULL;
    as_stat astat = as_n(a, &p, m, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(astat == AS_STAT(OK), "as_n");
    gen_p(g, m);
    printf("DATA\n");
    as_data_p(a, m);
    gen_st_f(st);
    gen_f(g);
    as_f(a);
}


