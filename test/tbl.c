
#include "../src/tbl.h"

typedef struct {
    uint8_t i;
    char c[2];
} ex;

void ex_f(void *data) {
    (void) data;
}

void ex_p(void *data) {
    (void) data;
}

#define EXT 26

int main(void) {
    ex tests[EXT];
    tbl *tl =  tbl_i(TBL_START_SIZE);
    tbl_stat st;
    for (uint8_t i = 0; i < EXT; i++) {
        tests[i] = (ex) {i, {'a' + i, '\0'}};
        tbl_itm *ti;
        if ((st = tbl_op(&tl, tests[i].c, &tests[i], &ti, &ex_f, TBL_OP_FLG(AD))) != TBL_STAT(OK)) return st;
    }
    printf("-----BUCKETS-----\n");
    tbl_bucksp(tl, &ex_p);
    printf("-----------------\n");
    printf("-----LIST--------\n");
    tbl_lstp(tl, &ex_p);
    printf("-----------------\n");
    tbl_f(tl, &ex_f);
    return 0;
}
