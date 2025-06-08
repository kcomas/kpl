
#include "gen_x64.h"

const char *gen_op_str(gen_op go) {
    static const char *gos[] = {
        "_START",
        "LBL",
        "ENTER",
        "LEAVE",
        "CALL",
        "CALLNPR",
        "NEG",
        "ADD",
        "SUB",
        "EQ",
        "NE",
        "GT",
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
            printf("(%s %s)", gen_cls_str(gen_var_g_c(o)), x64_type_str(gen_var_g_t(o)));
            gen_op_p(o->d[2].p, true, idnt + 1);
        }
        h = h->d[2].p;
    }
}

static void ovt_p(const te *ovt) {
    vr *m;
    printf("(%s ", gen_cls_str(gen_var_g_c(ovt)));
    switch (gen_var_g_c(ovt)) {
        case GEN_CLS(M):
            putchar('[');
            m = ovt->d[1].p;
            for (size_t i = 0; i < m->l; i++) ovt_p(m->d[i].p);
            printf("])");
            break;
        case GEN_CLS(A):
        case GEN_CLS(V):
        case GEN_CLS(T):
        case GEN_CLS(L):
        case GEN_CLS(D):
            printf("%s ", x64_type_str(gen_var_g_t(ovt)));
            break;
        default:
            printf("N ");
            break;
    }
    if (gen_var_g_c(ovt) == GEN_CLS(M)) return;
    switch (gen_var_g_t(ovt)) {
        case X64_TYPE(M):
            printf("%p)", ovt->d[1].p);
            break;
        default:
            printf("%lu)", ovt->d[1].u6);
            break;
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
            putchar(' ');
            ovt_p(ovt);
        }
        printf(")\n");
        te *cli = o->d[5].p;
        if (cli) {
            while (cli != o->d[6].p) {
                as_code_i_p(cli->d[0].p, m);
                cli = cli->d[2].p;
            }
            as_code_i_p(((te*) o->d[6].p)->d[0].p, m);
        }
        h = h->d[2].p;
    }
}

static void gen_call_m_f(void *p) {
    te *t = p;
    vr_f(t->d[1].p);
    t->af->f(t);
}

te *gen_call_m(gen *g, size_t n, ...) {
    vr *v = vr_i(n, g->af, (void*) te_f);
    va_list args;
    va_start(args, n);
    while (n > 0) {
        vr_ab(&v, P(va_arg(args, te*)));
        n--;
    }
    va_end(args);
    return gen_var_i(g, gen_call_m_f, GEN_CLS(M), X64_TYPE(N), P(v));
}

te *gen_lbl(gen *g, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(L), X64_TYPE(N), U6(id));
}

te *gen_arg(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(A), t, U6(id));
}

te *gen_tmp(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(T), t, U6(id));
}

te *gen_data(gen *g, x64_type t, un d) {
    return gen_var_i(g, NULL, GEN_CLS(D), t, d);
}

gen_st *gen_st_i(const alfr *af, const alfr *ta, tbl *atm, tbl *lat, vr *rstk, vr *xstk) {
    gen_st *st = af->a(sizeof(gen_st));
    st->vc = st->rac = st->xac = 0;
    st->r = 1;
    st->af = af;
    st->ta = ta;
    st->atm = atm;
    st->lat = lat;
    st->rstk = rstk;
    st->xstk = xstk;
    return st;
}

void gen_st_p(const gen_st *st) {
    printf("vc: %u, rac: %u, xac: %u\n", st->vc, st->rac, st->xac);
    printf("atm_l: %lu, lat_l: %lu\nR:", st->atm->i->l, st->lat->i->l);
    for (size_t i = 0; i < st->rstk->l; i++) printf("%s ", reg_str(st->rstk->d[i].u3));
    printf("\nX:");
    for (size_t i = 0; i < st->xstk->l; i++) printf("%s ", reg_str(st->xstk->d[i].u3));
    putchar('\n');
}

un ovt_hsh(const te *ovt) {
    return U6((ovt->d[1].u6 << 32) + ovt->d[0].u5);
}

static void gen_st_latf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    t->af->f(t);
}

static void update_lat(gen_st *st, te *ovt, te *o) {
    un hsh = ovt_hsh(ovt);
    te *kv;
    if (tbl_g_i(st->lat, hsh, &kv) == TBL_STAT(OK)) {
        te_f(kv->d[1].p);
        te_f(kv->d[2].p);
        kv->d[1] = P(te_c(ovt));
        kv->d[2] = P(te_c(o));
    } else {
        kv = te_i(3, st->ta, gen_st_latf);
        kv->d[0] = hsh;
        kv->d[1] = P(te_c(ovt));
        kv->d[2] = P(te_c(o));
        tbl_a(st->lat, kv);
    }
}

void gen_st_atmf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    t->af->f(t);
}

static gen_stat set_reg(gen_st *st, un hsh, te *ovt, te **kv, bool decs) {
    x64_type xt = gen_var_g_t(ovt);
    if (xt >= X64_TYPE(M) && xt <= X64_TYPE(I6) && st->rstk->l == 0) return GEN_STAT(INV);
    else if (st->xstk->l == 0) return GEN_STAT(INV);
    *kv = te_i(3, st->ta, gen_st_atmf);
    (*kv)->d[0] = hsh;
    (*kv)->d[1] = P(te_c(ovt));
    un d;
    vr *stk;
    if (xt < X64_TYPE(F5)) stk = st->rstk;
    else stk = st->xstk;
    if (decs) {
        vr_sb(stk, &d);
        (*kv)->d[2] = d;
    } else (*kv)->d[2] = U3(st->rstk->d[st->rstk->l - 1 - ovt->d[1].u3].u3);
    tbl_a(st->atm, *kv);
    return GEN_STAT(OK);
}

// check that the types do not change
static gen_stat reg_map(gen_st *st, te *ovt) {
    un hsh = ovt_hsh(ovt);
    te *kv;
    if (tbl_g_i(st->atm, hsh, &kv) == TBL_STAT(OK)) {
        if (gen_var_g_t(ovt) != gen_var_g_t(kv->d[1].p)) return GEN_STAT(INV);
    } else set_reg(st, hsh, ovt, &kv, false);
    return GEN_STAT(OK);
}

static gen_stat gen_st_p1_ovt(gen_st *st, te *ovt, te* o) {
    gen_stat stat;
    vr *m;
    switch (gen_var_g_c(ovt)) {
        case GEN_CLS(M):
            m = ovt->d[1].p;
            for (size_t i = 0; i < m->l; i++) if ((stat = gen_st_p1_ovt(st, m->d[i].p, o)) != GEN_STAT(OK)) return stat;
            break;
        case GEN_CLS(A):
            if (gen_var_g_t(ovt) >= X64_TYPE(M) && gen_var_g_t(ovt) <= X64_TYPE(I6)) st->rac = ovt->d[1].u3 + 1 > st->rac ? ovt->d[1].u3 + 1 : st->rac;
            else st->xac = ovt->d[1].u3 + 1 > st->xac ? ovt->d[1].u3 + 1 : st->xac;
            if ((stat = reg_map(st, ovt)) != GEN_STAT(OK)) return stat;
            update_lat(st, ovt, o);
            break;
        case GEN_CLS(V):
            st->vc = ovt->d[1].u3 + 1 > st->vc ? ovt->d[1].u3 + 1 : st->vc;
            break;
        case GEN_CLS(T):
            update_lat(st, ovt, o);
            break;
        default:
            break;
    }
    return GEN_STAT(OK);
}

gen_stat gen_st_p1(gen *g, gen_st *st) {
    gen_stat stat = GEN_STAT(OK);
    // swap R(DX) with R(10), R(CX) with R(11), XMM(0) with XMM(7)
    static reg r[] = {R(12),  R(13), R(14), R(15), R(BX), R(9), R(8), R(11), R(10), R(SI), R(DI)};
    for (size_t i = 0; i < 11; i++) vr_ab(&st->rstk, U3(r[i]));
    static reg x[] = {XMM(15), XMM(14), XMM(13), XMM(12), XMM(11), XMM(10), XMM(9), XMM(8), XMM(6), XMM(5), XMM(4), XMM(3), XMM(2), XMM(1), XMM(7)};
    for (size_t i = 0; i < 15; i++) vr_ab(&st->xstk, U3(x[i]));
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        for (size_t i = 1; i < 4; i++) {
            if (!o->d[i].p) break;
            else if ((stat = gen_st_p1_ovt(st, o->d[i].p, o)) != GEN_STAT(OK)) return stat;
        }
        h = h->d[2].p;
    }
    if (st->rac > 6 || st->xac > 6) return GEN_STAT(INV); // TODO pass more args via stk
    un rx;
    for (uint8_t i = 0; i < st->rac; i++) vr_sb(st->rstk, &rx);
    for (uint8_t i = 0; i < st->xac; i++) vr_sb(st->xstk, &rx);
    return stat;
}

void gen_st_f(gen_st *st) {
    tbl_f(st->atm);
    tbl_f(st->lat);
    vr_f(st->rstk);
    vr_f(st->xstk);
    st->af->f(st);
}

void set_code_s(te *ci, as *a) {
    if (!ci->d[5].p && a->code->t) ci->d[5] = P(te_c(a->code->t));
}

void set_code_e(te *ci, as *a) {
    ci->d[6] = P(te_c(a->code->t));
}

gen_stat get_reg(gen_st *st, te *ovt, te **kv) {
    if (!ovt) return GEN_STAT(INV);
    un hsh = ovt_hsh(ovt);
    if (tbl_g_i(st->atm, hsh, kv) == TBL_STAT(NF)) {
        if (gen_var_g_c(ovt) != GEN_CLS(T)) return GEN_STAT(INV);
        return set_reg(st, hsh, ovt, kv, true);
    }
    return GEN_STAT(OK);
}

gen_stat get_reg_n(gen_st *st, te *ci, te **kv, size_t n) {
    if (n == 0 || n > 3) return GEN_STAT(INV);
    gen_stat stat;
    for (size_t i = 1; i < n + 1; i++) if ((stat = get_reg(st, ci->d[i].p, &kv[i - 1])) != GEN_STAT(OK)) return stat;
    return GEN_STAT(OK);
}

void drop_atm_kv(gen_st *st, const te *atm_kv, const te *ci) {
    te *kv;
    if (tbl_g_i(st->lat, atm_kv->d[0], &kv) == TBL_STAT(NF)) return;
    if (kv->d[2].p != ci) return;
    reg r = atm_kv->d[2].u3;
    if (r < XMM(0)) vr_ab(&st->rstk, U3(r));
    else vr_ab(&st->xstk, U3(r));
    tbl_s(st->atm, atm_kv->d[0], &kv);
    te_f(kv);
}

void drop_atm_kv_n(gen_st *st, te **atm_kv, const te *ci, size_t n) {
    for (size_t i = 0; i < n; i++) drop_atm_kv(st, atm_kv[i], ci);
}

as_stat gen_as(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4, te *restrict ci) {
    as_stat stat = AS_STAT(OK);
    stat = as_a(a, op_id, arg1, arg2, arg3, arg4);
    set_code_s(ci, a);
    return stat;
}

gen_stat gen_err(gen_stat stat, te *ci, te **e) {
    *e = te_c(ci);
    return stat;
}

static gen_stat lbl_fn(gen *g, void *s, te *ci, as *a, te **e)  {
    (void) g;
    (void) s;
    (void) e;
    te *lbl = ci->d[1].p;
    as_lbl_a(a, lbl->d[1].u6);
    set_code_s(ci, a);
    set_code_e(ci, a);
    return GEN_STAT(OK);
}

// not meant to be used outside
void gen_enter_leave(gen *g);
void gen_arith(gen *g);
void gen_cond(gen *g);
void gen_call(gen *g);

gen *gen_b(gen *g) {
    GEN_OP_A1(g, GEN_OP(LBL), GEN_CLS(L), X64_TYPE(N), lbl_fn);
    gen_enter_leave(g);
    gen_arith(g);
    gen_cond(g);
    gen_call(g);
    return g;
}
