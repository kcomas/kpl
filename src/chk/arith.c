
#include "../chk.h"

static chk_stat chk_op_dm_s_i6(chk *c, te *an, err **e) {
    te *r = an->d[6].p;
    if (r->d[4].i6 == 0) return chk_err(c, an, e, "chk x/0");
    return chk_op_lr_teq(c, an, e);
}

static chk_stat chk_op_dm(chk *c, te *an, err **e) {
    chk_stat stat;
    if ((stat = chk_er_pn_fn(c, an, e)) != CHK_STAT(OK)) return stat;
    return chk_op_lr_teq(c, an, e);
}

void chk_arith(chk *c) {
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(O), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_l_z_r_z_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(Z), TYPE(I6), AST_CLS(Z), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_u, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(S), TYPE(F6), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(S), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_op_l_r_z, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(Z), TYPE(F6));
    CHK_AA(c, chk_op_lers_us_u, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(O), TYPE(U6), AST_CLS(O), TYPE(U6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(A), TYPE(F6), AST_CLS(A), TYPE(F6));
    CHK_AA(c, chk_op_mon, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_mon, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_u, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(S), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(S), TYPE(F6), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUBA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(A), TYPE(U6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_l_z_r, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(Z), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lers_fs_f, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(F6));
    CHK_AA(c, chk_op_dm_s_i6, AST_CLS(O), TYPE(_N), OC(DIV), TYPE(_A), AST_CLS(O), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_dm, AST_CLS(O), TYPE(_N), OC(DIV), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_dm_s_i6, AST_CLS(O), TYPE(_N), OC(MOD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
}
