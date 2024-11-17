
#include "tkn.h"

static const char *const tss[] = {
    "OK",
    "FLT",
    "CHR",
    "CTRL",
    "CMD",
    "END"
};

const char *tkn_stat_str(tkn_stat tstat) {
    const char *s = "INVALID_TKN_STAT";
    if (tstat >= TKN_STAT(OK) && tstat <= TKN_STAT(END)) s = tss[tstat];
    return s;
}

extern inline void tkn_st_i(tkn_st *const ts, tdr *const r);

extern inline tkn_stat tkn_er(tkn_st *const ts, const char *const fnn, tkn_stat tstat);

#define TKN_ER(TS, TSTAT) tkn_er(TS, __func__, TKN_STAT(TSTAT))

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
    TV(FD),
    TV(TD)
};

static const size_t tv_len = AL(tv);

#define TKN_C(c, C) case c: \
    t->type = TKN_TYPE(C); \
    break

static tkn_stat tkn_var(tkn *const t, const char *const str) {
    char c = str[t->pos + t->len]; // for symbols
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
            TKN_C('D', DEB);
        }
    }
    return TKN_STAT(OK);
}

static tkn_stat tkn_num(tkn_st *const ts, tkn *const t, const char *const str) {
    t->type = TKN_TYPE(INT);
    for (;;) {
        char c = str[t->pos + t->len];
        if (isdigit(c)) t->len++;
        else if (c == '.') {
            if (t->type == TKN_TYPE(FLT)) {
                return TKN_ER(ts, FLT);
            } else {
                t->type = TKN_TYPE(FLT);
                t->len++;
            }
        } else break;
    }
    return TKN_ER(ts, OK);
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
    t->str = str;
    tkn_stat tsr;
    if (isalpha(str[t->pos])) {
        if ((tsr = tkn_var(t, str)) != TKN_STAT(OK)) return tsr;
    } else if (isdigit(str[t->pos])) {
        if ((tsr = tkn_num(ts, t, str)) != TKN_STAT(OK)) return tsr;
    } else {
        switch (str[t->pos]) {
            case '\0':
                T_TYP_LEN(NB);
                return TKN_ER(ts, END);
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
            case '`':
                t->len++;
                if ((tsr = tkn_var(t, str)) != TKN_STAT(OK)) return tsr;
                t->type = TKN_TYPE(SYM);
                break;
            T_ONE_C('\'', TC);
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
                        return TKN_ER(ts, CTRL);
                }
                break;
            T_ONE_C(':', AGN);
            T_ONE_C('$', CST);
            case '\\':
                t->len++;
                switch (str[t->pos + t->len]) {
                    T_ONE_C('c', CLSE);
                    T_ONE_C('d', DEL);
                    T_ONE_C('l', LD);
                    T_ONE_C('h', VH);
                    default:
                        return TKN_ER(ts, CMD);
                }
                break;
            T_ONE_C('+', ADD);
            T_ONE_C('-', SUB);
            T_ONE_C('*', MUL);
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
                return TKN_ER(ts, CHR);
        }
    }
    if (inc) tkn_st_u(ts, t);
    return TKN_ER(ts, OK);
}

extern inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str);

extern inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str);

static const char *const tkn_type_str[] = {
    "NB", // \0
    "NL", // \n
    "SEMI", // ;
    "WS", // white space
    "CMT", // comment
    "VAR",
    "SYM", // symbol literal
    "INT",
    "FLT",
    "STR", // "asdf"
    // const
    "TRUE",
    "FALSE",
    "SELF",
    "DEB",
    // TODO LOCAL
    // built in type names
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
    "TD",
    // erros
    "TC", // try / catch
    // wraps
    "LB", // {
    "RB", // }
    "LS", // [
    "RS", // ]
    "LP", // (
    "RP", // )
    // controls #
    "IF",
    "LOP", // while and for
    "RET",
    // general ops
    "AGN", // :
    "CST", // $
    "CLSE",
    "DEL", // \d
    "LD", // \l
    "VH", // \h
    // arith
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "EXP",
    "MOD",
    // cmp
    "EQ",
    "NOT",
    "GT",
    "LT",
    "AND",
    "OR",
    // type specific
    "CNCT", //,
    "RW" // <<

};

 // line, char, type, str
void tkn_p(const tkn *const t) {
    const char *type = "INVALID";
    if (t->type >= TKN_TYPE(NB) && t->type <= TKN_TYPE(RW)) type = tkn_type_str[t->type];
    printf("%lu,%lu,%s,", t->lno, t->cno, type);
    if (t->type == TKN_TYPE(NB)) printf("\\0");
    else if (t->type == TKN_TYPE(NL)) printf("\\n");
    else if (t->type == TKN_TYPE(WS)) printf("\\s[%lu]", t->len);
    else for (size_t i = 0; i < t->len; i++) putchar(t->str[t->pos + i]);
}

#ifndef MAX_NUM_LEN
    #define MAX_NUM_LEN 20
#endif

int64_t tkn_to_int64_t(const tkn *const t) {
    char istr[MAX_NUM_LEN], *eptr;
    memset(istr, '\0', MAX_NUM_LEN);
    memcpy(istr, t->str + t->pos, t->len);
    return strtoll(istr, &eptr, 10);
}

double tkn_to_double(const tkn *const t) {
    char istr[MAX_NUM_LEN], *eptr;
    memset(istr, '\0', MAX_NUM_LEN);
    memcpy(istr, t->str + t->pos, t->len);
    return strtod(istr, &eptr);
}

char *tkn_to_c_str(al *const a, const tkn *const t) {
    size_t len = t->len - 1;
    char *str = ala(a, len-- * sizeof(char));
    size_t tpos = 0, spos = 0;
    char c;
    while (tpos < len) {
        c = t->str[t->pos + 1 + tpos];
        if (c == '\\') {
            if (++tpos >= len) {
                alf(str);
                return NULL;
            }
            c = t->str[t->pos + 1 + tpos];
            switch (c) {
                case 'n':
                    str[spos++] = '\n';
                    break;
                case 't':
                    str[spos++] = '\t';
                    break;
                default:
                    alf(str);
                    return NULL;
            }
        } else str[spos++] = c;
        tpos++;
    }
    return str;
}
