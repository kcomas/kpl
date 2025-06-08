
#pragma once

#include "kpl.h"
#include "tbl.h"
#include "tkn.h"
#include "er.h"

#define AST_STAT(N) AST_STAT_##N

typedef enum {
    AST_STAT(OK),
    AST_STAT(TKN_ERR),
    AST_STAT(TKN_INV), // tkn should not have been found
    AST_STAT(TKN_NF), // no case for tkn
    AST_STAT(VAL_A_NN), // prev node for val not null
    AST_STAT(RES_A_NN), // prev node for res not null
    AST_STAT(VAR_A_NN), // prev node for var not null
    AST_STAT(TYPE_A_NN), // prev node for type not null
    AST_STAT(VAR_I_ERR), // failed to add var node
    AST_STAT(OP_CALL_A_NN), // prev op node for call not null
    AST_STAT(FH_A_NN), // prev node for fn/hash not null
    AST_STAT(VT_A_NN), // prev node for vec/tuple not null
    AST_STAT(CALL_A_N), // prev node for call null
    AST_STAT(IF_INV_BODY), // invalid if body
    AST_STAT(IF_A_NN), // prev node for if not null
    AST_STAT(IF_INV_FMT), // missing ( after if start
    AST_STAT(LOP_INV_BODY), // missing ( after lop start
    AST_STAT(LOP_A_NN), // prev node for lop not null
    AST_STAT(LOP_INV_FMT), // missing ( after lop start
    AST_STAT(INV_TYPE_LST_INIT), // type list must start with ( [ {
    AST_STAT(RET_A_NN), // prev node for if not null
    AST_STAT(END)
} ast_stat;

typedef struct {
    tkn_stat tstat;
    tkn_st ts;
    tkn next, peek;
    al *a;
    er *e;
    const char *str;
} ast_st;

inline void ast_st_i(ast_st *const as, al *const a, er *const e, char *const str) {
    tkn_st_i(&as->ts);
    as->a = a;
    as->e = e;
    as->str = str;
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
    TKN_FLG(RP) = (1 << 7), // )
} tkn_flg;

#define TFLS (TKN_FLG(NL) | TKN_FLG(SEMI))

#define TFWC (TKN_FLG(WS) | TKN_FLG(CMT))

ast_stat _ast_tkn_get(ast_st *const as, bool inc, uint8_t ign_flgs);

inline ast_stat ast_tkn_next(ast_st *const as, uint8_t ign_flgs) {
    return _ast_tkn_get(as, true, ign_flgs);
}

inline ast_stat ast_tkn_peek(ast_st *const as, uint8_t ign_flgs) {
    return _ast_tkn_get(as, false, ign_flgs);
}

typedef struct _ast ast;

#ifndef IDNT_ADD
    #define IDNT_ADD 4
#endif

void ast_p(const ast_st *const as, const ast *const a, size_t idnt);

void ast_f(ast *a);

typedef struct {
    type t;
    ast *a;
} type_node;

inline type_node *type_node_i(al *const am, type t, ast *const a) {
    type_node *tn = ala(am, sizeof(type_node));
    tn->t = t;
    tn->a = a;
    return tn;
}

type_node *type_node_c(al *const a, const type_node *const tn);

inline void type_node_p(const ast_st *const as, const type_node *const tn, size_t idnt) {
    if (tn) {
        putchar('\n');
        PCX(' ', idnt);
        printf("%s", type_get_str(tn->t));
        if (tn->a) {
            putchar('\n');
            PCX(' ', idnt);
            ast_p(as, tn->a, idnt);
        }
    } else {
        putchar('\n');
        PCX(' ', idnt);
        putchar('?');
    }
}

inline void type_node_f(type_node *tn) {
    FNNF(tn->a, ast_f);
    alf(tn);
}

#define RES_TYPE(N) RES_TYPE_##N

typedef enum {
    RES_TYPE(TRUE),
    RES_TYPE(FALSE),
    RES_TYPE(SELF)
} res_type;

typedef struct {
    res_type rt;
    type_node *tn;
} res_node; // reserved keywords

inline res_node *res_node_i(al *const a, res_type rt, type_node *tn) {
    res_node *rn = ala(a, sizeof(res_node));
    rn->rt = rt;
    rn->tn = tn;
    return rn;
}

inline void res_node_p(const ast_st *const as, const res_node *const rn, size_t idnt) {
    switch (rn->rt) {
        case RES_TYPE(TRUE):
            printf("T 1");
            break;
        case RES_TYPE(FALSE):
            printf("F 0");
            break;
        case RES_TYPE(SELF):
            printf("S SELF");
            break;
    }
    type_node_p(as, rn->tn, idnt);
}

inline void res_node_f(res_node *rn) {
    FNNF(rn->tn, type_node_f);
    alf(rn);
}

typedef struct {
    type_node *tn;
} val_node; // val is tkn str

inline val_node *val_node_i(al *const a, type t) {
    val_node *v = ala(a, sizeof(val_node));
    v->tn = type_node_i(a, t, NULL);
    return v;
}

inline void val_node_p(const ast_st *const as, const val_node *const v, size_t idnt) {
    type_node_p(as, v->tn, idnt);
}

inline void val_node_f(val_node *v) {
    type_node_f(v->tn);
    alf(v);
}

#define OP_TYPE(N) OP_TYPE_##N

typedef enum {
    OP_TYPE(ASS), // :
    OP_TYPE(CST), // $
    OP_TYPE(DEL), // \d
    OP_TYPE(ADD),
    OP_TYPE(SUB),
    OP_TYPE(MUL),
    // TODO OP
    OP_TYPE(EQ),
    OP_TYPE(NOT),
    OP_TYPE(GT),
    OP_TYPE(LT),
    // TODO OP
    OP_TYPE(OR),
    OP_TYPE(CNCT),
    OP_TYPE(RW)
} op_type;

typedef struct {
    op_type ot;
    type_node *ret;
    ast *l, *r;
} op_node;

inline op_node *op_node_i(al *const a, op_type ot) {
    op_node *on = ala(a, sizeof(op_node));
    on->ot = ot;
    return on;
}

void op_node_p(const ast_st *const as, const op_node *const op, size_t idnt);

inline void op_node_f(op_node *on) {
    FNNF(on->ret, type_node_f);
    FNNF(on->l, ast_f);
    FNNF(on->r, ast_f);
    alf(on);
}

typedef struct _lst_itm {
    ast *a;
    struct _lst_itm *prev, *next;
} lst_itm;

inline lst_itm *lst_itm_i(al *const am, ast *const a) {
    lst_itm *itm = ala(am, sizeof(lst_itm));
    itm->a = a;
    return itm;
}

inline void lst_itm_p(const ast_st *const as, const lst_itm *const li, void *fn, size_t idnt) {
    (void) fn;
    ast_p(as, li->a, idnt);
}

inline void lst_itm_f(lst_itm *li, void *fn) {
    (void) fn;
    FNNF(li->a, ast_f);
    alf(li);
}
typedef struct _lst_node {
    size_t len;
    type_node *tn;
    lst_itm *h, *t;
} lst_node;

inline lst_node *lst_node_i(al *const a, type t) {
    lst_node *ln = ala(a, sizeof(lst_node));
    ln->tn = type_node_i(a, t, NULL);
    return ln;
}

inline void lst_node_a(al *const am, lst_node *const lst, ast *const a) {
    lst_itm *li = lst_itm_i(am, a);
    LST_A(lst, li);
}

inline void lst_node_p(const ast_st *const as, const lst_node *const lst, size_t idnt) {
    printf("%lu\\/", lst->len);
    type_node_p(as, lst->tn, idnt);
    LST_P(lst, lst_itm, lst_itm_p, as, NULL, idnt, '\n');
}

inline void lst_node_f(lst_node *lst) {
    type_node_f(lst->tn);
    LST_F(lst, lst_itm, lst_itm_f, NULL);
    alf(lst);
}

typedef struct _if_itm {
    struct _if_itm *prev, *next;
    ast *cond; // null for else
    lst_node *body;
} if_itm;

inline if_itm *if_itm_i(al *const a, ast *const cond, lst_node *const body) {
    if_itm *im = ala(a, sizeof(if_itm));
    im->cond = cond;
    im->body = body;
    return im;
}

inline void if_itm_p(const ast_st *const as, const if_itm *const ii, void *fn, size_t idnt) {
    (void) fn;
    printf("C-");
    ast_p(as, ii->cond, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("B-");
    lst_node_p(as, ii->body, idnt);
}

inline void if_itm_lop_p(const ast_st *const as, const if_itm *const ii, size_t idnt) {
    putchar('\n');
    PCX(' ', idnt);
    if_itm_p(as, ii, 0, idnt);
}

inline void if_itm_f(if_itm *im, void *fn) {
    (void) fn;
    ast_f(im->cond);
    lst_node_f(im->body);
    alf(im);
}

inline void if_itm_lop_f(if_itm *im) {
    if_itm_f(im, 0);
}

typedef struct {
    size_t len;
    if_itm *h, *t;
} if_node;

inline if_node *if_node_i(al *const a) {
    return ala(a, sizeof(if_node));
}

inline void if_node_a(al *const a, if_node *const in, ast* const cond, lst_node *const body) {
    if_itm *ii = if_itm_i(a, cond, body);
    LST_A(in, ii);
}

inline void if_node_p(const ast_st *const as, const if_node *const in, size_t idnt) {
    printf("%lu,IF", in->len);
    LST_P(in, if_itm, if_itm_p, as, NULL, idnt, '\n');
}

inline void if_node_f(if_node *in) {
    LST_F(in, if_itm, if_itm_f, NULL);
    alf(in);
}

#define FN_VIM(N) FN_VIM_##N

typedef enum {
   FN_VIM(S), // standard local or global
   FN_VIM(A), // arg only
   FN_VIM(L) // local only
} fn_vim; // fn var insert mode

typedef struct _fn_node {
    uint8_t idc; // var id counter
    fn_vim vim;
    tbl *tl; // sym tbl
    struct _fn_node *par; // parent node
    type_node *sig; // tail arg is ret type only mods have NULL args
    lst_node *args, *body;
} fn_node;

inline fn_node *fn_node_i(al *const a, fn_node *const par) {
    fn_node *fn = ala(a, sizeof(fn_node));
    fn->tl = tbl_i(a, TBL_I_SIZE);
    fn->par = par;
    fn->args = lst_node_i(a, TYPE(STMT));
    fn->body = lst_node_i(a, TYPE(STMT));
    return fn;
}

type_node *fn_node_ret_type(const fn_node *const fn);

const char *fn_vim_str(fn_vim vim);

void fn_node_p(const ast_st *const as, const fn_node *const fn, size_t idnt);

void fn_node_tbl_data_f(void *data);

inline void fn_node_f(fn_node *fn) {
    tbl_f(fn->tl, &fn_node_tbl_data_f);
    FNNF(fn->sig, type_node_f);
    lst_node_f(fn->args);
    lst_node_f(fn->body);
    alf(fn);
}

typedef struct {
    ast *tgt;
    type_node *ret;
    lst_node *args;
} call_node;

inline call_node *call_node_i(al *const a, ast *const tgt, lst_node *const args) {
    call_node *cn = ala(a, sizeof(call_node));
    cn->tgt = tgt;
    cn->args = args;
    return cn;
}

inline void call_node_p(const ast_st *const as, const call_node *const cn, size_t idnt) {
    type_node_p(as, cn->ret, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("T-");
    ast_p(as, cn->tgt, idnt);
    putchar('\n');
    PCX(' ', idnt);
    printf("A-");
    lst_node_p(as, cn->args, idnt);
}

inline void call_node_f(call_node *cn) {
    ast_f(cn->tgt);
    FNNF(cn->ret, type_node_f);
    lst_node_f(cn->args);
    alf(cn);
}

typedef struct {
    type_node *tn;
    ast *a;
} ret_node; // takes next stmt

inline ret_node *ret_node_i(al *const a) {
    return ala(a, sizeof(ret_node));
}

inline void ret_node_p(const ast_st *const as, const ret_node *const r, size_t idnt) {
    type_node_p(as, r->tn, idnt);
    putchar('\n');
    PCX(' ', idnt);
    ast_p(as, r->a, idnt);
}

inline void ret_node_f(ret_node *r) {
    FNNF(r->tn, type_node_f);
    ast_f(r->a);
    alf(r);
}

#define VAR_TYPE(N) VAR_TYPE_##N

typedef enum {
    VAR_TYPE(U), // unknown
    VAR_TYPE(G), // global
    VAR_TYPE(L), // local
    VAR_TYPE(A) // arg
} var_type;

const char *var_type_str(var_type vt);

typedef struct {
    uint8_t id;
    var_type vt;
    type_node *tn; // null for unknown
    fn_node *fns; // scope
    char str[]; // null term
} var_node; // tbl itm data TODO struct padding

var_node *var_node_i(al *const a, fn_node *const fns, const tkn *const t, const char *const str);

inline void var_node_p(const ast_st *const as, const var_node *const var, size_t idnt) {
    printf("%p,(%s),%d,%s", var->fns, var_type_str(var->vt), var->id, var->str);
    type_node_p(as, var->tn, idnt);
}

inline void var_node_f(var_node *vn) {
    FNNF(vn->tn, type_node_f);
    alf(vn);
}

#define AST_TYPE(N) AST_TYPE_##N

typedef enum {
    AST_TYPE(TYPE),
    AST_TYPE(RES),
    AST_TYPE(VAL),
    AST_TYPE(OP),
    AST_TYPE(LST),
    AST_TYPE(IF),
    AST_TYPE(LOP),
    AST_TYPE(FN),
    AST_TYPE(CALL),
    AST_TYPE(RET),
    AST_TYPE(VAR)
} ast_type;

typedef union {
    type_node *tn;
    res_node *rn;
    val_node *val;
    op_node *op;
    lst_node *lst;
    if_node *in;
    if_itm *lop;
    fn_node *fn;
    call_node *cn;
    ret_node *ret;
    var_node *var;
} node;

typedef struct _ast {
    ast_type at;
    uint16_t rc;
    node n;
    tkn t;
} ast;

inline ast *ast_i(al *const am, ast_type at, node const n, const tkn *const t) {
    ast *a = ala(am, sizeof(ast));
    a->at = at;
    a->rc = 1;
    a->n = n;
    memcpy(&a->t, t, sizeof(tkn));
    return a;
}

inline type_node *ast_gtn(const ast *const a) {
    switch (a->at) {
        case AST_TYPE(TYPE): return a->n.tn;
        case AST_TYPE(RES): return a->n.rn->tn;
        case AST_TYPE(VAL): return a->n.val->tn;
        case AST_TYPE(OP): return a->n.op->ret;
        case AST_TYPE(LST): return a->n.lst->tn;
        case AST_TYPE(IF): return NULL;
        case AST_TYPE(LOP): return NULL;
        case AST_TYPE(FN): return a->n.fn->sig;
        case AST_TYPE(CALL): return a->n.cn->ret;
        case AST_TYPE(RET): return ast_gtn(a->n.ret->a);
        case AST_TYPE(VAR): return a->n.var->tn;
    }
    return NULL;
}

// a must be init
ast_stat ast_parse_stmt(ast_st *const as, fn_node *const fns, ast **a, uint8_t stp_flgs);

ast_stat ast_parse_stmts(ast_st *const as, fn_node *const fns, lst_node *const lst, uint8_t stp_flgs, uint8_t end_flgs);
