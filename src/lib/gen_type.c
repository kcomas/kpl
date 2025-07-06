
#include "gen_type.h"


static gen_stat gen_type_err(gen *g, te *t, err **e, const char *m) {
    *e = err_i(g->ea, (void*) type_p, (void*) te_f, te_c(t), m);
    return GEN_STAT(INV);
}

void* type_ref_g_des(type t) {
    switch (t) {
        case TYPE(SG):
            return mc_f;
        case TYPE(ER):
            return err_f;
        case TYPE(VR):
            return vr_f;
        case TYPE(TE):
        case TYPE(ST):
        case TYPE(UN):
            return te_f;
        default:
            break;
    }
    return NULL;
}

static gen_stat v_des(gen *bg, te *t, void **fn, err **e) {
    te *st = t->d[2].p;
    if (!st || !type_is_ref(st->d[1].u4)) return gen_type_err(bg, t, e, "gen type inv vr ref");
    *fn = type_ref_g_des(st->d[1].u4);
    if (!fn) return gen_type_err(bg, t, e, "gen unable to get type for vr des");
    return GEN_STAT(OK);
}

gen_stat gen_type_aff(gen *g, te *t, err **e, const char *pf) {
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 0), gen_idx_m(g, X64_TYPE(M), 2, gen_arg(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U3), U3(offsetof(te, af)))), NULL) != GEN_STAT(OK)) return gen_type_err(g, t, e, pf);
    if (gen_a(g, GEN_OP(SET), gen_tmp(g, X64_TYPE(M), 0), gen_idx_m(g, X64_TYPE(M), 2, gen_tmp(g, X64_TYPE(M), 0), gen_data(g, X64_TYPE(U3), U3(offsetof(alfr, f)))), NULL) != GEN_STAT(OK)) return gen_type_err(g, t, e, pf);
    if (gen_a(g, GEN_OP(CALLNPR), gen_call_m(g, 1, gen_arg(g, X64_TYPE(M), 0)), gen_tmp(g, X64_TYPE(M), 0), NULL) != GEN_STAT(OK)) return gen_type_err(g, t, e, pf);
    if (gen_a(g, GEN_OP(L), NULL, NULL, NULL) != GEN_STAT(OK)) return gen_type_err(g, t, e, pf);
    return GEN_STAT(OK);
}

static gen_stat st_des(gen *bg, te *t, gen **g, err **e) {
    void *fn = NULL;
    *g = gen_i_gen(bg);
    if (gen_a(*g, GEN_OP(E), NULL, NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
    size_t i = 0;
    te *h = ((tbl*) t->d[2].p)->i->h, *n;
    while (h) {
        n = h->d[0].p;
        if (n->d[2].p && type_is_ref(((te*) n->d[2].p)->d[1].u4)) {
            fn = type_ref_g_des(((te*) n->d[2].p)->d[1].u4);
            if (!fn) return gen_type_err(bg, t, e, "gen unable to get fn for te des");
            int32_t off = offsetof(te, d) + sizeof(void*) * i;
            if (gen_a(*g, GEN_OP(CALL), gen_call_m(*g, 1, gen_idx_m(*g, X64_TYPE(M), 2, gen_arg(*g, X64_TYPE(M), 0), gen_data(*g, off <= INT8_MAX ? X64_TYPE(U3) : X64_TYPE(U5), U5(off)))), gen_data(*g, X64_TYPE(M), P(fn)), NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
        }
        i++;
        h = h->d[2].p;
    }
    return gen_type_aff(*g, t, e, __FUNCTION__);
}

static gen_stat un_des(gen *bg, te *t, gen **g, err **e) {
    int32_t el = 0, tl = 1;
    size_t ui = 0;
    void *fn = NULL;
    *g = gen_i_gen(bg);
    if (gen_a(*g, GEN_OP(E), NULL, NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
    te *h = ((tbl*) t->d[2].p)->i->h, *n;
    while (h) {
        n = h->d[0].p;
        if (h->d[2].p) {
            if (gen_a(*g, GEN_OP(NE), gen_idx_m(*g, X64_TYPE(U6), 2, gen_arg(*g, X64_TYPE(M), 0), gen_data(*g, X64_TYPE(U3), U3(offsetof(te, d)))), gen_data(*g, X64_TYPE(U6), U6(ui++)), gen_lbl(*g, tl)) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
        }
        if (n->d[2].p && type_is_ref(((te*) n->d[2].p)->d[1].u4)) {
            fn = type_ref_g_des(((te*) n->d[2].p)->d[1].u4);
            if (!fn) return gen_type_err(bg, t, e, "gen unable to get fn for te des");
            if (gen_a(*g, GEN_OP(CALL), gen_call_m(*g, 1, gen_idx_m(*g, X64_TYPE(M), 2, gen_arg(*g, X64_TYPE(M), 0), gen_data(*g, X64_TYPE(U3), U3(offsetof(te, d) + sizeof(void*))))), gen_data(*g, X64_TYPE(M), P(fn)), NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
        }
        if (h->d[2].p) {
            if (gen_a(*g, GEN_OP(JMP), gen_lbl(*g, el), NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
            if (gen_a(*g, GEN_OP(LBL), gen_lbl(*g, tl++), NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
        }
        h = h->d[2].p;
    }
    if (gen_a(*g, GEN_OP(LBL), gen_lbl(*g, el), NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
    return gen_type_aff(*g, t, e, __FUNCTION__);
}

static gen_stat te_des(gen *bg, te *t, gen **g, err **e) {
    void *fn = NULL;
    *g = gen_i_gen(bg);
    if (gen_a(*g, GEN_OP(E), NULL, NULL, NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
    for (size_t i = 2; i < t->l; i++) {
        if (!t->d[i].p || !type_is_ref(((te*) t->d[i].p)->d[1].u4)) continue;
        fn = type_ref_g_des(((te*) t->d[i].p)->d[1].u4);
        if (!fn) return gen_type_err(bg, t, e, "gen unable to get fn for te des");
        int32_t off = offsetof(te, d) + sizeof(void*) * (i - 2);
        if (gen_a(*g, GEN_OP(CALL), gen_call_m(*g, 1, gen_idx_m(*g, X64_TYPE(M), 2, gen_arg(*g, X64_TYPE(M), 0), gen_data(*g, off <= INT8_MAX ? X64_TYPE(U3) : X64_TYPE(U5), U5(off)))), gen_data(*g, X64_TYPE(M), P(fn)), NULL) != GEN_STAT(OK)) return gen_type_err(bg, t, e, __FUNCTION__);
    }
    return gen_type_aff(*g, t, e, __FUNCTION__);
}

gen_stat gen_type_des(gen *bg, te *t, gen **g, void **fn, err **e) {
    if (!t) return gen_type_err(bg, t, e, "gen type inv for des");
    switch (type_g_c(t->d[1].u4)) {
        case TYPE_CLS(V):
            return v_des(bg, t, fn, e);
        case TYPE_CLS(H):
            switch(t->d[1].u4) {
                case TYPE(ST):
                    return st_des(bg, t, g, e);
                case TYPE(UN):
                    return un_des(bg, t, g, e);
                default:
                    break;
            }
            break;
        case TYPE_CLS(C):
            switch (t->d[1].u4) {
                case TYPE(TE):
                    return te_des(bg, t, g, e);
                break;
            }
            break;
        default:
            break;
    }
    return gen_type_err(bg, t, e, "gen type inv cls for des");
}
