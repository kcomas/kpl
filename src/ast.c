
#include "ast.h"

static tkn *inc_tkn(ast_st *const at, bool inc) {
    return inc ? &at->next : &at->peek;
}

typedef struct {
    tkn_ign flg;
    tkn_type type;
} tkn_ign_flg;

#define TIF(N) {TKN_IGN(N), TKN_TYPE(N)}

static const tkn_ign_flg tif[] = {
    TIF(NL),
    TIF(WS),
    TIF(CMT)
};

static const size_t tif_len = AL(tif);

ast_stat _ast_tkn_get(ast_st *const at, bool inc, uint8_t ign_flgs) {
    bool l = true;
    while (l) {
        l = false;
        at->tstat = _tkn_get(&at->ts, inc_tkn(at, inc), at->str, inc);
        if (at->tstat != TKN_STAT(OK)) break;
        for (size_t i = 0; i < tif_len; i++) {
            if ((tif[i].flg & ign_flgs) && inc_tkn(at, inc)->type == tif[i].type) {
                l = true;
                break;
            }
        }
    }
    if (at->tstat == TKN_STAT(OK)) return AST_STAT(OK);
    else if (at->tstat == TKN_STAT(END)) return AST_STAT(END);
    return AST_STAT(TKN_ERR);
}

extern inline ast_stat ast_tkn_next(ast_st *const at, uint8_t ign_flgs);

extern inline ast_stat ast_tkn_peek(ast_st *const at, uint8_t ign_flgs);
