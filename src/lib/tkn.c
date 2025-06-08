
#include "tkn.h"

void tkn_st_i(tkn_st *const ts, const char *str, tbl *t) {
    ts->lno = ts->cno = 1;
    ts->pos = ts->ml = 0;
    ts->str = str;
    ts->t = t;
}

void tkn_st_inc(tkn_st *const ts, bool incl) {
    if (incl) {
        ts->lno++;
        ts->cno = 1;
    } else ts->cno++;
    ts->pos++;
}

void tkn_st_f(tkn_st *ts) {
    tbl_f(ts->t);
}
