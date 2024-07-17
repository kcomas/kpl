
#pragma once

#include "kpl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK),
    TKN_STAT(FLT), // bad float format
    TKN_STAT(CHR), // no tkn for chr
    TKN_STAT(CTRL), // no # ctrl tkn
    TKN_STAT(END) // no more tkns
} tkn_stat;

typedef struct {
    size_t lno, cno, pos; // line, char, pos to get next tkn
} tkn_st;

inline void tkn_st_i(tkn_st *const ts) {
    ts->lno = ts->cno = 1;
    ts->pos = 0;
}

#define TKN_TYPE(N) TKN_TYPE##N

typedef enum {
    TKN_TYPE(NB), // \0
    TKN_TYPE(NL), // \n
    TKN_TYPE(SEMI), // ;
    TKN_TYPE(WS), // white space
    TKN_TYPE(CMT), // comment
    TKN_TYPE(VAR),
    TKN_TYPE(INT),
    TKN_TYPE(FLT),
    TKN_TYPE(STR), // "asdf"
    // built in type names
    TKN_TYPE(VD),
    TKN_TYPE(U3),
    TKN_TYPE(U4),
    TKN_TYPE(U5),
    TKN_TYPE(U6),
    TKN_TYPE(I3),
    TKN_TYPE(I4),
    TKN_TYPE(I5),
    TKN_TYPE(I6),
    TKN_TYPE(F5),
    TKN_TYPE(F6),
    TKN_TYPE(DT),
    TKN_TYPE(CR),
    TKN_TYPE(SL),
    TKN_TYPE(SG),
    TKN_TYPE(VR),
    TKN_TYPE(TE),
    TKN_TYPE(HH),
    TKN_TYPE(ST),
    TKN_TYPE(FN),
    TKN_TYPE(ER),
    TKN_TYPE(FD),
    // wraps
    TKN_TYPE(LB), // {
    TKN_TYPE(RB), // }
    TKN_TYPE(LS), // [
    TKN_TYPE(RS), // ]
    TKN_TYPE(LP), // (
    TKN_TYPE(RP), // )
    // controls #
    TKN_TYPE(IF),
    TKN_TYPE(LOP), // while and for
    TKN_TYPE(RET),
    // general ops
    TKN_TYPE(ASS), // :
    TKN_TYPE(CST), // $
    // arith
    TKN_TYPE(ADD),
    TKN_TYPE(SUB),
    TKN_TYPE(MUL),
    TKN_TYPE(DIV),
    TKN_TYPE(EXP),
    TKN_TYPE(MOD),
    // cmp
    TKN_TYPE(EQ),
    TKN_TYPE(NOT),
    TKN_TYPE(NEQ),
    TKN_TYPE(GT),
    TKN_TYPE(LN),
    TKN_TYPE(GTEQ),
    TKN_TYPE(LTEQ),
    TKN_TYPE(AND),
    TKN_TYPE(OR),
    // type specific
    TKN_TYPE(CNCT), //,
    TKN_TYPE(RW), // <<
} tkn_type;

typedef struct {
    tkn_type type;
    size_t lno, cno, pos, len; // line, char, start pos, len
} tkn;

// state, token, str, inc state
tkn_stat _tkn_get(tkn_st *const ts, tkn *const t, const char *const str, bool inc);

inline tkn_stat tkn_next(tkn_st *const ts, tkn *const t, const char *const str) {
    return _tkn_get(ts, t, str, true);
}

inline tkn_stat tkn_peek(tkn_st *const ts, tkn *const t, const char *const str) {
    return _tkn_get(ts, t, str, false);
}

// print tkn
void tkn_p(const tkn *const t, const char *const str);
