
#include "gen_x64.h"

const char *gen_op_str(gen_op go) {
    static const char *gos[] = {
        "_START",
        "ENTER",
        "LEAVE",
        "ADD",
        "_END"
    };
    const char *s = "INV";
    if (go > GEN_OP(_START) && go < GEN_OP(_END)) s = gos[go];
    return s;
}

const char *x64_type_str(x64_type xt) {
    switch (xt) {
        case X64_TYPE(N): return "N";
        case X64_TYPE(M): return "M";
        case X64_TYPE(U3): return "U3";
        case X64_TYPE(U4): return "U4";
        case X64_TYPE(U5): return "U5";
        case X64_TYPE(U6): return "U6";
        case X64_TYPE(I3): return "I3";
        case X64_TYPE(I4): return "I4";
        case X64_TYPE(I5): return "I5";
        case X64_TYPE(I6): return "I6";
        case X64_TYPE(F5): return "F5";
        case X64_TYPE(F6): return "F6";
        default:
            break;
    }
    return "INV";
}

gen_st *gen_st_i(alfn *sa, frfn *sf, frfn *atmf, frfn *latf, tbl *atm, tbl *lat, vr *rstk, vr *xstk) {
    gen_st *st = sa(sizeof(gen_st));
    st->vc = st->rac = st->xac = 0;
    st->r = 1;
    st->sa = sa;
    st->sf = sf;
    st->atmf = atmf;
    st->latf = latf;
    st->atm = atm;
    st->lat = lat;
    st->rstk = rstk;
    st->xstk = xstk;
    return st;
}

void gen_st_p(const gen_st *st) {
    printf("vc: %u, rac: %u, xac: %u\n", st->vc, st->rac, st->xac);
    printf("lat_l: %lu\nR:", st->lat->i->l);
    for (size_t i = 0; i < st->rstk->l; i++) printf("%s ", reg_str(st->rstk->d[i].u3));
    printf("\nX:");
    for (size_t i = 0; i < st->xstk->l; i++) printf("%s ", reg_str(st->xstk->d[i].u3));
    putchar('\n');
}

static void update_lat(gen_st *st, te *ovt, te *o) {
    un hsh = U6((ovt->d[0].u3  << 8) + ovt->d[2].u3);
    te *kv;
    if (tbl_g_i(st->lat, hsh, &kv) == TBL_STAT(OK)) {
        te_f(kv->d[1].p);
        te_f(kv->d[2].p);
        kv->d[1] = P(te_c(ovt));
        kv->d[2] = P(te_c(o));
    } else {
        kv = te_i(3, st->sa, st->latf);
        kv->d[0] = hsh;
        kv->d[1] = P(te_c(ovt));
        kv->d[2] = P(te_c(o));
        tbl_a(st->lat, kv);
    }
}

gen_stat gen_st_p1(gen *g, gen_st *st, as *a) {
    // swap R(DX) with R(10), R(CX) with R(11), XMM(0) with XMM(7)
    static reg r[] = {R(12),  R(13), R(14), R(15), R(BX), R(9), R(8), R(11), R(10), R(SI), R(DI)};
    for (size_t i = 0; i < 11; i++) vr_ab(&st->rstk, U3(r[i]));
    static reg x[] = {XMM(15), XMM(14), XMM(13), XMM(12), XMM(11), XMM(10), XMM(9), XMM(8), XMM(6), XMM(5), XMM(4), XMM(3), XMM(2), XMM(1), XMM(7)};
    for (size_t i = 0; i < 15; i++) vr_ab(&st->xstk, U3(x[i]));
    size_t ral = 0, xal = 0;
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        for (size_t i = 1; i < 4; i++) {
            te *ovt = o->d[i].p;
            if (!ovt) break;
            switch (ovt->d[0].u3) {
                case GEN_CLS(A):
                    if (ovt->d[1].u3 >= X64_TYPE(M) && ovt->d[1].u3 <= X64_TYPE(I6)) st->rac = ovt->d[2].u3 > st->rac ? ovt->d[2].u3 : st->rac;
                    else st->xac = ovt->d[2].u3 > st->xac ? ovt->d[2].u3 : st->xac;
                    // TODO map args to regs
                    update_lat(st, ovt, o);
                    break;
                case GEN_CLS(V):
                    st->vc = ovt->d[2].u3 > st->vc ? ovt->d[2].u3 : st->vc;
                    break;
                case GEN_CLS(T):
                    update_lat(st, ovt, o);
                    break;
            }
        }
        h = h->d[2].p;
    }
    return GEN_STAT(OK);
}

void gen_st_f(gen_st *st) {
    tbl_f(st->atm);
    tbl_f(st->lat);
    vr_f(st->rstk);
    vr_f(st->xstk);
    st->sf(st);
}

void gen_op_p(const tbl *ot, bool ci, size_t idnt) {
    te *h = ot->i->h;
    while (h) {
        te *o = h->d[0].p;
        if (!ci) {
            printf("(%s", gen_op_str(o->d[0].u6));
            gen_op_p(o->d[2].p, true, idnt + 1);
            printf(")\n");
        } else {
            putchar('\n');
            for (size_t i = 0; i < idnt; i++) putchar(' ');
            printf("(%s %s)", gen_cls_str(o->d[1].u3), x64_type_str(o->d[2].u3));
            gen_op_p(o->d[4].p, true, idnt + 1);
            if (h->d[2].p) putchar('\n');
        }
        h = h->d[2].p;
    }
}

void gen_p(const gen *g, const uint8_t *m) {
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        printf("(%s", gen_op_str(o->d[0].u6));
        for (size_t i = 1; i < 4; i++) {
            te *ovt = o->d[i].p;
            if (!ovt) {
                printf(" (N)");
                continue;
            }
            printf(" (%s ", gen_cls_str(ovt->d[0].u3));
            switch (ovt->d[0].u3) {
                case GEN_CLS(A):
                case GEN_CLS(V):
                case GEN_CLS(T):
                    printf("%s ", x64_type_str(ovt->d[1].u3));
                    break;
                default:
                    printf("N ");
                    break;
            }
            printf("%lu)", ovt->d[2].u6);
        }
        printf(")\n");
        // TODO as print
        h = h->d[2].p;
    }
}

static gen_stat enter_fn(alfn *al, frfn *af, gen *g, void *s, te *ci, as *a) {
    bool set = true;
    if (a->code->h) {
        set = false;
        ci->d[5] = P(a->code->h);
    }
    as_a(a, AS_X64(PUSH), as_arg(al, af, ARG_ID(R), U3(R(BP))), NULL, NULL, NULL);
    if (set) ci->d[5] = P(a->code->t);
    as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(BP))), as_arg(al, af, ARG_ID(R), U3(R(SP))), NULL, NULL);
    gen_st *st = (gen_st*) s;
    if (st->vc > 0) {
        size_t stks = st->vc * sizeof(void*);
        if (stks > UINT8_MAX) return GEN_STAT(INV);
        // TODO allocate stk
    }
    if (st->rac >= 3) as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(10))), as_arg(al, af, ARG_ID(R), U3(R(DX))), NULL, NULL);
    if (st->xac >= 4) as_a(a, AS_X64(MOV), as_arg(al, af, ARG_ID(R), U3(R(11))), as_arg(al, af, ARG_ID(R), U3(R(CX))), NULL, NULL);
    ci->d[6] = P(a->code->t);
    return GEN_STAT(OK);
}

gen *gen_b(gen *g) {
    GEN_OP_A0(g, GEN_OP(ENTER), &enter_fn);
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), GEN_CLS(A), U3(X64_TYPE(U6)), NULL);
    GEN_OP_A1(g, GEN_OP(LEAVE), GEN_CLS(A), U3(X64_TYPE(U6)), NULL);
    return g;
}
