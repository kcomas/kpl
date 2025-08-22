
#include "idx.h"

#define CFLG(N) CFLG_##N

typedef enum {
    CFLG(NPR) = 1 << 0, // no preserve registers
    CFLG(NR) = 1 << 1, // no return value
    CFLG(V) = 1 << 2
} cflgs;

typedef struct {
    uint8_t r;
    int8_t i, a;
    un d;
} rac; // reg to, arg idx, reg from, data

#ifndef MAX_R_STK
    #define MAX_R_STK 6
#endif

static gen_stat ur(gen_st *st, rac args[14], int8_t r, uint8_t *cr, uint8_t *cx, as *a, te *ci) {
    for (size_t i = 0; i < 14; i++) {
        if (args[i].a != r) continue;
        if (args[i].a < XMM(0)) {
            if (st->rstk->l <= *cr) return GEN_STAT(INV);
            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), st->rstk->d[*cr]), as_arg_i(a, ARG_ID(R), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            args[i].a = st->rstk->d[*cr].u3;
            ++*cr;
        } else {
            if (st->xstk->l <= *cx) return GEN_STAT(INV);
            if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), st->xstk->d[*cx]), as_arg_i(a, ARG_ID(X), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return GEN_STAT(INV);
            args[i].a = st->xstk->d[*cx].u3;
            ++*cx;
        }
        break;
    }
    return GEN_STAT(OK);
}

static gen_stat call(gen *g, gen_st *st, te *restrict ci, as *a, err **e, te *restrict cret, vr *restrict vs, vr *restrict va, te *restrict cfn, uint8_t flgs) {
    gen_stat stat;
    bool swaps = false;
    int8_t ras = 0;
    uint8_t ra = 0, xa = 6, cr = 0, cx = 0, align = 0;
    int32_t idx;
    rac args[] = {
        { R(DI), -1, -1, U6(0) },
        { R(SI), -1, -1, U6(0) },
        { R(DX), -1, -1, U6(0) },
        { R(CX), -1, -1, U6(0) },
        { R(8), -1, -1, U6(0) },
        { R(9), -1, -1, U6(0) },
        { XMM(0), -1, -1, U6(0) },
        { XMM(1), -1, -1, U6(0) },
        { XMM(2), -1, -1, U6(0) },
        { XMM(3), -1, -1, U6(0) },
        { XMM(4), -1, -1, U6(0) },
        { XMM(5), -1, -1, U6(0) },
        { XMM(6), -1, -1, U6(0) },
        { XMM(7), -1, -1, U6(0) }
    }, tmp;
    size_t kvi = 0;
    te *ovt, *kvs, *kv[14], *rkv, *h, *sa[MAX_R_STK];
    reg ret = R(AX), save;
    if (ras && vs) return gen_err(g, ci, e, "gen err reg overflow cannot pass NF stack args");
    if (!(flgs & CFLG(NR))) {
        if ((stat = get_reg(st, cret, &rkv)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
        ret = rkv->d[2].u3;
    }
    uint8_t ri = 0, xi = 0, vi = 0;
    reg rs[12], xs[14];
    if (!(flgs & CFLG(NPR))) {
        h = st->atm->i->h;
        while (h) {
            if (tbl_g_i(st->lat, ((te*) h->d[0].p)->d[0], &kvs) == TBL_STAT(OK) && kvs->d[2].p == ci) {
                h = h->d[2].p;
                continue; // do not save reg if this is the last time used
            }
            save = ((te*) h->d[0].p)->d[2].u3;
            if (flgs & CFLG(NR)) {
                if (save < XMM(0)) rs[ri++] = save;
                else xs[xi++] = save;
            } else if (save != ret) {
                if (save < XMM(0)) rs[ri++] = save;
                else xs[xi++] = save;
            }
            h = h->d[2].p;
        }
    }
    /*
    for (size_t i = 0; i < 12; i++) {
        if (args[i].i < 0) continue;
        printf("r: %s, i: %d, ", reg_str(args[i].r), args[i].i);
        if (args[i].a > -1) printf("a: %s, ", reg_str(args[i].a));
        printf("d: 0x%lX\n", args[i].d.u6);
    }
    for (size_t i = 0; i < ri; i++) printf("%s ", reg_str(rs[i]));
    if (ri > 0) putchar('\n');
    */
    if (xi > 0) putchar('\n');
    if (ras & 1) align++;
    if (vs && vs->l & 1) align++;
    if (ri & 1) align++;
    if (xi & 1) align++;
    if (align & 1 && gen_as(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (ri > 0) {
        for (size_t i = 0; i < ri; i++) if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(rs[i])), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    if (xi > 0) {
        if (gen_as(a, AS_X64(SUB), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * xi)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        for (size_t i = 0; i < xi; i++) {
            if (i) {
                if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * i)), as_arg_i(a, ARG_ID(X), U3(xs[i])), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(X), U3(xs[i])), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        }
    }
    if (vs) {
        for (size_t i = 0; i < vs->l; i++) {
            ovt = vs->d[i].p;
            switch (gen_var_g_c(ovt)) {
                case GEN_CLS(A):
                case GEN_CLS(T):
                    if (!x64_type_is_ref(gen_var_g_t(ovt))) return gen_err(g, ci, e, "gen arg tmp stk arg not ref");
                    if (get_reg(st, ovt, &kvs) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv arg tmp stk reg");
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), kvs->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    drop_atm_kv(st, kvs, ci);
                    break;
                case GEN_CLS(V):
                    if (st_stkv_idx(st, gen_var_g_t(ovt), ovt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen call inv stk arg");
                    if (!x64_type_is_ref(gen_var_g_t(ovt))) {
                        if (gen_as_rrmbd(a, AS_X64(LEA), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    } else return gen_err(g, ci, e, "gen stk ref nyi");
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                case GEN_CLS(S):
                    if (st_stka_idx(gen_var_g_t(ovt), ovt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen call inv stka arg");
                    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                case GEN_CLS(I):
                    if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call idx no tmp r regs");
                    if ((stat = idx_from(g, st, ci, a, e, AS_X64(LEA), ovt->d[1].p, as_arg_i(a, ARG_ID(R), U3(R(AX))), st->rstk->d[0].u3, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                default:
                    return gen_err(g, ci, e, "gen stk args cls inv");
            }
        }
    }
    if (va) {
        for (size_t i = 0; i < va->l; i++) {
            if (xa > 13) return gen_err(g, ci, e, "gen call too many args");
            ovt = va->d[i].p;
            if (ra > 5 && gen_var_g_t(ovt) < X64_TYPE(F5)) {
                if (ras == MAX_R_STK) return gen_err(g, ci, e, "gen err too many args");
                sa[ras++] = ovt;
                continue;
            }
            switch (gen_var_g_c(ovt)) {
                case GEN_CLS(A):
                case GEN_CLS(T):
                    if ((stat = get_reg(st, ovt, &kv[kvi])) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(F5):
                        case X64_TYPE(F6):
                            args[xa].i = i;
                            args[xa].a = kv[kvi]->d[2].u3;
                            kvi++;
                            break;
                        case X64_TYPE(U3):
                        case X64_TYPE(I6):
                        case X64_TYPE(U6):
                        case X64_TYPE(M):
                        case X64_TYPE(MM):
                            args[ra].i = i;
                            args[ra].a = kv[kvi]->d[2].u3;
                            kvi++;
                            break;
                        default:
                            return gen_err(g, ci, e, "gen call cls AT inv type");
                    }
                    break;
            }
            switch (gen_var_g_t(ovt)) {
                case X64_TYPE(F5):
                case X64_TYPE(F6):
                    xa++;
                    break;
                default:
                    ra++;
                    break;
            }
        }
        ra = 0;
        xa = 6;
        for (size_t i = 0; i < va->l; i++) {
            ovt = va->d[i].p;
            if (ra > 5 && gen_var_g_t(ovt) < X64_TYPE(F5)) continue;
            switch (gen_var_g_c(ovt)) {
                case GEN_CLS(A):
                case GEN_CLS(T):
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(F5):
                        case X64_TYPE(F6):
                            xa++;
                            break;
                        case X64_TYPE(U3):
                        case X64_TYPE(I6):
                        case X64_TYPE(U6):
                        case X64_TYPE(M):
                        case X64_TYPE(MM):
                            ra++;
                            break;
                        default:
                            return gen_err(g, ci, e, "gen call cls AT inv type");
                    }
                    break;
                case GEN_CLS(D):
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(S):
                            args[ra].i = i;
                            if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), U3(args[ra].r)), as_arg_i(a, ARG_ID(S), ovt->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, "gen data inv S");
                            args[ra].a = args[ra].r;
                            ra++;
                            break;
                        case X64_TYPE(F5):
                        case X64_TYPE(F6):
                            args[xa].i = i;
                            args[xa].d = ovt->d[1];
                            xa++;
                            break;
                        case X64_TYPE(U3):
                        case X64_TYPE(U4):
                        case X64_TYPE(U5):
                        case X64_TYPE(U6):
                        case X64_TYPE(I3):
                        case X64_TYPE(I4):
                        case X64_TYPE(I5):
                        case X64_TYPE(I6):
                        case X64_TYPE(M):
                            args[ra].i = i;
                            args[ra].d = ovt->d[1];
                            ra++;
                            break;
                        default:
                            return gen_err(g, ci, e, "gen call cls D inv type");
                    }
                    break;
                case GEN_CLS(V):
                    if ((stat = st_stkv_idx(st, gen_var_g_t(ovt), ovt->d[1].u3, &idx)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv call stk idx");
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(F5):
                        case X64_TYPE(F6):
                            args[xa].i = i;
                            if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                            if (gen_as_rrmbd(a, AS_X64(MOVSD), args[xa].r, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                            args[xa].a = args[xa].r;
                            xa++;
                            break;
                        default:
                            args[ra].i = i;
                            if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                            if (gen_as_rrmbd(a, AS_X64(MOV), args[ra].r, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                            args[ra].a = args[ra].r;
                            ra++;
                            break;
                    }
                    break;
                case GEN_CLS(I):
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(F5):
                        case X64_TYPE(F6):
                            args[xa].i = i;
                            if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                            if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call idx no tmp r regs");
                            if ((stat = idx_from(g, st, ci, a, e, AS_X64(MOVSD), ovt->d[1].p, as_arg_i(a, ARG_ID(X), U3(args[xa].r)), st->rstk->d[0].u3, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                            args[xa].a = args[xa].r;
                            xa++;
                            break;
                        case X64_TYPE(I6):
                        case X64_TYPE(U6):
                        case X64_TYPE(M):
                            args[ra].i = i;
                            if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                            if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call idx no tmp r regs");
                            if ((stat = idx_from(g, st, ci, a, e, AS_X64(MOV), ovt->d[1].p, as_arg_i(a, ARG_ID(R), U3(args[ra].r)), st->rstk->d[0].u3, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                            args[ra].a = args[ra].r;
                            ra++;
                            break;
                        case X64_TYPE(MM):
                            if (((vr*) ovt->d[1].p)->l == 1) {
                                h = ((vr*) ovt->d[1].p)->d[0].p;
                                switch (gen_var_g_c(h)) {
                                    case GEN_CLS(V):
                                        args[ra].i = i;
                                        if ((stat = st_stkv_idx(st, gen_var_g_t(h), h->d[1].u3, &idx)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv call stk idx");
                                        if (gen_as_rrmbd(a, AS_X64(LEA), args[ra].r, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                                        args[ra].a = args[ra].r;
                                        ra++;
                                        break;
                                    default:
                                        return gen_err(g, ci, e, "gen inv single MM cls");
                                }
                            } else {
                                args[ra].i = i;
                                if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                                if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call idx no tmp r regs");
                                if ((stat = idx_from(g, st, ci, a, e, AS_X64(LEA), ovt->d[1].p, as_arg_i(a, ARG_ID(R), U3(args[ra].r)), st->rstk->d[0].u3, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                                args[ra].a = args[ra].r;
                                ra++;
                            }
                            break;
                        default:
                            return gen_err(g, ci, e, "gen call cls I inv type");
                    }
                    break;
                case GEN_CLS(S):
                    if ((stat = st_stka_idx(gen_var_g_t(ovt), ovt->d[1].u3, &idx)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv call stka idx");
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(MI6):
                        case X64_TYPE(MM):
                            args[ra].i = i;
                            if (ur(st, args, args[ra].r, &cr, &cx, a, ci) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen update arg reg inv");
                            if (gen_as_rrmbd(a, AS_X64(MOV), args[ra].r, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(args[ra].r)), as_arg_i(a, ARG_ID(RM), U3(args[ra].r)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                            args[ra].a = args[ra].r;
                            ra++;
                            break;
                        default:
                            return gen_err(g, ci, e, "gen call cls S inv type");
                    }
                    break;
                default:
                    return gen_err(g, ci, e, "gen call inv ovt cls");
            }
        }
    }
    for (ssize_t i = 13; i > 0; i--) {
        if (args[i].a < 0) continue;
        for (ssize_t n = i - 1; n >= 0; n--) {
            if (args[i].a == args[n].r) {
                tmp = args[i];
                args[i] = args[n];
                args[n] = tmp;
                swaps = true;
            }
        }
    }
    if (swaps) {
        for (ssize_t i = 13; i > 0; i--) {
            if (args[i].a < 0) continue;
            for (ssize_t n = i - 1; n >= 0; n--) {
                if (args[n].a < 0) continue;
                if (args[i].a == args[n].r) {
                    if (args[i].a < XMM(0)) {
                        if (st->rstk->l <= cr) return gen_err(g, ci, e, "gen r clash no regs");
                        if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), st->rstk->d[cr]), as_arg_i(a, ARG_ID(R), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                        args[i].a = st->rstk->d[cr].u3;
                        cr++;
                    } else {
                        if (st->xstk->l <= cx) return gen_err(g, ci, e, "gen x clash no regs");
                        if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), st->xstk->d[cx]), as_arg_i(a, ARG_ID(X), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                        args[i].a = st->xstk->d[cx].u3;
                        cx++;
                    }
                }
            }
        }
    }
    for (size_t i = 0; i < ra; i++) {
        if (args[i].i < 0) continue;
        if (args[i].a > -1) {
            if (args[i].r != args[i].a && gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(args[i].r)), as_arg_i(a, ARG_ID(R), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else {
            if (args[i].d.u6) {
                if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(args[i].r)), as_arg_i(a, ARG_ID(QW), args[i].d), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else {
                if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(args[i].r)), as_arg_i(a, ARG_ID(R), U3(args[i].r)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            }
        }
    }
    for (size_t i = 6; i < xa; i++) {
        if (args[i].i < 0) continue;
        vi++;
        if (args[i].a > -1) {
            if (args[i].r != args[i].a && gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(args[i].r)), as_arg_i(a, ARG_ID(X), U3(args[i].a)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else {
            if (args[i].d.f6) {
                if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(args[i].r)), as_arg_i(a, ARG_ID(QW), args[i].d), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else {
                if (gen_as(a, AS_X64(PXOR), as_arg_i(a, ARG_ID(X), U3(args[i].r)), as_arg_i(a, ARG_ID(X), U3(args[i].r)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            }
        }
    }
    if (ras) {
        for (int8_t i = ras - 1; i >= 0; i--) {
            ovt = sa[i];
            switch (gen_var_g_c(ovt)) {
                case GEN_CLS(A):
                case GEN_CLS(T):
                    if ((stat = get_reg(st, ovt, &kvs)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen reg");
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), kvs->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, "gen data inv S");
                    drop_atm_kv(st, kvs, ci);
                    continue;
                case GEN_CLS(D):
                    switch (gen_var_g_t(ovt)) {
                        case X64_TYPE(S):
                            if (gen_as(a, AS_X64(LEA), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(S), ovt->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, "gen data inv S");
                            break;
                        default:
                            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(QW), ovt->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, "gen data inv S");
                            break;
                    }
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                case GEN_CLS(V):
                    if ((stat = st_stkv_idx(st, gen_var_g_t(ovt), ovt->d[1].u3, &idx)) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv call stk idx");
                    if (gen_as_rrmbd(a, AS_X64(MOV), R(AX), R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                case GEN_CLS(I):
                    if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call idx no tmp r regs");
                    if ((stat = idx_from(g, st, ci, a, e, AS_X64(LEA), ovt->d[1].p, as_arg_i(a, ARG_ID(R), U3(R(AX))), st->rstk->d[0].u3, ARG_ID(R))) != GEN_STAT(OK)) return stat;
                    if (gen_as(a, AS_X64(PUSH), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                default:
                    return gen_err(g, ci, e, "gen call inv ovt cls stk call");
            }
        }
    }
    if (flgs & CFLG(V)) {
        if (vi) {
            if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(B), U3(vi)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else if (gen_as(a, AS_X64(XOR), as_arg_i(a, ARG_ID(R), U3(R(AX))), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    switch (gen_var_g_c(cfn)) {
        case GEN_CLS(A):
        case GEN_CLS(T):
            if (get_reg(st, cfn, &kvs) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen fn inv reg");
            if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(R), kvs->d[2]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(st, kvs, ci);
            break;
        case GEN_CLS(L):
            if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(L), cfn->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            break;
        case GEN_CLS(D):
            switch (gen_var_g_t(cfn)) {
                case X64_TYPE(U5):
                    if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(DW), cfn->d[1]), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                case X64_TYPE(M):
                    if (st->rstk->l == 0) return gen_err(g, ci, e, "gen call mem addr no available regs");
                    if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(st->rstk->d[0].u3)), as_arg_i(a, ARG_ID(QW), cfn->d[1]), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    if (gen_as(a, AS_X64(CALL), as_arg_i(a, ARG_ID(R), U3(st->rstk->d[0].u3)), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                    break;
                default:
                    return gen_err(g, ci, e, "gen call inv cls D call");
            }
            break;
        default:
            return gen_err(g, ci, e, "gen fn cls inv");
    }
    if (!(flgs & CFLG(NR))) {
        switch (gen_var_g_t(cret)) {
            case X64_TYPE(F5):
            case X64_TYPE(F6):
                if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(ret)), as_arg_i(a, ARG_ID(X), U3(XMM(0))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                break;
            default:
                if (gen_as(a, AS_X64(MOV), as_arg_i(a, ARG_ID(R), U3(ret)), as_arg_i(a, ARG_ID(R), U3(R(AX))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                break;
        }
    }
    if (vs && gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * vs->l)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (ras && gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * ras)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    if (xi > 0) {
        for (ssize_t i = xi - 1; i >= 0; i--) {
            if (i) {
                if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xs[i])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * i)), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else if (gen_as(a, AS_X64(MOVSD), as_arg_i(a, ARG_ID(X), U3(xs[i])), as_arg_i(a, ARG_ID(RM), U3(R(SP))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        }
        if (gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*) * xi)), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    if (ri > 0) {
        for (ssize_t i = ri - 1; i >= 0; i--) if (gen_as(a, AS_X64(POP), as_arg_i(a, ARG_ID(R), U3(rs[i])), NULL, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    }
    if (align & 1 && gen_as(a, AS_X64(ADD), as_arg_i(a, ARG_ID(R), U3(R(SP))), as_arg_i(a, ARG_ID(B), U3(sizeof(void*))), NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
    drop_atm_kv_n(st, kv, ci, kvi);
    drop_atm_kv(st, ci, rkv);
    return GEN_STAT(OK);
}

static gen_stat call_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, ci->d[1].p, NULL, ((te*) ci->d[2].p)->d[1].p, ci->d[3].p, 0);
}

static gen_stat callv_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, ci->d[1].p, NULL, ((te*) ci->d[2].p)->d[1].p, ci->d[3].p, CFLG(V));
}

static gen_stat callnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, NULL, NULL, ((te*) ci->d[1].p)->d[1].p, ci->d[2].p, CFLG(NR));
}

static gen_stat callw_fn(gen *g, void *s, te *ci, as *a, err **e) {
   te *w = ((te*) ci->d[2].p)->d[1].p;
   return call(g, s, ci, a, e, ci->d[1].p, w->d[0].p, w->d[1].p, ci->d[3].p, 0);
}

static gen_stat callwnpr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   te *w = ((te*) ci->d[2].p)->d[1].p;
   return call(g, s, ci, a, e, ci->d[1].p, w->d[0].p, w->d[1].p, ci->d[3].p, CFLG(NPR));
}

static gen_stat callwnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   te *w = ((te*) ci->d[1].p)->d[1].p;
   return call(g, s, ci, a, e, NULL, w->d[0].p, w->d[1].p, ci->d[2].p, CFLG(NR));
}

static gen_stat callwnprnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   te *w = ((te*) ci->d[1].p)->d[1].p;
   return call(g, s, ci, a, e, NULL, w->d[0].p, w->d[1].p, ci->d[2].p, CFLG(NR) | CFLG(NPR));
}

static gen_stat callnpr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, ci->d[1].p, NULL, ((te*) ci->d[2].p)->d[1].p, ci->d[3].p, CFLG(NPR));
}

static gen_stat callvnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, NULL, NULL, ((te*) ci->d[1].p)->d[1].p, ci->d[2].p, CFLG(V) | CFLG(NR));
}

static gen_stat callvnprnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return call(g, s, ci, a, e, NULL, NULL, ((te*) ci->d[1].p)->d[1].p, ci->d[2].p, CFLG(V) | CFLG(NR) | CFLG(NPR));
}

static gen_stat callnprnr_fn(gen *g, void *s, te *ci, as *a, err **e) {
   return call(g, s, ci, a, e, NULL, NULL, ((te*) ci->d[1].p)->d[1].p, ci->d[2].p, CFLG(NR) | CFLG(NPR));
}

static gen_stat callnprnrna_fn(gen *g, void *s, te *ci, as *a, err **e) {
    return call(g, s, ci, a, e, NULL, NULL, NULL, ci->d[1].p, CFLG(NPR) | CFLG(NR));
}

void gen_call(gen *g) {
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(M), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(M), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(U5), call_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callw_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callw_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(M), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callw_fn);
    GEN_OP_A2(g, GEN_OP(CALL), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callnr_fn);
    GEN_OP_A2(g, GEN_OP(CALL), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnr_fn);
    GEN_OP_A2(g, GEN_OP(CALL), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callwnr_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(T), X64_TYPE(M), callw_fn);
    GEN_OP_A3(g, GEN_OP(CALL), GEN_CLS(T), X64_TYPE(M), GEN_CLS(W), X64_TYPE(N), GEN_CLS(T), X64_TYPE(M), callw_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callwnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callwnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(M), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callwnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(F6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(M), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(U5), callnpr_fn);
    GEN_OP_A3(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(I6), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callw_fn);
    GEN_OP_A2(g, GEN_OP(CALLNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callvnprnr_fn);
    GEN_OP_A3(g, GEN_OP(CALLV), GEN_CLS(T), X64_TYPE(U6), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callv_fn);
    GEN_OP_A2(g, GEN_OP(CALLV), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnr_fn);
    GEN_OP_A2(g, GEN_OP(CALLVNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callvnprnr_fn);
    GEN_OP_A2(g, GEN_OP(CALLNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(D), X64_TYPE(M), callnprnr_fn);
    GEN_OP_A2(g, GEN_OP(CALLNPR), GEN_CLS(M), X64_TYPE(N), GEN_CLS(T), X64_TYPE(M), callnprnr_fn);
    GEN_OP_A2(g, GEN_OP(CALLNPR), GEN_CLS(W), X64_TYPE(N), GEN_CLS(L), X64_TYPE(N), callwnprnr_fn);
    GEN_OP_A1(g, GEN_OP(CALLNPR), GEN_CLS(L), X64_TYPE(N), callnprnrna_fn);
    GEN_OP_A1(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(M), callnprnrna_fn);
    GEN_OP_A1(g, GEN_OP(CALLNPR), GEN_CLS(T), X64_TYPE(M), callnprnrna_fn);
}
