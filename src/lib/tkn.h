
#pragma once

#include "def.h"
#include "mc.h"
#include "tbl.h"

#define TKN_STAT(N) TKN_STAT_##N

typedef enum {
    TKN_STAT(OK), // found tkn
    TKN_STAT(INV),
    TKN_STAT(END) // end of str
} tkn_stat;

typedef tbl *tkn_tbl_i(void);

typedef struct _tkn tkn;

// on match
typedef tkn_stat tkn_pf(tkn *t, te *m);

// match te[u5(cno)|u4(lno)|u4(id);u5(start)|u5(end)]

// id
uint16_t tkn_m_g_i(const te *m);

void tkn_m_s_i(te *m, uint16_t i);

// lno
uint16_t tkn_m_g_l(const te *m);

void tkn_m_s_l(te *m, uint16_t l);

// cno
uint32_t tkn_m_g_c(const te *m);

void tkn_m_s_c(te *m, uint32_t c);

// start pos
uint32_t tkn_m_g_s(const te *m);

void tkn_m_s_s(te *m, uint32_t s);

// end pos
uint32_t tkn_m_g_e(const te *m);

void tkn_m_s_e(te *m, uint32_t e);

#define TOKEN(N) TOKEN_##N

typedef enum {
    TOKEN(UN) = 0, // unknown
    TOKEN(_),
    TOKEN(USR) = 1000
} token;

typedef struct _tkn {
    uint16_t idc, lno;
    uint32_t cno, pos; // id counter
    ssize_t r;
    const alfr *af, *ta;
    tkn_tbl_i *ttif;
    tkn_pf *df; // called when nothing matches
    tbl *t; // trie
    mc *s;
} tkn;

tkn *tkn_i(const alfr *af, const alfr *ta, tkn_tbl_i ttif, tkn_pf df, mc *s);

tkn *tkn_i_tkn(const tkn *t, mc *s);

// give zero for an assigned id
uint16_t tkn_a(tkn *t, uint16_t tid, const char *s, tkn_pf *pf);

tkn_stat tkn_n(tkn *t, te *m);

void tkn_s(tkn *t, te *m);

void tkn_f(tkn *t);
