
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

extern inline type_node *type_node_i(type t, ast *const a);

extern inline void type_node_f(type_node *tn);

extern inline val_node *val_node_i(type t);

extern inline void val_node_f(val_node *v);

extern inline op_node *op_node_i(op_type ot);

extern inline void op_node_f(op_node *on);

extern inline lst_itm *lst_itm_i(ast *const a);

extern inline void lst_itm_f(lst_itm *itm, void *fn);

extern inline lst_node *lst_node_i(type t);

extern inline void lst_node_a(lst_node *const lst, ast *const a);

extern inline void lst_node_f(lst_node *lst);

extern inline if_itm *if_itm_i(ast *const cond, lst_node *const body);

extern inline void if_itm_f(if_itm *im, void *fn);

extern inline if_node *if_node_i(void);

extern inline void if_node_a(if_node *const in, ast* const cond, lst_node *const body);

extern inline void if_node_f(if_node *in);

extern inline fn_node *fn_node_i(fn_node *const par);

void fn_node_tbl_data_f(void *data) {
    var_node_f((var_node*) data);
}

extern inline void fn_node_f(fn_node *fn);

extern inline call_node *call_node_i(ast *const tgt, lst_node *const args);

extern inline void call_node_f(call_node *cn);

extern inline ret_node *ret_node_i(ast *const a);

extern inline void ret_node_f(ret_node *r);

extern inline var_node *var_node_i(fn_node *const fns, const tkn *const t, const char *const str);

extern inline void var_node_f(var_node *vn);

#define AST_F_CASE(T, N, F) case AST_TYPE(T): \
    FNNF(a->n.N, F); \
    break

inline ast *ast_i(ast_type at, node const n, tkn *t);

void ast_f(ast *a) {
    switch (a->at) {
        AST_F_CASE(TYPE, tn, type_node_f);
        AST_F_CASE(VAL, val, val_node_f);
        AST_F_CASE(OP, op, op_node_f);
        AST_F_CASE(LST, lst, lst_node_f);
        AST_F_CASE(IF, in, if_node_f);
        AST_F_CASE(FN, fn, fn_node_f);
        AST_F_CASE(CALL, cn, call_node_f);
        AST_F_CASE(RET, ret, ret_node_f);
        case AST_TYPE(VAR):
            break; // var freed in fn sym tbl
    }
    free(a);
}

ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, lst_node *cl);
