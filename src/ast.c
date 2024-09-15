
#include "ast.h"

static const char *const ass[] = {
    "OK",
    "TKN_ERR",
    "TKN_INV",
    "TKN_NF",
    "VAL_A_NN",
    "RES_A_NN",
    "VAR_A_NN",
    "TYPE_A_NN",
    "VAR_I_ERR",
    "OP_CALL_A_NN",
    "FH_A_NN",
    "HSH_INV_KEY",
    "HSH_INV_ASS",
    "HSH_INV_VALUE",
    "VT_A_NN",
    "CALL_A_N",
    "IF_INV_BODY",
    "IF_A_NN",
    "IF_INV_FMT",
    "LOP_INV_BODY",
    "LOP_A_NN",
    "LOP_INV_FMT",
    "INV_TYPE_LST_INIT",
    "RET_A_NN",
    "END"
};

const char *ast_stat_str(ast_stat astat) {
    const char *s = "INVALID_AST_STAT";
    if (astat >= AST_STAT(OK) && AST_STAT(END)) s = ass[astat];
    return s;
}

extern inline void ast_st_i(ast_st *const as, al *const a, er *const e, char *const str);

extern inline ast_stat ast_er(ast_st *const as, const char *const fnn, ast_stat astat);

#define AST_ER(AS, ASTAT) ast_er(AS, __func__, AST_STAT(ASTAT))

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
        if ((tft[i].flg & flgs) && type == tft[i].type) return true;
    }
    return false;
}

ast_stat _ast_tkn_get(ast_st *const as, bool inc, uint8_t ign_flgs) {
    bool l = true;
    while (l) {
        l = false;
        as->tstat = _tkn_get(&as->ts, inc_tkn(as, inc), as->str, inc);
        if (as->tstat != TKN_STAT(OK)) break;
        if (flg_mtch(inc_tkn(as, inc)->type, ign_flgs)) {
            l = true;
            if (!inc) tkn_st_u(&as->ts, &as->peek);
        }
    }
    if (as->tstat == TKN_STAT(OK)) return AST_ER(as, OK);
    else if (as->tstat == TKN_STAT(END)) return AST_ER(as, END);
    return AST_ER(as, TKN_ERR);
}

extern inline ast_stat ast_tkn_next(ast_st *const as, uint8_t ign_flgs);

extern inline ast_stat ast_tkn_peek(ast_st *const as, uint8_t ign_flgs);

extern inline type_node *type_node_i(al *const am, type t, ast *const a);

type_node *type_node_c(al *const a, const type_node *const tn) {
    type_node *nn = ala(a, sizeof(type_node));
    nn->t = tn->t;
    if (tn->a) tn->a->rc++;
    nn->a = tn->a;
    return nn;
}

extern inline void type_node_p(const ast_st *const as, const type_node *const tn, size_t idnt);

extern inline void type_node_f(type_node *tn);

extern inline res_node *res_node_i(al *const a, res_type rt, type_node *tn);

extern inline void res_node_p(const ast_st *const as, const res_node *const rn, size_t idnt);

extern inline void res_node_f(res_node *rn);

extern inline val_node *val_node_i(al *const a, type t);

extern inline void val_node_p(const ast_st *const as, const val_node *const v, size_t idnt);

extern inline void val_node_f(val_node *v);

extern inline op_node *op_node_i(al *const a, op_type ot);

static const char *const op_type_str[] = {
    "TC",
    "ASS",
    "CST",
    "DEL",
    "ADD",
    "SUB",
    "MUL",
    "EQ",
    "NOT",
    "GT",
    "LT",
    "OR",
    "CNCT",
    "RW"
};

void op_node_p(const ast_st *const as, const op_node *const op, size_t idnt) {
    const char *type = "INVALID";
    if (op->ot >= OP_TYPE(TC) && op->ot <= OP_TYPE(RW)) type = op_type_str[op->ot];
    printf("%s", type);
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

extern inline lst_itm *lst_itm_i(al *const am, ast *const a);

extern inline void lst_itm_p(const ast_st *const as, const lst_itm *const li, void *fn, size_t idnt);

extern inline void lst_itm_f(lst_itm *li, void *fn);

extern inline lst_node *lst_node_i(al *const a, type t);

extern inline void lst_node_a(al *const am, lst_node *const lst, ast *const a);

extern inline void lst_node_p(const ast_st *const as, const lst_node *const lst, size_t idnt);

extern inline void lst_node_f(lst_node *lst);

extern inline kv_itm *kv_itm_i(al *const am, size_t id, const char *const k, ast *const a);

extern inline void kv_itm_p(const ast_st *const as, const kv_itm *const kv, void *fn, size_t idnt);

extern inline void kv_itm_f(kv_itm *kv, void *fn);

extern inline hsh_node *hsh_node_i(al *const a, type t);

extern inline void hsh_node_a(al *const am, hsh_node *const hsh, const char *const k, ast *const a);

extern inline void hsh_node_p(const ast_st *const as, const hsh_node *const hsh, size_t idnt);

extern inline void hsh_node_f(hsh_node *const hsh);

extern inline hsh_data *hsh_data_i(al *const a, size_t id, type_node *const tn);

extern inline void hsh_data_p(const ast_st *const as, const tbl *const tl, size_t idnt);

void hsh_data_f(void *data) {
    hsh_data *hd = (hsh_data*) data;
    FNNF(hd->tn, type_node_f);
    alf(hd);
}

extern inline if_itm *if_itm_i(al *const a, ast *const cond, lst_node *const body);

extern inline void if_itm_p(const ast_st *const as, const if_itm *const ii, void *fn, size_t idnt);

extern inline void if_itm_lop_p(const ast_st *const as, const if_itm *const ii, size_t idnt);

extern inline void if_itm_f(if_itm *im, void *fn);

extern inline void if_itm_lop_f(if_itm *im);

extern inline if_node *if_node_i(al *const a);

extern inline void if_node_a(al *const a, if_node *const in, ast* const cond, lst_node *const body);

extern inline void if_node_p(const ast_st *const as, const if_node *const in, size_t idnt);

extern inline void if_node_f(if_node *in);

extern inline fn_node *fn_node_i(al *const a, fn_node *const par);

static type_node *_fn_node_ret_type(const fn_node *const fn) {
    type_node *tmpt = NULL;
    if (fn->sig && fn->sig->t == TYPE(FN) && fn->sig->a && fn->sig->a->at == AST_TYPE(LST)) {
        tmpt = ast_gtn(fn->sig->a->n.lst->t->a);
    }
    return tmpt;
}

type_node *fn_node_ret_type(const fn_node *const fn) {
    type_node *tmpt = _fn_node_ret_type(fn);
    if (tmpt->t == TYPE(ER)) tmpt = ast_gtn(tmpt->a);
    return tmpt;
}

bool fn_node_tc(const fn_node *const fn) {
    type_node *tmpt = _fn_node_ret_type(fn);
    if (tmpt &&  tmpt->t == TYPE(ER)) return true;
    return false;
}

const char *const fnvims[] = {
    "S",
    "A",
    "L"
};

const char *fn_vim_str(fn_vim vim) {
    const char *s = "INVALID";
    if (vim >= FN_VIM(S) && vim <= FN_VIM(L)) s = fnvims[vim];
    return s;
}

void fn_node_p(const ast_st *const as, const fn_node *const fn, size_t idnt) {
    printf("%p,%s,%d,", fn, fn_vim_str(fn->vim), fn->idc);
    tbl_lstp(fn->tl, NULL, ' ');
    putchar('\n');
    PCX(' ', idnt);
    printf("%p", fn->par);
    type_node_p(as, fn->sig, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("A-");
    lst_node_p(as, fn->args, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("B-");
    lst_node_p(as, fn->body, idnt);
}

void fn_node_tbl_data_f(void *data) {
    var_node_f((var_node*) data);
}

extern inline void fn_node_f(fn_node *fn);

extern inline call_node *call_node_i(al *const a, ast *const tgt, lst_node *const args);

extern inline void call_node_p(const ast_st *const as, const call_node *const cn, size_t idnt);

extern inline void call_node_f(call_node *cn);

extern inline ret_node *ret_node_i(al *const a, fn_node *const fn);

extern inline void ret_node_p(const ast_st *const as, const ret_node *const r, size_t idnt);

extern inline void ret_node_f(ret_node *r);

static const char *const vts[] = {
    "U",
    "G",
    "L",
    "A"
};

const char *var_type_str(var_type vt) {
    const char *s = "INVALID";
    if (vt >= VAR_TYPE(U) && vt <= VAR_TYPE(A)) s = vts[vt];
    return s;
}

#ifndef MAX_VAR_LEN
    #define MAX_VAR_LEN 21 // null term
#endif

// TODO better tbl errors
var_node *var_node_i(al *const a, fn_node *const fns, const tkn *const t, const char *const str) {
    char vstr[MAX_VAR_LEN];
    memset(vstr, '\0', MAX_VAR_LEN);
    memcpy(vstr, str + t->pos, t->len);
    tbl_itm *ti;
    fn_node *scope = fns->vim == FN_VIM(S) ? fns : NULL;
    while (scope) {
        if (tbl_op(a, &scope->tl, vstr, NULL, &ti, NULL, TBL_OP_FLG(FD)) == TBL_STAT(OK)) return (var_node*) ti->data;
        scope = scope->par;
    }
    if (!scope) {
        var_node *vn = ala(a, sizeof(var_node) + t->len + 1);
        vn->id = fns->idc++;
        if (fns->vim == FN_VIM(A)) vn->vt = VAR_TYPE(A);
        else if (fns->vim == FN_VIM(L)) vn->vt = VAR_TYPE(L);
        else vn->vt = fns->par ? VAR_TYPE(L) : VAR_TYPE(G);
        vn->fns = fns;
        memcpy(vn->str, str + t->pos, t->len);
        if (tbl_op(a, &fns->tl, vstr, vn, &ti, NULL, TBL_OP_FLG(AD)) != TBL_STAT(OK)) {
            var_node_f(vn);
            return NULL;
        }
        return vn;
    }
    return NULL;
}

extern inline void var_node_p(const ast_st *const as, const var_node *const var, size_t idnt);

extern inline void var_node_f(var_node *vn);

extern inline ast *ast_i(al *const am, ast_type as, node const n, const tkn *const t);

extern inline type_node *ast_gtn(const ast *const a);

static const char *const ast_type_str[] = {
    "TYPE",
    "RES",
    "VAL",
    "OP",
    "LST",
    "HSH",
    "TBL",
    "IF",
    "LOP",
    "FN",
    "CALL",
    "RET",
    "VAR"
};

#define AST_P_CASE(T, N, F) case AST_TYPE(T): \
    return F(as, a->n.N, idnt);

void ast_p(const ast_st *const as, const ast *const a, size_t idnt) {
    if (!a) {
        printf("NULL");
        return;
    }
    const char *type = "INVALID";
    if (a->at >= AST_TYPE(TYPE) && a->at <= AST_TYPE(VAR)) type = ast_type_str[a->at];
    printf("%s<%d>", type, a->rc);
    putchar('|');
    tkn_p(&a->t, as->str);
    putchar('|');
    idnt += IDNT_ADD;
    switch (a->at) {
        AST_P_CASE(TYPE, tn, type_node_p);
        AST_P_CASE(RES, rn, res_node_p);
        AST_P_CASE(VAL, val, val_node_p);
        AST_P_CASE(OP, op, op_node_p);
        AST_P_CASE(LST, lst, lst_node_p);
        AST_P_CASE(HSH, hsh, hsh_node_p);
        AST_P_CASE(TBL, tl, hsh_data_p);
        AST_P_CASE(IF, in, if_node_p);
        AST_P_CASE(LOP, lop, if_itm_lop_p);
        AST_P_CASE(FN, fn, fn_node_p);
        AST_P_CASE(CALL, cn, call_node_p);
        AST_P_CASE(RET, ret, ret_node_p);
        AST_P_CASE(VAR, var, var_node_p);
    }
}

#define AST_F_CASE(T, N, F) case AST_TYPE(T): \
    FNNF(a->n.N, F); \
    break

void ast_f(ast *a) {
    if (--a->rc > 0) return;
    switch (a->at) {
        AST_F_CASE(TYPE, tn, type_node_f);
        AST_F_CASE(RES, rn, res_node_f);
        AST_F_CASE(VAL, val, val_node_f);
        AST_F_CASE(OP, op, op_node_f);
        AST_F_CASE(LST, lst, lst_node_f);
        AST_F_CASE(HSH, hsh, hsh_node_f);
        case AST_TYPE(TBL):
            if (a->n.tl) tbl_f(a->n.tl, hsh_data_f);
            break;
        AST_F_CASE(IF, in, if_node_f);
        AST_F_CASE(LOP, lop, if_itm_lop_f);
        AST_F_CASE(FN, fn, fn_node_f);
        AST_F_CASE(CALL, cn, call_node_f);
        AST_F_CASE(RET, ret, ret_node_f);
        case AST_TYPE(VAR):
            break; // var freed in fn sym tbl
    }
    alf(a);
}

#define TYPE_NA_CASE(T) case TKN_TYPE(T): \
    if (*a) return AST_ER(as, TYPE_A_NN); \
    *a = ast_i(as->a, AST_TYPE(TYPE), (node) { .tn = type_node_i(as->a, TYPE(T), NULL) }, &as->next); \
    return ast_parse_stmt(as, fns, a, stp_flgs)

#define VAL_CASE(T) case TKN_TYPE(T): \
    if (*a) return AST_ER(as, VAL_A_NN); \
    *a = ast_i(as->a, AST_TYPE(VAL), (node) { .val = val_node_i(as->a, TYPE(T)) }, &as->next); \
    return ast_parse_stmt(as, fns, a, stp_flgs)

#define RES_CASE(T, TT) case TKN_TYPE(T): \
    if (*a) return AST_ER(as, RES_A_NN); \
    *a = ast_i(as->a, AST_TYPE(RES), (node) { .rn = res_node_i(as->a, RES_TYPE(T), TT) }, &as->next); \
    return ast_parse_stmt(as, fns, a, stp_flgs)

static ast_stat ast_parse_call(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs, const tkn *const tkn_s) {
    ast_stat astat;
    lst_node *args = lst_node_i(as->a, TYPE(ARGS));
    if ((astat = ast_parse_stmts(as, fns, args, TFLS, TKN_FLG(RP))) != AST_STAT(OK)) {
        lst_node_f(args);
        return astat;
    }
    *a = ast_i(as->a, AST_TYPE(CALL), (node) { .cn = call_node_i(as->a, *a, args) }, tkn_s);
    return ast_parse_stmt(as, fns, a, stp_flgs);
}

static ast_stat ast_parse_op(op_type ot, ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs) {
    ast_stat astat;
    if ((astat = ast_tkn_peek(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
    op_node *op = op_node_i(as->a, ot);
    op->l = *a;
    *a = ast_i(as->a, AST_TYPE(OP), (node) { .op = op }, &as->next);
    if (as->peek.type == TKN_TYPE(LP)) {
        tkn_st_u(&as->ts, &as->peek);
        if (op->l) {
            ast_f(*a);
            return AST_ER(as, OP_CALL_A_NN);
        }
        return ast_parse_call(as, fns, a, stp_flgs, &as->peek);
    }
    return ast_parse_stmt(as, fns, &op->r, stp_flgs);
}

#define OP_CASE(T) case TKN_TYPE(T): \
    return ast_parse_op(OP_TYPE(T), as, fns, a, stp_flgs)

static ast_stat ast_parse_if_itm(ast_st *const as, fn_node *const fns, if_node *in, bool pc) {
    ast_stat astat;
    if (!pc) {
        if ((astat = ast_tkn_peek(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
        if (as->peek.type != TKN_TYPE(LB)) return AST_ER(as, OK);
        tkn_st_u(&as->ts, &as->peek);
    }
    ast *cond = NULL;
    lst_node *body = lst_node_i(as->a, TYPE(STMT));
    if (pc) {
        if ((astat = ast_parse_stmt(as, fns, &cond, TKN_FLG(RP))) != AST_STAT(OK)) {
            FNNF(cond, ast_f);
            return astat;
        }
        if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
        if (as->next.type != TKN_TYPE(LB)) return AST_ER(as, IF_INV_BODY);
    }
    if ((astat = ast_parse_stmts(as, fns, body, TFLS, TKN_FLG(RB))) != AST_STAT(OK)) {
        lst_node_f(body);
        return astat;
    }
    if_node_a(as->a, in, cond, body);
    return AST_ER(as, OK);
}

static ast_stat ast_parse_if(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs, const tkn *const tkn_s) {
    bool pc = true;
    if_node *in = if_node_i(as->a);
    ast_stat astat;
    while (pc) {
        if ((astat = ast_parse_if_itm(as, fns, in, pc)) != AST_STAT(OK)) {
            if_node_f(in);
            return astat;
        }
        if ((astat = ast_tkn_peek(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
        if (as->peek.type == TKN_TYPE(LP)) tkn_st_u(&as->ts, &as->peek);
        else pc = false;
    }
    if ((astat = ast_parse_if_itm(as, fns, in, pc)) != AST_STAT(OK)) {
        if_node_f(in);
        return astat;
    }
    *a = ast_i(as->a, AST_TYPE(IF), (node) { .in = in }, tkn_s);
    return ast_parse_stmt(as, fns, a, stp_flgs);
}

static ast_stat ast_parse_lop(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs, const tkn *const tkn_s) {
    ast_stat astat;
    ast *cond = NULL;
    lst_node *body = lst_node_i(as->a, TYPE(STMT));
    if ((astat = ast_parse_stmt(as, fns, &cond, TKN_FLG(RP))) != AST_STAT(OK)) {
        FNNF(cond, ast_f);
        return astat;
    }
    if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
    if (as->next.type != TKN_TYPE(LB)) return AST_ER(as, LOP_INV_BODY);
    if ((astat = ast_parse_stmts(as, fns, body, TFLS, TKN_FLG(RB))) != AST_STAT(OK)) {
        lst_node_f(body);
        return astat;
    }
    if_itm *lop = if_itm_i(as->a, cond, body);
    *a = ast_i(as->a, AST_TYPE(LOP), (node) { .lop = lop }, tkn_s);
    return ast_parse_stmt(as, fns, a, stp_flgs);
}

static ast_stat ast_parse_fn(ast_st *const as, fn_node *const par, ast **a, uint8_t stp_flgs, const tkn *const tkn_s) {
    fn_node *fn = fn_node_i(as->a, par);
    fn->vim = FN_VIM(A);
    ast_stat astat;
    if ((astat = ast_parse_stmts(as, fn, fn->args, TKN_FLG(SEMI), TKN_FLG(RP))) != AST_STAT(OK)) {
        fn_node_f(fn);
        return astat;
    }
    fn->vim = FN_VIM(S);
    if ((astat = ast_parse_stmts(as, fn, fn->body, TFLS, TKN_FLG(RB))) != AST_STAT(OK)) {
        fn_node_f(fn);
        return astat;
    }
    *a = ast_i(as->a, AST_TYPE(FN), (node) { .fn = fn }, tkn_s);
    return ast_parse_stmt(as, par, a, stp_flgs);
}

static ast_stat ast_parse_hsh(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs, const tkn *const tkn_s) {
    ast_stat astat;
    hsh_node *hsh = hsh_node_i(as->a, TYPE(ST));
    for (;;) {
        ast *atmp = NULL;
        char kstr[MAX_VAR_LEN + 1]; // for `
        memset(kstr, '\0', MAX_VAR_LEN + 1);
        if ((astat = ast_tkn_next(as, TKN_FLG(NL) | TFWC)) != AST_STAT(OK)) return astat;
        if (as->next.type == TKN_TYPE(RB)) break;
        if (as->next.type != TKN_TYPE(SYM)) {
            hsh_node_f(hsh);
            return AST_ER(as, HSH_INV_KEY);
        }
        memcpy(kstr, as->str + as->next.pos, as->next.len);
        if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
        if (as->next.type != TKN_TYPE(ASS)) {
            hsh_node_f(hsh);
            return AST_ER(as, HSH_INV_ASS);
        }
        if ((astat = ast_parse_stmt(as, fns, &atmp, TFLS | TKN_FLG(RB))) != AST_STAT(OK)) {
            hsh_node_f(hsh);
            return AST_ER(as, HSH_INV_VALUE);
        }
        hsh_node_a(as->a, hsh, kstr, atmp);
        if (as->next.type == TKN_TYPE(RB)) break;
    }
    *a = ast_i(as->a, AST_TYPE(HSH), (node) { .hsh = hsh }, tkn_s);
    return ast_parse_stmt(as, fns, a, stp_flgs);
}

ast_stat ast_parse_stmt(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs) {
    ast_stat astat;
    if ((astat = ast_tkn_next(as, TFWC)) != AST_STAT(OK)) return astat;
    if (flg_mtch(as->next.type, stp_flgs)) return AST_ER(as, OK);
    ast *atmp = NULL;
    var_node *var;
    tkn ttmp;
    switch (as->next.type) {
        case TKN_TYPE(NB):
        case TKN_TYPE(NL):
        case TKN_TYPE(SEMI):
        case TKN_TYPE(WS):
        case TKN_TYPE(CMT):
            return AST_ER(as, TKN_INV);
        case TKN_TYPE(VAR):
            if (*a) return AST_ER(as, VAR_A_NN);
            if (!(var = var_node_i(as->a, fns, &as->next, as->str))) {
                var_node_f(var);
                return AST_ER(as, VAR_I_ERR);
            }
            *a = ast_i(as->a, AST_TYPE(VAR), (node) { .var = var }, &as->next);
            return ast_parse_stmt(as, fns, a, stp_flgs);
        VAL_CASE(INT);
        VAL_CASE(FLT);
        VAL_CASE(STR);
        RES_CASE(TRUE, type_node_i(as->a, TYPE(BL), NULL));
        RES_CASE(FALSE, type_node_i(as->a, TYPE(BL), NULL));
        RES_CASE(SELF, NULL);
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
        // TODO TYPES
        case TKN_TYPE(FN):
            if (*a) return AST_ER(as, TYPE_A_NN);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->next.type != TKN_TYPE(LP)) return AST_ER(as, INV_TYPE_LST_INIT);
            atmp = ast_i(as->a, AST_TYPE(LST), (node) { .lst = lst_node_i(as->a, TYPE(STMT)) }, &as->next);
            if ((astat = ast_parse_stmts(as, fns, atmp->n.lst, TKN_FLG(SEMI), TKN_FLG(RP))) != AST_STAT(OK)) {
                ast_f(atmp);
                return astat;
            }
            *a = ast_i(as->a, AST_TYPE(TYPE), (node) { .tn = type_node_i(as->a, TYPE(FN), atmp) }, &ttmp);
            return ast_parse_stmt(as, fns, a, stp_flgs);
        case TKN_TYPE(ER):
            if (*a) return AST_ER(as, TYPE_A_NN);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->next.type != TKN_TYPE(LP)) return AST_ER(as, INV_TYPE_LST_INIT);
            if ((astat = ast_parse_stmt(as, fns, &atmp, TKN_FLG(RP))) != AST_STAT(OK)) {
                ast_f(atmp);
                return astat;
            }
            *a = ast_i(as->a, AST_TYPE(TYPE), (node) { .tn = type_node_i(as->a, TYPE(ER), atmp) }, &ttmp);
            return ast_parse_stmt(as, fns, a, stp_flgs);
        TYPE_NA_CASE(FD);
        // TODO TYPES
        OP_CASE(TC);
        OP_CASE(ASS);
        OP_CASE(CST);
        OP_CASE(DEL);
        OP_CASE(ADD);
        OP_CASE(SUB);
        OP_CASE(MUL);
        // TODO OPS
        OP_CASE(EQ);
        OP_CASE(NOT);
        OP_CASE(GT);
        OP_CASE(LT);
        // TODO OPS
        OP_CASE(OR);
        OP_CASE(CNCT);
        OP_CASE(RW);
        // TODO OPS
        // wraps
        case TKN_TYPE(LB):
            if (*a) return AST_ER(as, FH_A_NN);
            if ((astat = ast_tkn_peek(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->peek.type == TKN_TYPE(LP)) {
                memcpy(&ttmp, &as->next, sizeof(tkn));
                tkn_st_u(&as->ts, &as->peek);
                return ast_parse_fn(as, fns, a, stp_flgs, &ttmp);
            }
            memcpy(&ttmp, &as->next, sizeof(tkn));
            return ast_parse_hsh(as, fns, a, stp_flgs, &ttmp);
            break;
        case TKN_TYPE(RB):
            return AST_ER(as, TKN_INV);
        case TKN_TYPE(LS):
            if (*a) return AST_ER(as, VT_A_NN);
            *a = ast_i(as->a, AST_TYPE(LST), (node) { .lst = lst_node_i(as->a, TYPE(TE)) }, &as->next);
            if ((astat = ast_parse_stmts(as, fns, (*a)->n.lst, TKN_FLG(SEMI), TKN_FLG(RS))) != AST_STAT(OK)) {
                ast_f(*a);
                return astat;
            }
            return ast_parse_stmt(as, fns, a, stp_flgs);
        case TKN_TYPE(RS):
            return AST_ER(as, TKN_INV);
        case TKN_TYPE(LP):
            if (!*a) return AST_ER(as, CALL_A_N);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            return ast_parse_call(as, fns, a, stp_flgs, &ttmp);
        case TKN_TYPE(RP):
            return AST_ER(as, TKN_INV);
        case TKN_TYPE(IF):
            if (*a) return AST_ER(as, IF_A_NN);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->next.type != TKN_TYPE(LP)) return AST_ER(as, IF_INV_FMT);
            return ast_parse_if(as, fns, a, stp_flgs, &ttmp);
        case TKN_TYPE(LOP):
            if (*a) return AST_ER(as, LOP_A_NN);
            memcpy(&ttmp, &as->next, sizeof(tkn));
            if ((astat = ast_tkn_next(as, TKN_FLG(WS))) != AST_STAT(OK)) return astat;
            if (as->next.type != TKN_TYPE(LP)) return AST_ER(as, LOP_INV_FMT);
            return ast_parse_lop(as, fns, a, stp_flgs, &ttmp);
        case TKN_TYPE(RET):
           if (*a) return AST_ER(as, RET_A_NN);
           *a = ast_i(as->a, AST_TYPE(RET), (node) { .ret = ret_node_i(as->a, fns) }, &as->next);
           return ast_parse_stmt(as, fns, &(*a)->n.ret->a, stp_flgs);
        // TODO controls
        default:
            break; // TODO remove
    }
    return AST_ER(as, TKN_NF);
}


ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, lst_node *const lst, uint8_t stp_flgs, uint8_t end_flgs) {
    ast *a = NULL;
    ast_stat astat;
    while ((astat = ast_parse_stmt(as, fns, &a, stp_flgs | end_flgs)) == AST_STAT(OK)) {
        if (a) {
            lst_node_a(as->a, lst, a);
            a = NULL;
        }
        if (flg_mtch(as->next.type, end_flgs)) break;
    }
    return astat;
}
