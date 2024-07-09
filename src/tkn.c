
#include "tkn.h"

extern inline void tkn_st_init(tkn_st *const ts);

typedef struct {
    tkn_type type;
    char c2[2];
} type_var;

#define TV(N) {TKN_TYPE(N), #N}

static const type_var tv[] = {
    TV(VD),
    TV(U3),
    TV(U4),
    TV(U5),
    TV(U6),
    TV(I3),
    TV(I4),
    TV(I5),
    TV(I6),
    TV(F5),
    TV(F6),
    TV(SG),
    TV(SL),
    TV(VR),
    TV(TE),
    TV(HH),
    TV(ST),
    TV(FN),
    TV(ER),
    TV(FD)
};

static const size_t tv_len = sizeof(tv) / sizeof(tv[0]);

static tkn_stat var(tkn *const t, const char *const str) {
    char c = str[t->pos];
    while (isalpha(c) || isdigit(c)) c = str[t->pos + ++t->len];
    t->type = TKN_TYPE(VAR);
    if (t->len == 2) {
        for (size_t i = 0; i < tv_len; i++) {
            if (str[t->pos] == tv[i].c2[0] && str[t->pos] == tv[i].c2[1]) {
                t->type = tv[i].type;
                break;
            }
        }
    }
    return TKN_STAT(OK);
}

static tkn_stat num(tkn *const t, const char *const str) {
    bool isf = false;
    for (;;) {
        char c = str[t->pos];
        if (isdigit(c)) t->pos++;
        else if (c == '.') {
            if (isf) {
                return TKN_STAT(INV_FLT);
            } else {
                isf = true;
                t->pos++;
            }
        } else break;
    }
    return TKN_STAT(OK);
}

#define T_TYP_LEN(T) t->type = TKN_TYPE(T); \
    t->len++

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
            case '\0':
                T_TYP_LEN(NB);
                memset(ts, 0, sizeof(ts));
                break;
            case '\n':
                T_TYP_LEN(NL);
                break;
            default:
                return TKN_STAT(INV_CHR);
        }
    }
    if (inc) {
        if (t->type == TKN_TYPE(NL)) {
            ts->lno++;
            ts->cno = 1;
        } else ts->cno = t->cno + t->len;
        ts->pos = t->pos + t->len;
    }
    return TKN_STAT(OK);
}

extern inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str);

extern inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str);

static const char *const tkn_type_str[] = {
    // TODO
};

void tkn_p(const tkn *const t, const char *const str) {
    const char *const type = "INVALID";
    printf("type: %s, lno: %lu, cno: %lu, str: ", type, t->lno, t->cno);
    for (size_t i = 0; i < t->len; i++) {
        putchar(str[t->pos + i]);
    }
}
