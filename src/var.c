
#include "var.h"

var_sg *var_sg_i(al *const a, size_t size) {
    var_sg *sg = ala(a, sizeof(var_sg));
    sg->str = ala(a, size * sizeof(char));
    sg->size = size;
    return sg;
}

var_sg *var_sg_i_str(al *const a, const char *const str) {
    size_t len = strlen(str);
    var_sg *sg = var_sg_i(a, len + sizeof(char));
    sg->len = len;
    strcpy(sg->str, str);
    return sg;
}

void var_sg_rci(var_sg *const sg) {
    sg->rc++;
}

void var_sg_rcd(var_sg *const sg) {
    sg->rc--;
}

size_t var_sg_len(var_sg *const sg) {
    return sg->len;
}

const char *var_sg_str(var_sg *const sg) {
    return sg->str;
}

var_sg *var_sg_cnct_sg_sg(al *const a, const var_sg *const l, const var_sg *const r) {
    var_sg *sg = var_sg_i(a, l->len + r->len + sizeof(char));
    memcpy(sg->str, l->str, l->len * sizeof(char));
    sg->len += l->len;
    memcpy(sg->str + l->len * sizeof(char), r->str, r->len * sizeof(char));
    sg->len += r->len;
    return sg;
}

var_sg *var_sg_cnct_sg_tsv(al *const a, const var_sg *const l, const var_tsv *const r) {
    size_t size = l->len * (r->len - 1) + sizeof(char);
    for (size_t i = 0; i < r->len; i++) size += r->v[i].sg->len;
    var_sg *sg = var_sg_i(a, size);
    for (size_t i = 0; i < r->len - 1; i++) {
        memcpy(sg->str + sg->len * sizeof(char), r->v[i].sg->str, r->v[i].sg->len * sizeof(char));
        sg->len += r->v[i].sg->len;
        memcpy(sg->str + sg->len * sizeof(char), l->str, l->len * sizeof(char));
        sg->len += l->len;
    }
    memcpy(sg->str + sg->len * sizeof(char), r->v[r->len - 1].sg->str, r->v[r->len - 1].sg->len * sizeof(char));
    sg->len += r->v[r->len - 1].sg->len;
    return sg;
}

void var_sg_d(var_sg *sg) {
    alf(sg->str);
    alf(sg);
}

void var_sg_f(var_sg *sg) {
    if (--sg->rc >= 0) return;
    var_sg_d(sg);
}

static er_itm *er_var(mod *const m, ast *const a, const char *stat) {
    er_itm *ei = er_itm_i(m->r->a, ER(RUN), __func__, stat);
    ei->path = m->src.path;
    if (a) {
        ei->lno = a->t.lno;
        ei->cno = a->t.cno;
    }
    return ei;
}

void er_rt(mod *const m, ast *const a, er_itm *const ei) {
    er_a(m->r->e, ei);
    er_a(m->r->e, er_var(m, a, "RETHROW"));
}

void var_sg_er(mod *const m, ast *const a, var_sg *const sg) {
    er_itm *ei = er_var(m, a, NULL);
    ei->sg = sg;
    er_a(m->r->e, ei);
}

ssize_t var_rcf(var v, type t) {
    switch (t) {
        case TYPE(STR):
        case TYPE(SG):
            return --v.sg->rc;
        case TYPE(VR):
        case TYPE(TE):
        case TYPE(ST):
            return --v.tsv->rc;
        case TYPE(ER):
            return --v.ei->rc;
        case TYPE(TD):
            return --v.td->rc;
        default:
            break;
    }
    exit(KPLE); // runtime error should be caught during jit
    return 0;
}

bool var_nvd(var v) { return v.vd != NULL; }

bool var_zoo_u6(uint64_t v) { return v != 0; }
bool var_zoo_i6(int64_t v) { return v != 0; }
bool var_zoo_er(er_itm *const ei) { return ei->et != ER(OK); }

static void dig_len(size_t *len, uint64_t num) {
    while (num) {
        ++*len;
        num /= 10;
    }
}

static void str_w_dig(var_sg *const sg, size_t *len, uint64_t num) {
    while (num) {
        sg->str[--*len] = num % 10 + '0';
        num /= 10;
    }
}

static var_sg *int_sg(al *const a, bool neg, uint64_t num) {
    size_t len = 0;
    dig_len(&len, num);
    if (neg) len++;
    var_sg *sg = var_sg_i(a, len);
    sg->len = len;
    str_w_dig(sg, &len, num);
    if (neg) sg->str[0] = '-';
    return sg;
}

var_sg *var_i6_sg(al *const a, int64_t i6) {
    if (i6 == 0) return var_sg_i_str(a, "0");
    if (i6 > 0) return int_sg(a, false, (uint64_t) i6);
    return int_sg(a, true, (uint64_t) 0 - i6);
}

var_sg *var_u6_sg(al *const a, uint64_t u6) {
    if (u6 == 0) return var_sg_i_str(a, "0");
    return int_sg(a, false, u6);
}

#ifndef FLT_DEC_PREC
    #define FLT_DEC_PREC 6
#endif

static const uint64_t flp10[] = {1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9};

var_sg *var_f6_sg(al *const a, double f6) {
    size_t len = 0;
    bool neg = false;
    if (f6 < 0) {
        neg = true;
        len++;
        f6 = 0 - f6;
    }
    int64_t up = (int64_t) f6;
    if (up == 0) len++;
    else dig_len(&len, up);
    len += FLT_DEC_PREC + 1; // for .
    var_sg *sg = var_sg_i(a, len);
    sg->len = len;
    int64_t frac = (f6 - up) * flp10[FLT_DEC_PREC];
    if (frac == 0) sg->str[--len] = '0';
    else str_w_dig(sg, &len, frac);
    sg->str[--len] = '.';
    if (up == 0) sg->str[--len] = '0';
    else str_w_dig(sg, &len, up);
    if (neg) sg->str[0] = '-';
    return sg;
}

#define VAR_BOP_T(N, OP, T, CT) VAR_FN_BOP_T(N, T, CT) {  return l OP r; }

// add
VAR_BOP_T(add, +, bl, bool)
VAR_BOP_T(add, +, i6, int64_t)
VAR_BOP_T(add, +, u6, uint64_t)
VAR_BOP_T(add, +, f6, double)

// sub
VAR_BOP_T(sub, -, bl, bool)
VAR_BOP_T(sub, -, i6, int64_t)
VAR_BOP_T(sub, -, u6, uint64_t)
VAR_BOP_T(sub, -, f6, double)

// mul
VAR_BOP_T(mul, &&, bl, bool)
VAR_BOP_T(mul, *, i6, int64_t)
VAR_BOP_T(mul, *, u6, uint64_t)
VAR_BOP_T(mul, *, f6, double)

// div
VAR_BOP_T(div, ||, bl, bool)
VAR_BOP_T(div, /, i6, int64_t)
VAR_BOP_T(div, /, u6, uint64_t)
VAR_BOP_T(div, /, f6, double)

// eq
VAR_BOP_T(eq, ==, bl, bool)
VAR_BOP_T(eq, ==, i6, int64_t)
VAR_BOP_T(eq, ==, u6, uint64_t)
VAR_BOP_T(eq, ==, f6, double)

// gt
VAR_BOP_T(gt, >, bl, bool)
VAR_BOP_T(gt, >, i6, int64_t)
VAR_BOP_T(gt, >, u6, uint64_t)
VAR_BOP_T(gt, >, f6, double)

// lt
VAR_BOP_T(lt, <, bl, bool)
VAR_BOP_T(lt, <, i6, int64_t)
VAR_BOP_T(lt, <, u6, uint64_t)
VAR_BOP_T(lt, <, f6, double)

// or
VAR_BOP_T(or, ||, bl, bool)
VAR_BOP_T(or, ||, i6, int64_t)
VAR_BOP_T(or, ||, u6, uint64_t)
VAR_BOP_T(or, ||, f6, double)

bool var_not(bool v) { return !v; }

var_tsv *var_tsv_i(al *const a, size_t size, size_t len, jit_fn *gc) {
    var_tsv *vtsv = ala(a, sizeof(var_tsv));
    vtsv->v = ala(a, size * sizeof(var));
    vtsv->size = size;
    vtsv->len = len;
    vtsv->gc = gc;
    return vtsv;
}

var_tsv *var_ts_fm(al *const a, mod *const m, jit_fn *gc) {
    var_tsv *ts = var_tsv_i(a, (size_t) m->ng, (size_t) m->ng, gc);
    for (uint8_t i = 0; i < m->ng; i++) ts->v[i] = m->g[i];
    return ts;
}

size_t var_tsv_len(var_tsv *const tsv) {
    return tsv->len;
}

void var_tsv_rci(var_tsv *const tsv) {
    tsv->rc++;
}

void var_tsv_rcd(var_tsv *const tsv) {
    tsv->rc--;
}

jit_fn *var_tsv_gc(var_tsv *const tsv) {
    return tsv->gc;
}

var var_tsv_gidx(var_tsv *const tsv, size_t idx) {
    return tsv->v[idx];
}

void var_tsv_sidx(var_tsv *const tsv, size_t idx, var v) {
    tsv->v[idx] = v;
}

void var_tsv_add(al *const a, var_tsv *const tsv, var v) {
    if (tsv->len == tsv->size) {
        size_t nsize = tsv->size * TSVML;
        var *nv = ala(a, nsize * sizeof(var));
        for (size_t i = 0; i < tsv->size; i++) nv[i] = tsv->v[i];
        alf(tsv->v);
        tsv->size = nsize;
        tsv->v = nv;
    }
    tsv->v[tsv->len++] = v;
}

var var_tsv_sub(mod *const m, ast *const a, var_tsv *const tsv) {
    if (tsv->len == 0) {
        er_itm *ei = er_var(m, a, "VRS");
        er_a(m->r->e, ei);
        return (var) { .i6 = 0 };
    }
    return tsv->v[--tsv->len];
}

void var_tsv_d(var_tsv *tsv) {
    alf(tsv->v);
    alf(tsv);
}


var_td *var_td_i(mod *const m, var_tsv *const te, code *const c) {
    var_td *td = ala(m->r->a, sizeof(var_td));
    td->m = m;
    td->m->c = c;
    td->te = te;
    return td;
}

void var_td_rci(var_td *const td) {
    td->rc++;
}

void var_td_rcd(var_td *const td) {
    td->rc--;
}

var_tsv *var_td_te(var_td *const td) {
    return td->te;
}

void var_td_d(var_td *td) {
    tds_a(td->m->s, td->m->r);
    mod_f(td->m);
    alf(td);
}

void var_td_f(var_td *td) {
    if (--td->rc >= 0) return;
    var_td_d(td);
}
