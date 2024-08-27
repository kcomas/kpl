
#include "tkn.h"

extern inline void tkn_st_i(tkn_st *const ts);

extern inline void tkn_st_u(tkn_st *const ts, const tkn *const t);

typedef struct {
    tkn_type type;
    char c2[2];
} type_var;

#define TV(N) {TKN_TYPE(N), #N}

static const type_var tv[] = {
    TV(VD),
    TV(BL),
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

static const size_t tv_len = AL(tv);

#define TKN_C(c, C) case c: \
    t->type = TKN_TYPE(C); \
    break

static tkn_stat tkn_var(tkn *const t, const char *const str) {
    char c = str[t->pos];
    while (isalpha(c) || isdigit(c)) c = str[t->pos + ++t->len];
    t->type = TKN_TYPE(VAR);
    if (t->len == 2) {
        for (size_t i = 0; i < tv_len; i++) {
            if (str[t->pos] == tv[i].c2[0] && str[t->pos + 1] == tv[i].c2[1]) {
                t->type = tv[i].type;
                break;
            }
        }
    } else if (t->len == 1) {
        switch (str[t->pos]) {
            TKN_C('T', TRUE);
            TKN_C('F', FALSE);
            TKN_C('S', SELF);
        }
    }
    return TKN_STAT(OK);
}

static tkn_stat tkn_num(tkn *const t, const char *const str) {
    t->type = TKN_TYPE(INT);
    for (;;) {
        char c = str[t->pos + t->len];
        if (isdigit(c)) t->len++;
        else if (c == '.') {
            if (t->type == TKN_TYPE(FLT)) {
                return TKN_STAT(FLT);
            } else {
                t->type = TKN_TYPE(FLT);
                t->len++;
            }
        } else break;
    }
    return TKN_STAT(OK);
}

#define T_TYP_LEN(T) t->type = TKN_TYPE(T); \
    t->len++

#define T_ONE_C(C, T) case C: \
    T_TYP_LEN(T); \
    break

#define T_TWO_C(C1, T1, C2, T2) case C1: \
    T_TYP_LEN(T1); \
    if (str[t->pos + t->len] == C2) { \
        T_TYP_LEN(T2); \
    } \
    break


tkn_stat _tkn_get(tkn_st *const ts, tkn *const t, const char *const str, bool inc) {
    t->lno = ts->lno;
    t->cno = ts->cno;
    t->pos = ts->pos;
    t->len = 0;
    tkn_stat tsr;
    if (isalpha(str[t->pos])) {
        if ((tsr = tkn_var(t, str)) != TKN_STAT(OK)) return tsr;
    } else if (isdigit(str[t->pos])) {
        if ((tsr = tkn_num(t, str)) != TKN_STAT(OK)) return tsr;
    } else {
        switch (str[t->pos]) {
            case '\0':
                T_TYP_LEN(NB);
                return TKN_STAT(END);
            T_ONE_C('\n', NL);
            T_ONE_C(';', SEMI);
            case ' ':
                T_TYP_LEN(WS);
                while (str[t->pos + t->len] == ' ') t->len++;
                break;
            case '"':
                T_TYP_LEN(STR);
                while (str[t->pos + t->len] != '"') t->len++;
                t->len++;
                break;
            T_ONE_C('{', LB);
            T_ONE_C('}', RB);
            T_ONE_C('[', LS);
            T_ONE_C(']', RS);
            T_ONE_C('(', LP);
            T_ONE_C(')', RP);
            case '#':
                t->len++;
                switch (str[t->pos + t->len]) {
                    T_ONE_C('?', IF);
                    T_ONE_C('@', LOP);
                    T_ONE_C(';', RET);
                    default:
                        return TKN_STAT(CTRL);
                }
                break;
            T_ONE_C(':', ASS);
            T_ONE_C('$', CST);
            T_ONE_C('+', ADD);
            T_ONE_C('-', SUB);
            case '/':
                T_TYP_LEN(DIV);
                if (str[t->pos + t->len] == '/') {
                    t->type = TKN_TYPE(CMT);
                    while (str[t->pos + t->len] != '\n' && str[t->pos + t->len] != '\0') t->len++;
                }
                break;
            T_ONE_C('=', EQ);
            T_ONE_C('!', NOT);
            T_ONE_C('>', GT);
            T_TWO_C('<', LT, '<', RW);
            T_ONE_C('|', OR);
            T_ONE_C(',', CNCT);
            default:
                return TKN_STAT(CHR);
        }
    }
    if (inc) tkn_st_u(ts, t);
    return TKN_STAT(OK);
}

extern inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str);

extern inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str);

static const char *const tkn_type_str[] = {
    "NB",
    "NL",
    "SEMI",
    "WS",
    "CMT",
    "VAR",
    "INT",
    "FLT",
    "STR",
    "TRUE",
    "FALSE",
    "SELF",
    "VD",
    "BL",
    "U3",
    "U4",
    "U5",
    "U6",
    "I3",
    "I4",
    "I5",
    "I6",
    "F5",
    "F6",
    "DT",
    "CR",
    "SL",
    "SG",
    "VR",
    "TE",
    "HH",
    "ST",
    "FN",
    "ER",
    "FD",
    "LB",
    "RB",
    "LS",
    "RS",
    "LP",
    "RP",
    "IF",
    "LOP",
    "RET",
    "ASS",
    "CST",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "EXP",
    "MOD",
    "EQ",
    "NOT",
    "GT",
    "LT",
    "AND",
    "OR",
    "CNCT",
    "RW"
};

 // line, char, type, str
void tkn_p(const tkn *const t, const char *const str) {
    const char *type = "INVALID";
    if (t->type >= TKN_TYPE(NB) && t->type <= TKN_TYPE(RW)) type = tkn_type_str[t->type];
    printf("%lu,%lu,%s,", t->lno, t->cno, type);
    if (t->type == TKN_TYPE(NB)) printf("\\0");
    else if (t->type == TKN_TYPE(NL)) printf("\\n");
    else if (t->type == TKN_TYPE(WS)) printf("\\s[%lu]", t->len);
    else for (size_t i = 0; i < t->len; i++) putchar(str[t->pos + i]);
}
