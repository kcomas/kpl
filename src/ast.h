
#pragma once

#include "kpl.h"
#include "tbl.h"
#include "tkn.h"

#define AST_STAT(N) ast_stat_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(TKN_ERR),
    AST_STAT(END)
} ast_stat;

typedef struct {
    tkn_stat tstat;
    tkn_st ts;
    tkn next, peek;
    char *str;
} ast_st;

inline void ast_st_i(ast_st *const at, char *const str) {
    tkn_st_i(&at->ts);
    at->str = str;
}

#define TKN_FLG(N) TKN_IGN_##N

typedef enum {
    TKN_FLG(NB) = (1 << 0),
    TKN_FLG(NL) = (1 << 1),
    TKN_FLG(SEMI) = (1 << 2),
    TKN_FLG(WS) = (1 << 3),
    TKN_FLG(CMT) = (1 << 4),
    TKN_FLG(RB) = (1 << 5), // }
    TKN_FLG(RS) = (1 << 6), // ]
    TKN_FLG(RP) = (1 << 7) // )
} tkn_flg;

ast_stat _ast_tkn_get(ast_st *const at, bool inc, uint8_t ign_flgs);

inline ast_stat ast_tkn_next(ast_st *const at, uint8_t ign_flgs) {
    return _ast_tkn_get(at, true, ign_flgs);
}

inline ast_stat ast_tkn_peek(ast_st *const at, uint8_t ign_flgs) {
    return _ast_tkn_get(at, false, ign_flgs);
}

typedef struct _ast ast;

void ast_f(ast *a);

#define TYPE(N) TYPE_##N

typedef enum {
    // ast types
    TYPE(INT),
    TYPE(FLT),
    TYPE(STR),
    // data types
    TYPE(VD),
    TYPE(U3),
    TYPE(U4),
    TYPE(U5),
    TYPE(U6),
    TYPE(I3),
    TYPE(I4),
    TYPE(I5),
    TYPE(I6),
    TYPE(F5),
    TYPE(F6),
    TYPE(DT),
    TYPE(CR),
    TYPE(SL),
    TYPE(SG),
    TYPE(VR),
    TYPE(TE),
    TYPE(HH),
    TYPE(ST),
    TYPE(FN),
    TYPE(ER),
    TYPE(FD)
} type;

typedef struct {
    type t;
    ast *a;
} type_node;

inline type_node *type_node_i(type t, ast *const a) {
    type_node *tn = calloc(1, sizeof(type_node));
    tn->t = t;
    tn->a = a;
    return tn;
}

inline void type_node_f(type_node *tn) {
    FNNF(tn->a, ast_f);
    free(tn);
}

typedef struct {
    type t;
} val_node; // val is tkn str

inline val_node *val_node_i(type t) {
    val_node *v = calloc(1, sizeof(val_node));
    v->t = t;
    return v;
}

inline void val_node_f(val_node *v) {
    free(v);
}

#define OP_TYPE(N) OP_TYPE_##N

typedef enum {
    OP_TYPE(ASS), // :
    OP_TYPE(CST) // $
} op_type;

typedef struct {
    op_type ot;
    type_node *ret;
    ast *l, *r;
} op_node;

inline op_node *op_node_i(op_type ot) {
    op_node *on = calloc(1, sizeof(op_node));
    on->ot = ot;
    return on;
}

inline void op_node_f(op_node *on) {
    FNNF(on->ret, type_node_f);
    FNNF(on->l, ast_f);
    FNNF(on->r, ast_f);
    free(on);
}

typedef struct _lst_itm {
    ast *a;
    struct _lst_itm *next;
} lst_itm;

inline lst_itm *lst_itm_i(ast *const a) {
    lst_itm *itm = calloc(1, sizeof(lst_itm));
    itm->a = a;
    return itm;
}

inline void lst_itm_f(lst_itm *itm, void *fn) {
    (void) fn;
    FNNF(itm->a, ast_f);
    free(itm);
}

typedef struct _lst_node {
    size_t len;
    type typ;
    lst_itm *h, *t;
} lst_node;

inline lst_node *lst_node_i(type typ) {
    lst_node *ln = calloc(1, sizeof(lst_node));
    ln->typ = typ;
    return ln;
}

inline void lst_node_a(lst_node *const lst, ast *const a) {
    LST_A(lst, lst_itm_i(a));
}

inline void lst_node_f(lst_node *lst) {
    LST_F(lst, lst_itm, lst_itm_f, NULL);
}

typedef struct _if_itm {
    struct _if_itm *next;
    ast *cond; // null for else
    lst_node *body;
} if_itm;

inline if_itm *if_itm_i(ast *const cond, lst_node *const body) {
    if_itm *im = calloc(1, sizeof(if_itm));
    im->cond = cond;
    im->body = body;
    return im;
}

inline void if_itm_f(if_itm *im, void *fn) {
    (void) fn;
    ast_f(im->cond);
    lst_node_f(im->body);
    free(im);
}

typedef struct {
    size_t len;
    if_itm *h, *t;
} if_node;

inline if_node *if_node_i(void) {
    return calloc(1, sizeof(if_node));
}

inline void if_node_a(if_node *const in, ast* const cond, lst_node *const body) {
    LST_A(in, if_itm_i(cond, body));
}

inline void if_node_f(if_node *in) {
    LST_F(in, if_itm, if_itm_f, NULL);
}

typedef struct _fn_node {
    uint8_t idc; // var id counter
    tbl *tl; // sym tbl
    type_node *ret;
    struct _fn_node *par; // parent node
    lst_node *args, *body; // tail arg is ret type only mods have NULL args
} fn_node;

inline fn_node *fn_node_i(fn_node *const par) {
    fn_node *fn = calloc(1, sizeof(fn_node));
    fn->tl = tbl_i(TBL_I_SIZE);
    fn->par = par;
    fn->args = lst_node_i(TYPE(TE));
    fn->body = lst_node_i(TYPE(TE));
    return fn;
}

void fn_node_tbl_data_f(void *data);

inline void fn_node_f(fn_node *fn) {
    tbl_f(fn->tl, &fn_node_tbl_data_f);
    FNNF(fn->ret, type_node_f);
    lst_node_f(fn->args);
    lst_node_f(fn->body);
    free(fn);
}

typedef struct {
    ast *tgt;
    type_node *ret;
    lst_node *args;
} call_node;

inline call_node *call_node_i(ast *const tgt, lst_node *const args) {
    call_node *cn = calloc(1, sizeof(call_node));
    cn->tgt = tgt;
    cn->args = args;
    return cn;
}

inline void call_node_f(call_node *cn) {
    ast_f(cn->tgt);
    FNNF(cn->ret, type_node_f);
    lst_node_f(cn->args);
}

typedef struct {
    ast *a;
} ret_node; // takes next stmt

inline ret_node *ret_node_i(ast *const a) {
    ret_node *r = calloc(1, sizeof(ret_node));
    r->a = a;
    return r;
}

inline void ret_node_f(ret_node *r) {
    ast_f(r->a);
    free(r);
}

typedef struct {
    uint8_t id;
    type_node *tn; // null for unknown
    fn_node *fns; // scope
    char str[]; // null term
} var_node; // tbl itm data TODO struct padding

inline var_node *var_node_i(fn_node *const fns, const tkn *const t, const char *const str) {
    // TODO check if this var node exists
    var_node *vn = calloc(1, sizeof(var_node) + t->len + 1);
    vn->id = fns->idc++;
    vn->fns = fns;
    memcpy(vn->str, str + t->pos, t->len);
    return vn;
}

inline void var_node_f(var_node *vn) {
    FNNF(vn->tn, type_node_f);
    free(vn);
}

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(TYPE),
    AST_TYPE(VAL),
    AST_TYPE(OP),
    AST_TYPE(LST),
    AST_TYPE(IF),
    AST_TYPE(FN),
    AST_TYPE(CALL),
    AST_TYPE(RET),
    AST_TYPE(VAR)
} ast_type;

typedef union {
    type_node *tn;
    val_node *val;
    op_node *op;
    lst_node *lst;
    if_node *in;
    fn_node *fn;
    call_node *cn;
    ret_node *ret;
    var_node *var;
} node;

typedef struct _ast {
    ast_type at;
    node n;
    tkn t;
} ast;

inline ast *ast_i(ast_type at, node const n, tkn *t) {
    ast *a = calloc(1, sizeof(ast));
    a->at = at;
    a->n = n;
    memcpy(&a->t, t, sizeof(tkn));
    return a;
}

ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, lst_node *cl, uint8_t stp_flgs);
