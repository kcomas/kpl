
#include "../chk.h"

static chk_stat chk_z_type_h(chk *c, te *an, err **e) {
    te *st, *kv;
    if (ast_g_t(an->d[4].p, &st) != AST_STAT(OK)) return chk_err(c, an, e, "chk st cannot get access type");
    if (st->d[1].u4 == TYPE(SL)) st = st->d[2].p;
    if (tbl_g_i(st->d[2].p, an->d[5], &kv) != TBL_STAT(OK)) return chk_err(c, an, e, "chk st inv access type");
    ((te*) an->d[3].p)->d[2] = P(te_c(kv->d[2].p));
    return CHK_STAT(OK);
}

static chk_stat chk_z_vd(chk *c, te *an, err **e) {
    (void) e;
    ((te*) an->d[3].p)->d[2] = P(type_s_i(c->a->ta, NULL, TYPE(VD)));
    return CHK_STAT(OK);
}

static chk_stat chk_z_vd_move(chk *c, te *an, err **e) {
    chk_stat stat;
    te *lte = ((te*) an->d[4].p)->d[3].p;
    if ((stat = chk_move_var(c, an, e, lte->d[0].p)) != CHK_STAT(OK)) return stat;
    return chk_z_vd(c, an, e);
}

static chk_stat chk_z_vd_un(chk *c, te *an, err **e) {
    (void) e;
    tbl *tt = tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(4, c->ta, NULL));
    tbl_a(tt, ast_lst_tbl_e_i(c->a, mc_c(an->d[5].p), U6(0), type_s_i(c->ta, NULL, TYPE(VD))));
    ((te*) an->d[3].p)->d[2] = P(type_h_i(c->ta, NULL, TYPE(UN), tt));
    return CHK_STAT(OK);
}

static chk_stat chk_z_z_(chk *c, te *an, err **e) {
    return chk_n(c, an, e);
}

static chk_stat chk_z_z_un(chk *c, te *an, err **e) {
    te *tn = ((te*) ((te*) an->d[4].p)->d[3].p)->d[2].p, *kv;
    tbl *ut = tn->d[2].p;
    if (ut->i->l == 1 && ((te*) ((te*) ut->i->t->d[0].p)->d[2].p)->d[1].u4 == TYPE(VD)) return chk_z_vd(c, an, e);
    if (tbl_g_i(ut, an->d[5], &kv) != TBL_STAT(OK)) return chk_err(c, an, e, "chk inv key in z un");
    ((te*) an->d[3].p)->d[2] = P(te_c(kv->d[2].p));
    type_rrf_sh((te**) &((te*) an->d[3].p)->d[2]);
    return CHK_STAT(OK);
}

void chk_z(chk *c) {
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(VD), AST_CLS(O), TYPE(VD));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(VR));
    CHK_AA(c, chk_z_vd_move, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(SG));
    CHK_AA(c, chk_z_vd_move, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(TE));
    CHK_AA(c, chk_z_vd_move, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(VR));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_z_vd, AST_CLS(Z), TYPE(_N), AST_CLS(O), TYPE(SG));
    CHK_AA(c, chk_z_vd_un, AST_CLS(Z), TYPE(_N), AST_CLS(_), TYPE(_N));
    CHK_AA(c, chk_z_z_, AST_CLS(Z), TYPE(_N), AST_CLS(Z), TYPE(_N));
    CHK_AA(c, chk_z_z_un, AST_CLS(Z), TYPE(_N), AST_CLS(Z), TYPE(UN));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(ST));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(A), TYPE(ST));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(Z), TYPE(ST));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(ET));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(E), TYPE(UN));
    CHK_AA(c, chk_z_type_h, AST_CLS(Z), TYPE(_N), AST_CLS(A), TYPE(UN));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(VD), AST_CLS(A), TYPE(SG));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(UN), AST_CLS(E), TYPE(ST));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(A), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(O), TYPE(VD));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(A), TYPE(SG));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(E), TYPE(ST));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(A), TYPE(ST));
    CHK_AA(c, chk_nop, AST_CLS(Z), TYPE(_M), AST_CLS(O), TYPE(ST));
}
