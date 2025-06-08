
#include "../src/tbl.h"

typedef struct {
    uint8_t i;
    char c[3];
} ex;

void ex_f(void *data) {
    (void) data;
}

void ex_p(void *data) {
    if (!data) return;
    ex *e = (ex*) data;
    printf("{%d %s}", e->i, e->c);
}

#define EXT 26

int main(void) {
    ex tests[EXT];
    tbl *tl =  tbl_i(TBL_I_SIZE);
    tbl_stat st;
    tbl_itm *ti;
    for (uint8_t i = 0; i < EXT; i++) {
        tests[i] = (ex) {i, {'a' + i, 'z' - i, '\0'}};
        if ((st = tbl_op(&tl, tests[i].c, &tests[i], &ti, &ex_f, TBL_OP_FLG(AD))) != TBL_STAT(OK)) return st;
    }
    printf("----START TBL TEST---\n");
    printf("-----BUCKETS---------\n");
    tbl_bucksp(tl, &ex_p);
    printf("---------------------\n");
    printf("-----LIST------------\n");
    tbl_lstp(tl, &ex_p);
    printf("---------------------\n");
    printf("-----REMOVE LIST-----\n");
    if ((st = tbl_op(&tl, "az", NULL, &ti, &ex_f, TBL_OP_FLG(RM))) != TBL_STAT(OK)) return st;
    if ((st = tbl_op(&tl, "by", NULL, &ti, &ex_f, TBL_OP_FLG(RM))) != TBL_STAT(OK)) return st;
    if ((st = tbl_op(&tl, "cx", NULL, &ti, &ex_f, TBL_OP_FLG(RM))) != TBL_STAT(OK)) return st;
    tbl_bucksp(tl, &ex_p);
    tbl_lstp(tl, &ex_p);
    printf("---------------------\n");
    printf("-----ADD LIST--------\n");
    if ((st = tbl_op(&tl, "asdf", &tests[0], &ti, &ex_f, TBL_OP_FLG(AD))) != TBL_STAT(OK)) return st;
    tbl_bucksp(tl, &ex_p);
    tbl_lstp(tl, &ex_p);
    printf("---------------------\n");
    printf("-----UPDATE LIST--------\n");
    if ((st = tbl_op(&tl, "asdf", &tests[5], &ti, &ex_f, TBL_OP_FLG(FD))) != TBL_STAT(OK)) return st;
    tbl_bucksp(tl, &ex_p);
    tbl_lstp(tl, &ex_p);
    printf("---------------------\n");
    printf("----END TBL TEST-----\n");
    tbl_f(tl, &ex_f);
    return 0;
}
