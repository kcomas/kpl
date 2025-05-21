
#include "../chk.h"

void chk_arith(chk *c) {
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(A), TYPE(I6), AST_CLS(A), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lers_us_u, AST_CLS(O), TYPE(_N), OC(ADD), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(ADDA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_mon, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(_), TYPE(_N), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lers_us_u, AST_CLS(O), TYPE(_N), OC(SUB), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(SUBA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(E), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(S), TYPE(I6), AST_CLS(O), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(U6), AST_CLS(A), TYPE(U6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MUL), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(I6), AST_CLS(E), TYPE(I6));
    CHK_AA(c, chk_op_lers_fs_f, AST_CLS(O), TYPE(_N), OC(MULA), TYPE(_A), AST_CLS(E), TYPE(F6), AST_CLS(S), TYPE(I6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(DIV), TYPE(_A), AST_CLS(O), TYPE(F6), AST_CLS(O), TYPE(F6));
    CHK_AA(c, chk_op_lr_teq, AST_CLS(O), TYPE(_N), OC(DIV), TYPE(_A), AST_CLS(O), TYPE(I6), AST_CLS(S), TYPE(I6));
}
