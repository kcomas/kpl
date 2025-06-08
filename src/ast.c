
#include "ast.h"

ast_stat _ast_tkn_get(ast_st *const at, bool inc) {
    ast_stat as = AST_STAT(TKN_ERR);
    at->tstat = _tkn_get(&at->ts, inc ? &at->next : &at->peek, at->str, inc);
    if (at->tstat == TKN_STAT(OK)) as = AST_STAT(OK);
    else if (at->tstat == TKN_STAT(END)) as = AST_STAT(END);
    return as;
}

extern inline ast_stat ast_tkn_next(ast_st *const at);

extern inline ast_stat ast_tkn_peek(ast_st *const at);
