
#include "ast.h"

extern inline void ast_st_i(ast_st *const as, char *const str);

static tkn *inc_tkn(ast_st *const as, bool inc) {
    return inc ? &as->next : &as->peek;
}

typedef struct {
    tkn_flg flg;
    tkn_type type;
} tkn_flg_type;

#define TFT(N) {TKN_FLG(N), TKN_TYPE(N)}

static const tkn_flg_type tft[] = {
    TFT(NB),
    TFT(NL),
    TFT(SEMI),
    TFT(WS),
    TFT(CMT),
    TFT(RB),
    TFT(RS),
    TFT(RP)
};

static const size_t tft_len = AL(tft);

static bool flg_mtch(tkn_type type, uint8_t flgs) {
    for (size_t i = 0; i < tft_len; i++) {
        if ((tft[i].flg & flgs) && type == tft[i].type) {
            return true;
        }
    }
    return false;
}

ast_stat _ast_tkn_get(ast_st *const as, bool inc, uint8_t ign_flgs) {
    bool l = true;
    while (l) {
        l = false;
        as->tstat = _tkn_get(&as->ts, inc_tkn(as, inc), as->str, inc);
        if (as->tstat != TKN_STAT(OK)) break;
        if (flg_mtch(inc_tkn(as, inc)->type, ign_flgs)) l = true;
    }
    if (as->tstat == TKN_STAT(OK)) return AST_STAT(OK);
    else if (as->tstat == TKN_STAT(END)) return AST_STAT(END);
    return AST_STAT(TKN_ERR);
}

extern inline ast_stat ast_tkn_next(ast_st *const as, uint8_t ign_flgs);

extern inline ast_stat ast_tkn_peek(ast_st *const as, uint8_t ign_flgs);

static const char *const tgs[] = {
    "INT",
    "FLT",
    "STR",
    "VD",
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
    "FD"
};

const char *type_get_str(type t) {
    const char *s = "INVALID";
    if (t >= TYPE(INT) && t <= TYPE(FD)) s = tgs[t];
    return s;
}

extern inline type_node *type_node_i(type t, ast *const a);

extern inline void type_node_p(const ast_st *const st, const type_node *const tn, size_t idnt);

extern inline void type_node_f(type_node *tn);

extern inline val_node *val_node_i(type t);

extern inline void val_node_p(val_node *v);

extern inline void val_node_f(val_node *v);

extern inline op_node *op_node_i(op_type ot);

static const char *const op_type_str[] = {
    "ASS",
    "CST"
};

void op_node_p(const ast_st *const as, const op_node *const op, size_t idnt) {
    const char *type = "INVALID";
    if (op->ot >= OP_TYPE(ASS) && op->ot <= OP_TYPE(CST)) type = op_type_str[op->ot];
    printf("%s\n", type);
    PCX(' ', idnt);
    type_node_p(as, op->ret, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("L-");
    ast_p(as, op->l, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("R-");
    ast_p(as, op->r, idnt);
}

extern inline void op_node_f(op_node *on);

extern inline lst_itm *lst_itm_i(ast *const a);

extern inline void lst_itm_p(const ast_st *const as, const lst_itm *const li, void *fn, size_t idnt);

extern inline void lst_itm_f(lst_itm *li, void *fn);

extern inline lst_node *lst_node_i(type t);

extern inline void lst_node_a(lst_node *const lst, ast *const a);

extern inline void lst_node_p(const ast_st *const as, const lst_node *const lst, size_t idnt);

extern inline void lst_node_f(lst_node *lst);

extern inline if_itm *if_itm_i(ast *const cond, lst_node *const body);

extern inline void if_itm_f(if_itm *im, void *fn);

extern inline if_node *if_node_i(void);

extern inline void if_node_a(if_node *const in, ast* const cond, lst_node *const body);

extern inline void if_node_f(if_node *in);

extern inline fn_node *fn_node_i(fn_node *const par);

extern inline void fn_node_p(const ast_st *const as, const fn_node *const fn, size_t idnt);

void fn_node_tbl_data_f(void *data) {
    var_node_f((var_node*) data);
}

extern inline void fn_node_f(fn_node *fn);

extern inline call_node *call_node_i(ast *const tgt, lst_node *const args);

extern inline void call_node_f(call_node *cn);

extern inline ret_node *ret_node_i(ast *const a);

extern inline void ret_node_f(ret_node *r);

#ifndef MAX_VAR_LEN
    #define MAX_VAR_LEN 20
#endif

var_node *var_node_i(fn_node *const fns, const tkn *const t, const char *const str) {
    static char vstr[MAX_VAR_LEN];
    memset(vstr, '\0', MAX_VAR_LEN);
    memcpy(vstr, str + t->pos, t->len);
    // check if this var node exists
    fn_node *scope = fns;
    tbl_itm *ti;
    while (scope) {
        if (tbl_op(&scope->tl, vstr, NULL, &ti, NULL, TBL_OP_FLG(FD)) == TBL_STAT(OK)) return (var_node*) ti->data;
        scope = scope->par;
    }
    if (!scope) {
        var_node *vn = calloc(1, sizeof(var_node) + t->len + 1);
        vn->id = fns->idc++;
        vn->fns = fns;
        memcpy(vn->str, str + t->pos, t->len);
        if (tbl_op(&fns->tl, vstr, vn, &ti, NULL, TBL_OP_FLG(AD)) != TBL_STAT(OK)) {
            var_node_f(vn);
            return NULL;
        }
        return vn;
    }
    return NULL;
}

extern inline void var_node_p(const ast_st *const as, const var_node *const var, size_t idnt);

extern inline void var_node_f(var_node *vn);

#define AST_F_CASE(T, N, F) case AST_TYPE(T): \
    FNNF(a->n.N, F); \
    break

extern inline ast *ast_i(ast_type as, node const n, tkn *t);

#ifndef IDNT_ADD
    #define IDNT_ADD 4
#endif

static const char *const ast_type_str[] = {
    "TYPE",
    "VAL",
    "OP",
    "LST",
    "IF",
    "FN",
    "CALL",
    "RET",
    "VAR"
};

void ast_p(const ast_st *const as, const ast *const a, size_t idnt) {
    if (!a) {
        printf("NULL");
        return;
    }
    const char *type = "INVALID";
    if (a->at >= AST_TYPE(TYPE) && a->at <= AST_TYPE(VAR)) type = ast_type_str[a->at];
    printf("%s", type);
    putchar('|');
    tkn_p(&a->t, as->str);
    putchar('|');
    idnt += IDNT_ADD;
    switch (a->at) {
        case AST_TYPE(TYPE): return type_node_p(as, a->n.tn, idnt);
        case AST_TYPE(VAL): return val_node_p(a->n.val);
        case AST_TYPE(OP): return op_node_p(as, a->n.op, idnt);
        case AST_TYPE(LST): return lst_node_p(as, a->n.lst, idnt);
        case AST_TYPE(IF): return; // TODO
        case AST_TYPE(FN): return fn_node_p(as, a->n.fn, idnt);
        case AST_TYPE(CALL):
        case AST_TYPE(RET):
            return;
        case AST_TYPE(VAR): return var_node_p(as, a->n.var, idnt);
    }
}

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

#define TYPE_NA_CASE(T) case TKN_TYPE(T): \
    if (*a) return AST_STAT(TYPE_A_NN); \
    *a = ast_i(AST_TYPE(TYPE), (node) { .tn = type_node_i(TYPE(T), NULL) }, &as->next); \
    return ast_parse_stmt(as, fns, a, stp_flgs)

#define VAL_CASE(T) case TKN_TYPE(T): \
    if (*a) return AST_STAT(VAL_A_NN); \
    *a = ast_i(AST_TYPE(VAL), (node) { .val = val_node_i(TYPE(T)) }, &as->next); \
    return ast_parse_stmt(as, fns, a, stp_flgs)

static ast_stat ast_parse_op(op_type ot, ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs) {
    // TODO parse call form peek (
    op_node *op = op_node_i(ot);
    op->l = *a;
    *a = ast_i(AST_TYPE(OP), (node) { .op = op }, &as->next);
    return ast_parse_stmt(as, fns, &op->r, stp_flgs);
}

#define OP_CASE(T) case TKN_TYPE(T): \
    return ast_parse_op(OP_TYPE(T), as, fns, a, stp_flgs)

static ast_stat ast_parse_fn(ast_st *const as, fn_node *const par, ast **a) {
    // at first arg
}

ast_stat ast_parse_stmt(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs) {
    ast_stat astat;
    if ((astat = ast_tkn_next(as, TFWC)) != AST_STAT(OK)) return astat;
    if (flg_mtch(as->next.type, stp_flgs)) return AST_STAT(OK);
    ast *atmp;
    var_node *var;
    tkn ttmp;
    switch (as->next.type) {
        case TKN_TYPE(VAR):
            if (*a) return AST_STAT(VAR_A_NN);
            if (!(var = var_node_i(fns, &as->next, as->str))) {
                var_node_f(var);
                return AST_STAT(VAR_I_ERR);
            }
            *a = ast_i(AST_TYPE(VAR), (node) { .var = var }, &as->next);
            return ast_parse_stmt(as, fns, a, stp_flgs);
        VAL_CASE(INT);
        VAL_CASE(FLT);
        VAL_CASE(STR);
        TYPE_NA_CASE(VD);
        TYPE_NA_CASE(U3);
        TYPE_NA_CASE(U4);
        TYPE_NA_CASE(U5);
        TYPE_NA_CASE(U6);
        TYPE_NA_CASE(I3);
        TYPE_NA_CASE(I4);
        TYPE_NA_CASE(I5);
        TYPE_NA_CASE(I6);
        TYPE_NA_CASE(F5);
        TYPE_NA_CASE(F6);
        TYPE_NA_CASE(DT);
        TYPE_NA_CASE(CR);
        TYPE_NA_CASE(SL);
        TYPE_NA_CASE(SG);
        // TODO TYPES
        case TKN_TYPE(FN):
            if (*a) return AST_STAT(TYPE_A_NN);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->next.type != TKN_TYPE(LP)) return AST_STAT(INV_TYPE_LST_INIT);
            atmp = ast_i(AST_TYPE(LST), (node) { .lst = lst_node_i(TYPE(TE)) }, &as->next);
            if ((astat = ast_parse_stmts(as, fns, atmp->n.lst, TKN_FLG(SEMI), TKN_FLG(RP))) != AST_STAT(OK)) {
                ast_f(atmp);
                return astat;
            }
            *a = ast_i(AST_TYPE(TYPE), (node) { .tn = type_node_i(TYPE(FN), atmp) }, &ttmp);
            return ast_parse_stmt(as, fns, a, stp_flgs);
        // TODO TYPES
        OP_CASE(ASS);
        OP_CASE(CST);
        // TODO OPS
        // TODO WRAPS
        case TKN_TYPE(LB):
            if (*a) return AST_STAT(FH_A_NN);
            if ((astat = ast_tkn_peek(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->peek.type == TKN_TYPE(LP)) {
                tkn_st_u(&as->ts, &as->peek);
                return ast_parse_fn(as, fns, a);
            }
            // TODO hash
        default:
            break; // TODO remove
    }
    return AST_STAT(TKN_NF);
}

ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, lst_node *const lst, uint8_t stp_flgs, uint8_t end_flgs) {
    ast *a = NULL;
    ast_stat astat;
    while ((astat = ast_parse_stmt(as, fns, &a, stp_flgs | end_flgs)) == AST_STAT(OK)) {
        if (a) {
            lst_node_a(lst, a);
            a = NULL;
        }
        if (flg_mtch(as->next.type, end_flgs)) break;
    }
    return astat;
}
