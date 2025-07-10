
#include "gen_x64.h"

const char *x64_type_str(x64_type xt) {
    switch (xt) {
        case X64_TYPE(N): return "N";
        case X64_TYPE(S): return "S";
        case X64_TYPE(M): return "M";
        case X64_TYPE(MU3): return "MU3";
        case X64_TYPE(MU4): return "MU4";
        case X64_TYPE(MU5): return "MU5";
        case X64_TYPE(MU6): return "MU6";
        case X64_TYPE(MI3): return "MI3";
        case X64_TYPE(MI4): return "MI4";
        case X64_TYPE(MI5): return "MI5";
        case X64_TYPE(MI6): return "MI6";
        case X64_TYPE(MF5): return "MF5";
        case X64_TYPE(MF6): return "MF6";
        case X64_TYPE(MM): return "MM";
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

bool x64_type_is_ref(x64_type xt) {
    if (xt >= X64_TYPE(MU3) && xt <= X64_TYPE(MM)) return true;
    return false;
}

x64_type x64_type_to_ref(x64_type xt) {
    if (x64_type_is_ref(xt)) return xt;
    switch (xt) {
        case X64_TYPE(M): return X64_TYPE(MM);
        case X64_TYPE(U3): return X64_TYPE(MU3);
        case X64_TYPE(U4): return X64_TYPE(MU4);
        case X64_TYPE(U5): return X64_TYPE(MU5);
        case X64_TYPE(U6): return X64_TYPE(MU6);
        case X64_TYPE(I3): return X64_TYPE(MI3);
        case X64_TYPE(I4): return X64_TYPE(MI4);
        case X64_TYPE(I5): return X64_TYPE(MI5);
        case X64_TYPE(I6): return X64_TYPE(MI6);
        case X64_TYPE(F5): return X64_TYPE(MF5);
        case X64_TYPE(F6): return X64_TYPE(MF6);
        default:
            break;
    }
    return X64_TYPE(N);
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

void ovt_p(const te *ovt) {
    if (!ovt) {
        printf("(N)");
        return;
    }
    vr *m;
    printf("(%s ", gen_cls_str(gen_var_g_c(ovt)));
    switch (gen_var_g_c(ovt)) {
        case GEN_CLS(M):
            putchar('[');
            m = ovt->d[1].p;
            if (m) for (size_t i = 0; i < m->l; i++) ovt_p(m->d[i].p);
            printf("])");
            break;
        case GEN_CLS(W):
            putchar('[');
            m = ((te*) ovt->d[1].p)->d[0].p;
            if (m) for (size_t i = 0; i < m->l; i++) ovt_p(m->d[i].p);
            putchar(']');
            putchar('[');
            m = ((te*) ovt->d[1].p)->d[1].p;
            if (m) for (size_t i = 0; i < m->l; i++) ovt_p(m->d[i].p);
            printf("])");
            break;
        case GEN_CLS(I):
            printf("%s ", x64_type_str(gen_var_g_t(ovt)));
            m = ovt->d[1].p;
            ovt_p(m->d[0].p);
            putchar('[');
            for (size_t i = 1; i < m->l; i++) ovt_p(m->d[i].p);
            printf("])");
            break;
        case GEN_CLS(A):
        case GEN_CLS(V):
        case GEN_CLS(S):
        case GEN_CLS(T):
        case GEN_CLS(L):
            printf("%s %lu)", x64_type_str(gen_var_g_t(ovt)), ovt->d[1].u6);
            break;
        case GEN_CLS(D):
            printf("%s ", x64_type_str(gen_var_g_t(ovt)));
            switch (gen_var_g_t(ovt)) {
                case X64_TYPE(S):
                    printf("\"%s\")", (char*) ((mc*) ovt->d[1].p)->d);
                    break;
                case X64_TYPE(M):
                    printf("%p)", ovt->d[1].p);
                    break;
                case X64_TYPE(I6):
                    printf("%ld)", ovt->d[1].u6);
                    break;
                case X64_TYPE(F5):
                case X64_TYPE(F6):
                    printf("%lf)", ovt->d[1].f6);
                    break;
                default:
                    printf("%lu)", ovt->d[1].u6);
                    break;
            }
            break;
        default:
            printf("N ");
            break;
    }
}

static void gen_ci_p(const te *ci) {
    printf("(%s", gen_op_str(ci->d[0].u6));
    for (size_t i = 1; i < 4; i++) {
        te *ovt = ci->d[i].p;
        if (!ovt) {
            printf(" (N)");
            continue;
        }
        putchar(' ');
        ovt_p(ovt);
    }
    putchar(')');
}

void gen_p(const gen *g, const uint8_t *m) {
    if (!g) {
        printf("(N)\n");
        return;
    }
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        gen_ci_p(o);
        putchar('\n');
        te *cli = o->d[5].p;
        if (cli) {
            while (cli != o->d[6].p) {
                as_code_i_p(cli->d[0].p, m);
                cli = cli->d[2].p;
            }
            if (o->d[6].p) as_code_i_p(((te*) o->d[6].p)->d[0].p, m);
        }
        h = h->d[2].p;
    }
}

static void gen_mi_f(void *p) {
    te *t = p;
    vr_f(t->d[1].p);
    t->af->f(t);
}

static vr *mi(gen *g, size_t n, va_list args) {
    if (!n) return NULL;
    vr *v = vr_i(n, g->va, (void*) te_f);
    while (n > 0) {
        vr_ab(&v, P(va_arg(args, te*)));
        n--;
    }
    return v;
}

te *gen_call_v(gen *g, vr *v) {
    return gen_var_i(g, gen_mi_f, GEN_CLS(M), X64_TYPE(N), P(v));
}

te *gen_call_m(gen *g, size_t n, ...) {
    va_list args;
    va_start(args, n);
    vr *v = mi(g, n, args);
    va_end(args);
    return gen_call_v(g, v);
}

static void gen_call_w_vr_f(void *p) {
    te *t = p;
    vr_f(t->d[0].p);
    vr_f(t->d[1].p);
    t->af->f(t);
}

static void gen_call_w_f(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    t->af->f(t);
}

vr *gen_vr(gen *g, size_t n, ...) {
    va_list args;
    va_start(args, n);
    vr *v = mi(g, n, args);
    va_end(args);
    return v;
}

te *gen_call_w(gen *g, vr *s, vr *r) {
    te *t = te_i(2, g->ta, gen_call_w_vr_f);
    t->d[0] = P(s);
    t->d[1] = P(r);
    return gen_var_i(g, gen_call_w_f, GEN_CLS(W), X64_TYPE(N), P(t));
}

te *gen_idx_v(gen *g, x64_type t, vr *v) {
    return gen_var_i(g, gen_mi_f, GEN_CLS(I), t, P(v));
}

te *gen_idx_m(gen *g, x64_type t, size_t n, ...) {
    va_list args;
    va_start(args, n);
    vr *v = mi(g, n, args);
    va_end(args);
    return gen_idx_v(g, t, v);
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

static void gen_data_s_f(void *p) {
    te *t = p;
    mc_f(t->d[1].p);
    t->af->f(t);
}

te *gen_data(gen *g, x64_type t, un d) {
    void *fn = NULL;
    if (t == X64_TYPE(S)) {
        mc_c(d.p);
        fn = gen_data_s_f;
    }
    return gen_var_i(g, fn, GEN_CLS(D), t, d);
}

te *gen_char(gen *g, const char *s) {
    return gen_var_i(g, gen_data_s_f, GEN_CLS(D), X64_TYPE(S), P(mc_i_cstr(s, &al_mc)));
}

te *gen_stkv(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(V), t, U6(id));
}

te *gen_stka(gen *g, x64_type t, size_t id) {
    return gen_var_i(g, NULL, GEN_CLS(S), t, U6(id));
}

static bool gen_w_eq(const te *l, const te *r) {
    return vr_eq(l->d[0].p, r->d[0].p, gen_itm_eq) && vr_eq(l->d[1].p, r->d[1].p, gen_itm_eq);
}

bool gen_itm_eq(un x, un y) {
    te *l = x.p;
    te *r = y.p;
    if (!l && !r) return true;
    if ((l && !r) || (!l && r) || l->d[0].u6 != r->d[0].u6) return false;
    if (gen_var_g_c(l) == GEN_CLS(M) || gen_var_g_c(l) == GEN_CLS(I)) return vr_eq(l->d[1].p, r->d[1].p, gen_itm_eq);
    if (gen_var_g_c(l) == GEN_CLS(W)) return gen_w_eq(l->d[1].p, r->d[1].p);
    if (gen_var_g_t(l) == X64_TYPE(S)) return mc_eq(l->d[1].p, r->d[1].p);
    return l->d[1].u6 == r->d[1].u6;
}

static bool gen_lst_eq(un x, un y) {
    te *a = x.p;
    te *b = y.p;
    if (!a || !b) return false;
    if (a->d[0].u6 != b->d[0].u6) return false;
    for (size_t i = 1; i < 4; i++) {
        te *l = a->d[i].p;
        te *r = b->d[i].p;
        if (!gen_itm_eq(P(l), P(r))) return false;
    }
    return true;
}

bool gen_code_eq(const gen *a, const gen *b) {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return lst_eq(a->code, b->code, gen_lst_eq);
}

gen_st *gen_st_i(const alfr *af, const alfr *ta, tbl *atm, tbl *lat, vr *rstk, vr *xstk) {
    gen_st *st = af->a(sizeof(gen_st));
    st->rac = st->xac = st->rvc = st->xvc = st->rsc = 0;
    st->r = 1;
    st->af = af;
    st->ta = ta;
    st->atm = atm;
    st->lat = lat;
    st->rstk = rstk;
    st->xstk = xstk;
    return st;
}

gen_st *gen_st_i_gen_st(const gen_st *st) {
    return gen_st_i(st->af, st->ta, tbl_i_tbl(st->atm), tbl_i_tbl(st->lat), vr_i_vr(st->rstk), vr_i_vr(st->xstk));
}

void gen_st_p(const gen_st *st) {
    printf("rvc: %u, xvc: %u, rac: %u, xac: %u, rsc: %u\n", st->rvc, st->xvc, st->rac, st->xac, st->rsc);
    printf("atm_l: %lu, lat_l: %lu\nR:", st->atm->i->l, st->lat->i->l);
    for (size_t i = 0; i < st->rstk->l; i++) printf("%s ", reg_str(st->rstk->d[i].u3));
    printf("\nX:");
    for (size_t i = 0; i < st->xstk->l; i++) printf("%s ", reg_str(st->xstk->d[i].u3));
    putchar('\n');
}

void gen_atm_p(const gen_st *st) {
    te *h = st->atm->i->h, *n;
    while (h) {
        n = h->d[0].p;
        ovt_p(n->d[1].p);
        printf(", %s\n", reg_str(n->d[2].u3));
        h = h->d[2].p;
    }
}

static void gen_st_latf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    te_f(t->d[2].p);
    t->af->f(t);
}

static void update_lat(gen_st *st, te *ovt, te *o) {
    un hsh = gen_var_hsh(ovt);
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

static void gen_st_atmf(void *p) {
    te *t = p;
    te_f(t->d[1].p);
    t->af->f(t);
}

tbl_stat gen_add_reg(gen_st *st, te *ovt, reg r) {
    te *kv = te_i(3, st->ta, gen_st_atmf);
    kv->d[0] = gen_var_hsh(ovt);
    kv->d[1] = P(te_c(ovt));
    kv->d[2] = U3(r);
    return tbl_a(st->atm, kv);
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
    } else (*kv)->d[2] = U3(stk->d[stk->l - 1 - ovt->d[1].u3].u3);
    tbl_a(st->atm, *kv);
    return GEN_STAT(OK);
}

// check that the types do not change
static gen_stat reg_map(gen_st *st, te *ovt) {
    un hsh = gen_var_hsh(ovt);
    te *kv;
    if (tbl_g_i(st->atm, hsh, &kv) == TBL_STAT(OK)) {
        if (gen_var_g_t(ovt) != gen_var_g_t(kv->d[1].p)) return GEN_STAT(INV);
    } else return set_reg(st, hsh, ovt, &kv, false);
    return GEN_STAT(OK);
}

static gen_stat gen_st_p1_ovt(gen_st *st, te *ovt, te* o) {
    gen_stat stat;
    vr *m;
    switch (gen_var_g_c(ovt)) {
        case GEN_CLS(M):
        case GEN_CLS(I):
            m = ovt->d[1].p;
            if (m) for (size_t i = 0; i < m->l; i++) if ((stat = gen_st_p1_ovt(st, m->d[i].p, o)) != GEN_STAT(OK)) return stat;
            break;
        case GEN_CLS(W):
            m = ((te*) ovt->d[1].p)->d[0].p;
            if (m) for (size_t i = 0; i < m->l; i++) if ((stat = gen_st_p1_ovt(st, m->d[i].p, o)) != GEN_STAT(OK)) return stat;
            m = ((te*) ovt->d[1].p)->d[1].p;
            if (m) for (size_t i = 0; i < m->l; i++) if ((stat = gen_st_p1_ovt(st, m->d[i].p, o)) != GEN_STAT(OK)) return stat;
            break;
        case GEN_CLS(A):
            if (gen_var_g_t(ovt) >= X64_TYPE(M) && gen_var_g_t(ovt) <= X64_TYPE(I6)) st->rac = ovt->d[1].u3 + 1 > st->rac ? ovt->d[1].u3 + 1 : st->rac;
            else st->xac = ovt->d[1].u3 + 1 > st->xac ? ovt->d[1].u3 + 1 : st->xac;
            if ((stat = reg_map(st, ovt)) != GEN_STAT(OK)) return stat;
            update_lat(st, ovt, o);
            break;
        case GEN_CLS(V):
            switch (gen_var_g_t(ovt)) {
                case X64_TYPE(F5):
                case X64_TYPE(F6):
                    st->xvc = ovt->d[1].u3 + 1 > st->xvc ? ovt->d[1].u3 + 1 : st->xvc;
                    break;
                default:
                    st->rvc = ovt->d[1].u3 + 1 > st->rvc ? ovt->d[1].u3 + 1 : st->rvc;
            }
            break;
        case GEN_CLS(S):
             switch (gen_var_g_t(ovt)) {
                case X64_TYPE(F5):
                case X64_TYPE(F6):
                    return GEN_STAT(INV);
                default:
                    st->rsc = ovt->d[1].u3 + 1 > st->rsc ? ovt->d[1].u3 + 1 : st->rsc;
            }
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
    static reg r[] = {R(BX), R(13), R(12), R(14), R(15), R(9), R(8), R(11), R(10), R(SI), R(DI)};
    for (size_t i = 0; i < 11; i++) vr_ab(&st->rstk, U3(r[i]));
    static reg x[] = {XMM(15), XMM(14), XMM(13), XMM(12), XMM(11), XMM(10), XMM(9), XMM(8), XMM(6), XMM(5), XMM(4), XMM(3), XMM(2), XMM(1), XMM(7)};
    for (size_t i = 0; i < 15; i++) vr_ab(&st->xstk, U3(x[i]));
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        if (o->d[0].u6 == GEN_OP(NOP)) { // only used to get ovt
            h = h->d[2].p;
            continue;
        }
        for (size_t i = 1; i < 4; i++) {
            if (!o->d[i].p) break;
            else if ((stat = gen_st_p1_ovt(st, o->d[i].p, o)) != GEN_STAT(OK)) return stat;
        }
        h = h->d[2].p;
    }
    if (st->rac > 5 || st->xac > 5) return GEN_STAT(INV); // TODO pass more args via stk
    un rx;
    for (uint8_t i = 0; i < st->rac; i++) vr_sb(st->rstk, &rx);
    for (uint8_t i = 0; i < st->xac; i++) vr_sb(st->xstk, &rx);
    return stat;
}

static void set_code_s(te *ci, as *a) {
    if (!ci->d[5].p && a->code->t) ci->d[5] = P(te_c(a->code->t));
}

gen_stat st_stkv_idx(const gen_st *st, x64_type t, uint8_t v, int32_t *idx) {
    *idx = sizeof(void*);
    switch (t) {
        case X64_TYPE(F5):
        case X64_TYPE(F6):
            if (v >= st->xvc) return GEN_STAT(INV);
            *idx += sizeof(void*) * v;
            break;
        default:
            if (v >= st->rvc) return GEN_STAT(INV);
            *idx += st->xvc * sizeof(void*);
            *idx += sizeof(void*) * v;
            break;
    }
    *idx = -*idx;
    return GEN_STAT(OK);
}

gen_stat st_stka_idx(x64_type t, uint8_t v, int32_t *idx) {
    switch (t) {
        case X64_TYPE(F5):
        case X64_TYPE(F6):
            return GEN_STAT(INV);
        default:
            break;
    }
    *idx = sizeof(void*) + sizeof(void*) * (v + 1);
    return GEN_STAT(OK);
}

te *bd_arg(as *a, int32_t dsp) {
    te *arg;
    if (dsp >= INT8_MIN && dsp <= INT8_MAX) arg = as_arg_i(a, ARG_ID(B), I3(dsp));
    else arg = as_arg_i(a, ARG_ID(DW), I5(dsp));
    return arg;
}

as_stat gen_as_rmbdr(as *a, as_inst i, reg d, int32_t dsp, reg s, te *ci) {
    return gen_as(a, i, as_arg_i(a, ARG_ID(RM), U3(d)), bd_arg(a, dsp), as_arg_i(a, s >= XMM(0) ? ARG_ID(X) : ARG_ID(R), U3(s)), NULL, ci);
}

as_stat gen_as_rrmbd(as *a, as_inst i, reg d, reg s, int32_t dsp, te *ci) {
    return gen_as(a, i, as_arg_i(a, d >= XMM(0) ? ARG_ID(X) : ARG_ID(R), U3(d)), as_arg_i(a, ARG_ID(RM), U3(s)), bd_arg(a, dsp), NULL, ci);
}

gen_stat get_reg(gen_st *st, te *ovt, te **kv) {
    if (!ovt) return GEN_STAT(INV);
    un hsh = gen_var_hsh(ovt);
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
    te *kv = NULL;
    if (tbl_g_i(st->lat, atm_kv->d[0], &kv) == TBL_STAT(NF)) return;
    if (kv->d[2].p != ci) return;
    reg r = atm_kv->d[2].u3;
    if (r < XMM(0)) vr_ab(&st->rstk, U3(r));
    else vr_ab(&st->xstk, U3(r));
    tbl_s(st->atm, atm_kv->d[0], &kv);
    /*
    for (uint8_t x = 0; x < st->rstk->l - 1; x++) {
        for (uint8_t y = x + 1; y < st->rstk->l; y++) {
            if (st->rstk->d[x].u3 == st->rstk->d[y].u3) {
                ovt_p(kv->d[1].p);
                HERE("DUP IN RSTK");
                gen_st_p(st);
                exit(1);
            }
        }
    }
    */
    te_f(kv);
}

void drop_atm_kv_n(gen_st *st, te **atm_kv, const te *ci, size_t n) {
    for (size_t i = 0; i < n; i++) drop_atm_kv(st, atm_kv[i], ci);
}

gen_stat stk_va(gen_st *st, te *c, int32_t *v) {
    if (gen_var_g_c(c) == GEN_CLS(V)) return st_stkv_idx(st, gen_var_g_t(c), c->d[1].u3, v);
    return st_stka_idx(gen_var_g_t(c), c->d[1].u3, v);
}

gen_stat stk_g_idx2(gen_st *st, te *restrict c0, te *restrict c1, int32_t *restrict v0, int32_t *restrict v1) {
    if (stk_va(st, c0, v0) != GEN_STAT(OK) || stk_va(st, c1, v1) != GEN_STAT(OK)) return GEN_STAT(INV);
    return GEN_STAT(OK);
}

gen_stat stk_g_idx3(gen_st *st, te *restrict c0, te *restrict c1, te *restrict c2, int32_t *restrict v0, int32_t *restrict v1, int32_t *restrict v2) {
    if (stk_va(st, c0, v0) != GEN_STAT(OK)) return GEN_STAT(INV);
    return stk_g_idx2(st, c1, c2, v1, v2);
}

void gen_st_f(gen_st *st) {
    if (!st || --st->r > 0) return;
    tbl_f(st->atm);
    tbl_f(st->lat);
    vr_f(st->rstk);
    vr_f(st->xstk);
    st->af->f(st);
}

as_stat gen_as(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4, te *restrict ci) {
    as_stat stat = AS_STAT(OK);
    stat = as_a(a, op_id, arg1, arg2, arg3, arg4);
    set_code_s(ci, a);
    return stat;
}

static void gen_err_p(void *d) {
    gen_ci_p(d);
}

gen_stat gen_err(const gen *g, te *ci, err **e, const char *m) {
    *e = err_i(g->ea, gen_err_p, (void*) te_f, te_c(ci), m);
    return GEN_STAT(INV);
}

static void jmp_2_leave(gen *g, te *restrict hlve, te *restrict lbl) {
    te *h = g->code->h, *c, *lve = hlve->d[0].p;
    while (h != hlve) {
        c = h->d[0].p;
        if (c->d[0].u6 == GEN_OP(JMP) && ((te*) c->d[1].p)->d[1].u6 == ((te*) lbl->d[1].p)->d[1].u6) {
            c->d[0] = lve->d[0];
            te_f(c->d[1].p);
            c->d[1] = P(te_c(lve->d[1].p));
            c->d[4] = lve->d[4];
        }
        h = h->d[2].p;
    }
}

static void update_tmp_ci(te *ovt, uint32_t to, uint32_t from) {
    vr *v;
    switch (gen_var_g_c(ovt)) {
        case GEN_CLS(T):
            if (ovt->d[1].u5 == from) ovt->d[1] = U5(to);
            break;
        case GEN_CLS(M):
        case GEN_CLS(I):
            v = ovt->d[1].p;
            for (size_t i = 0; i < v->l; i++) update_tmp_ci(v->d[i].p, to, from);
            break;
        case GEN_CLS(W):
            STOP("TODO set_2_nop CLS W");
            break;
        default:
            break;
    }
}

static void set_2_nop(gen *g, te *h) {
    te *ci = h->d[0].p, *tmp = h;
    uint32_t to = ((te*) ci->d[1].p)->d[1].u5, from = ((te*) ci->d[2].p)->d[1].u5;
    h = h->d[1].p;
    te_f(tmp->d[0].p);
    lst_li_d(g->code, tmp);
    while (h) {
        ci = h->d[0].p;
        for (size_t i = 1; i <= 3; i++) {
            if (!ci->d[i].p) break;
            update_tmp_ci(ci->d[i].p, to, from);
        }
        h = h->d[1].p;
    }
}

void gen_x64_opt(gen *g, gen_st *st) {
    (void) st; // TODO stack vars to available regs
    te *h = g->code->h, *c;
    while (h) {
        c = h->d[0].p;
        if (c->d[0].u6 == GEN_OP(LEAVE) && h->d[1].p) {
            c = ((te*) h->d[1].p)->d[0].p;
            if (c && c->d[0].u6 == GEN_OP(LBL)) jmp_2_leave(g, h, c);
        }
        if (c->d[0].u6 == GEN_OP(SET) && gen_var_g_c(c->d[1].p) == GEN_CLS(T) && gen_var_g_c(c->d[2].p) == GEN_CLS(T) && h->d[1].p) {
            c = ((te*) h->d[1].p)->d[0].p;
            if (c && c->d[0].u6 == GEN_OP(NOP)) {
                c = h->d[2].p;
                set_2_nop(g, h);
                h = c;
                continue;
            }
        }
        // TODO opt setting stk cls m vars to 0
        h = h->d[2].p;
    }
}

static gen_stat lbl_fn(gen *g, void *s, te *ci, as *a, err **e)  {
    (void) g;
    (void) s;
    (void) e;
    te *lbl = ci->d[1].p;
    as_lbl_a(a, lbl->d[1].u6);
    set_code_s(ci, a);
    return GEN_STAT(OK);
}

static gen_stat jmp_fn(gen *g, void *s, te *ci, as *a, err **e) {
    (void) s;
    te *lbl = ci->d[1].p;
    if (gen_as(a, AS_X64(JMP), as_arg_i(a, ARG_ID(L), lbl->d[1]), NULL, NULL, NULL, ci)) return gen_err(g, ci, e, __FUNCTION__);
    return GEN_STAT(OK);
}

static gen_stat nop_fn(gen *g, void *s, te *ci, as *a, err **e) {
    (void) g;
    (void) s;
    (void) ci;
    (void) a;
    (void) e;
    return GEN_STAT(OK);
}

// not meant to be used outside
void gen_enter_leave(gen *g);
void gen_set(gen *g);
void gen_arith(gen *g);
void gen_cond(gen *g);
void gen_call(gen *g);
void gen_ref(gen *g);

gen *gen_b(gen *g) {
    GEN_OP_A1(g, GEN_OP(LBL), GEN_CLS(L), X64_TYPE(N), lbl_fn);
    GEN_OP_A1(g, GEN_OP(NOP), GEN_CLS(T), X64_TYPE(M), nop_fn);
    GEN_OP_A1(g, GEN_OP(NOP), GEN_CLS(T), X64_TYPE(MM), nop_fn);
    GEN_OP_A1(g, GEN_OP(JMP), GEN_CLS(L), X64_TYPE(N), jmp_fn);
    gen_enter_leave(g);
    gen_set(g);
    gen_arith(g);
    gen_cond(g);
    gen_call(g);
    gen_ref(g);
    return g;
}
