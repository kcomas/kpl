
#include "var.h"

var_sg *var_sg_i(size_t size) {
    var_sg *sg = calloc(1, sizeof(var_sg) + size * sizeof(char));
    sg->size = size;
    return sg;
}

var_sg *var_sg_i_str(const char *const str) {
    size_t len = strlen(str);
    var_sg *sg = var_sg_i(len + sizeof(char));
    sg->len = len;
    strcpy(sg->str, str);
    return sg;
}

void var_sg_rci(var_sg *const sg) {
    sg->rc++;
}

size_t var_sg_len(var_sg *const sg) {
    return sg->len;
}

const char *var_sg_str(var_sg *const sg) {
    return sg->str;
}

var_sg *var_sg_cnct_sg_sg(const var_sg *const l, const var_sg *const r) {
    var_sg *sg = var_sg_i(l->len + r->len + sizeof(char));
    memcpy(sg->str, l->str, l->len * sizeof(char));
    sg->len += l->len;
    memcpy(sg->str + l->len * sizeof(char), r->str, r->len * sizeof(char));
    sg->len += r->len;
    return sg;
}

var_sg *var_sg_cnct_sg_te_vr(const var_sg *const l, const var_te_vr *const r) {
    size_t size = l->len * (r->len - 1) + sizeof(char);
    for (size_t i = 0; i < r->len; i++) size += r->v[i].sg->len;
    var_sg *sg = var_sg_i(size);
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

void var_sg_f(var_sg *sg) {
    if (--sg->rc > 0) return;
    free(sg);
}

ssize_t var_rcd(var v, type t) {
    switch (t) {
        case TYPE(STR):
        case TYPE(SG):
            return --v.sg->rc;
        case TYPE(TE): return --v.te->rc;
        case TYPE(VR): return --v.vr->rc;
        default:
            break;
    }
    exit(KPLE); // runtime error should be could during jit
    return 0;
}

bool var_zoo_u6(uint64_t v) { return v != 0; }
bool var_zoo_i6(int64_t v) { return v != 0; }

#define VAR_BOP_T(N, OP, T, CT) VAR_FN_BOP_T(N, T, CT) {  return l OP r; }

// add
VAR_BOP_T(add, +, bl, bool)
VAR_BOP_T(add, +, i6, int64_t)
VAR_BOP_T(add, +, u6, uint64_t)

// sub
VAR_BOP_T(sub, -, bl, bool)
VAR_BOP_T(sub, -, i6, int64_t)
VAR_BOP_T(sub, -, u6, uint64_t)

// eq
VAR_BOP_T(eq, ==, bl, bool)
VAR_BOP_T(eq, ==, i6, int64_t)
VAR_BOP_T(eq, ==, u6, uint64_t)

// gt
VAR_BOP_T(gt, >, bl, bool)
VAR_BOP_T(gt, >, i6, int64_t)
VAR_BOP_T(gt, >, u6, uint64_t)

// lt
VAR_BOP_T(lt, <, bl, bool)
VAR_BOP_T(lt, <, i6, int64_t)
VAR_BOP_T(lt, <, u6, uint64_t)

// or
VAR_BOP_T(or, ||, bl, bool)
VAR_BOP_T(or, ||, i6, int64_t)
VAR_BOP_T(or, ||, u6, uint64_t)

#ifndef NUM_TO_SG_SIZE
    #define NUM_TO_SG_SIZE 30
#endif

// TODO check if output fails
#define INT_TO_SG(FMT, ARG)  var_sg *sg = var_sg_i(NUM_TO_SG_SIZE); \
    sg->len = (size_t) snprintf(sg->str, sg->size, FMT, ARG); \
    return sg

var_sg *var_u6_sg(uint64_t u6) { INT_TO_SG("%lu", u6); }

var_sg *var_i6_sg(uint64_t i6) { INT_TO_SG("%ld", i6); }

bool var_not(bool v) { return !v; }

var_te_vr *var_te_vr_i(size_t size, jit_fn *gc) {
    var_te_vr *vtv = calloc(1, sizeof(var_te_vr) + size * sizeof(var));
    vtv->size = size;
    vtv->gc = gc;
    return vtv;
}

var_te_vr *var_te_i(size_t size, jit_fn *gc) {
    var_te_vr *te = var_te_vr_i(size, gc);
    te->len = size;
    return te;
}

jit_fn *var_te_vr_gc(var_te_vr *const vtv) {
    return vtv->gc;
}

void var_te_vr_sidx(var_te_vr *const vtv, size_t idx, var v) {
    vtv->v[idx] = v;
}

var var_te_vr_gidx(var_te_vr *const vtv, size_t idx) {
    return vtv->v[idx];
}

void var_te_vr_f(var_te_vr *vtv) {
    free(vtv);
}
