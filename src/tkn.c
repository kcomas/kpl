
#include "tkn.h"

extern inline void tkn_st_init(tkn_st *const ts);

typedef struct {
    tkn_type type;
    char c2[2];
} type_var;

static const type_var tv[] = {
    {TKN_TYPE(VD), "VD"}
};

static const size_t tv_len = sizeof(tv) / sizeof(tv[0]);

static tkn_stat var(tkn *const t, const char *const str) {
    char c = str[t->pos];
    while (isalpha(c) || isdigit(c)) c = str[t->pos + ++t->len];
    t->type = TKN_TYPE(VAR);
    if (t->len == 2) {
        for (size_t i = 0; i < tv_len; i++) {

        }
    }
    return TKN_STAT(OK);
}

static tkn_stat num(tkn *const t, const char *const str) {
    return TKN_STAT(OK);
}

tkn_stat _tkn_get(tkn_st *const ts, tkn *const t, const char *const str, bool inc) {
    if (ts->lno < t->lno || ts->cno < t->cno || ts->pos < t->pos) return TKN_STAT(END);
    t->lno = ts->lno;
    t->cno = ts->cno;
    t->pos = ts->pos;
    t->len = 0;
    tkn_stat tsr;
    if (isalpha(str[t->pos])) {
        if ((tsr = var(t, str)) != TKN_STAT(OK)) return tsr;
    } else if (isdigit(str[t->pos])) {
        if ((tsr = num(t, str)) != TKN_STAT(OK)) return tsr;
    } else {
        switch (str[t->pos]) {

        }
    }
    if (inc) {
        ts->lno = t->lno;
        ts->cno = t->cno + t->len;
        ts->pos = t->pos + t->len;
    }
    return TKN_STAT(OK);
}

extern inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str);

extern inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str);
