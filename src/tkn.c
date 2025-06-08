
#include "tkn.h"

extern inline void tkn_st_init(tkn_st *tkns);

tkn_stat _tkn_get(tkn_st *const tkns, tkn *const t, const char *const str, bool inc) {
    if (tkns->lno < t->lno || tkns->cno < t->cno || tkns->pos < t->spos) return TKN_STAT(END);
    t->lno = tkns->lno;
    t->cno = tkns->cno;
    t->pos = tkns->pos;
    t->len = 1;
    if (isalpha(str[tkns->pos])) {

    } else if (isdigit(str[tkns->pos])) {

    } else {
        switch (str[tkns->pos]) {

        }
    }
    if (inc) {
        tkns->lno = t->lno;
        tkns->cno = t->cno;
        tkns->pos = t->pos + t->len;
    }
    return TKN_STAT(OK);
}

extern inline tkn_stat tkn_next(tkn_st *const tkns, tkn *const t, const char *const str);

extern inline tkn_stat tkn_peek(tkn_st *const tkns, tkn *const t, const char *const str);
