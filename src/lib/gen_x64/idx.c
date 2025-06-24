
#include "idx.h"

static gen_stat arg_id_bd_g(te *iv, arg_id *di) {
    switch (gen_var_g_t(iv)) {
        case X64_TYPE(U3):
            *di = ARG_ID(B);
            break;
        case X64_TYPE(U5):
            *di = ARG_ID(DW);
            break;
        default:
            return GEN_STAT(INV);
    }
    return GEN_STAT(OK);
}

gen_stat idx_to(const gen *g, void *s, te *restrict ci, as *a, err **e, as_inst ai, vr *i, te *restrict from, reg rtmp, arg_id atmp) {
    gen_stat stat;
    arg_id di;
    int32_t idx;
    te *kvib, *kvii, *tgt;
    as_inst tm = atmp == ARG_ID(R) ? AS_X64(MOV) : AS_X64(MOVSD);
    gen_cls x = GEN_CLS(N), y = GEN_CLS(N), z = GEN_CLS(N);
    if (i->l >= 2) {
        x = gen_var_g_c(i->d[0].p);
        y = gen_var_g_c(i->d[1].p);
    }
    if (i->l == 2) {
        if ((x == GEN_CLS(A) || x == GEN_CLS(T)) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D)) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (from->d[0].u6 == ARG_ID(QW)) {
                if (gen_as(a, ai, as_arg_i(a, atmp, U3(rtmp)), from, NULL, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
                if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), as_arg_i(a, atmp, U3(rtmp)), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            } else if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), from, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
        } else if (x == GEN_CLS(V) && y == GEN_CLS(D)) {
            tgt = i->d[0].p;
            if (st_stkv_idx(s, X64_TYPE(M), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
            if (gen_as_rrmbd(a, tm, rtmp, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), U3(rtmp)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), from, NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else return gen_err(g, ci, e, "nyi");
    } else if (i->l == 3) {
        z = gen_var_g_c(i->d[2].p);
        if ((x == GEN_CLS(A) || x == GEN_CLS(T)) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D) && (z == GEN_CLS(A) || z == GEN_CLS(T))) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if ((stat = get_reg(s, i->d[2].p, &kvii) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, ARG_ID(RS), as_x64_rs(kvii->d[2].u3, 8)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), from, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
            drop_atm_kv(s, kvii, ci);
        } else return gen_err(g, ci, e, "nyi");
    } else return gen_err(g, ci, e, "nyi");
    return GEN_STAT(OK);
}

gen_stat idx_from(const gen *g, void *s, te *restrict ci, as *a, err **e, as_inst ai, vr *i, te *restrict to, reg rtmp, arg_id atmp) {
    gen_stat stat;
    arg_id di;
    int32_t idx;
    te *kvib, *kvii, *tgt;
    as_inst tm = atmp == ARG_ID(R) ? AS_X64(MOV) : AS_X64(MOVSD);
    gen_cls x = GEN_CLS(N), y = GEN_CLS(N), z = GEN_CLS(N);
    if (i->l >= 2) {
        x = gen_var_g_c(i->d[0].p);
        y = gen_var_g_c(i->d[1].p);
    }
    if (i->l == 2) {
        if (x == GEN_CLS(V) && y == GEN_CLS(D)) {
            tgt = i->d[0].p;
            if (st_stkv_idx(s, X64_TYPE(M), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
            if (gen_as_rrmbd(a, tm, rtmp, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, to, as_arg_i(a, ARG_ID(RM), U3(rtmp)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
        } else if ((x == GEN_CLS(T) || x == GEN_CLS(A)) && y == GEN_CLS(D)) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, to, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), NULL, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
        } else return gen_err(g, ci, e, "nyi");
    } else if (i->l == 3) {
        z = gen_var_g_c(i->d[2].p);
        if ((x == GEN_CLS(A) || x == GEN_CLS(T)) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D) && (z == GEN_CLS(A) || z == GEN_CLS(T))) {
            if ((stat = get_reg(s, i->d[0].p, &kvib) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if ((stat = get_reg(s, i->d[2].p, &kvii) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, to, as_arg_i(a, ARG_ID(RM), kvib->d[2]), as_arg_i(a, ARG_ID(RS), as_x64_rs(kvii->d[2].u3, 8)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvib, ci);
            drop_atm_kv(s, kvii, ci);
        } else if (x == GEN_CLS(V) && gen_var_g_t(i->d[0].p) == X64_TYPE(M) && y == GEN_CLS(D) && (z == GEN_CLS(A) || z == GEN_CLS(T))) {
            tgt = i->d[0].p;
            if (st_stkv_idx(s, gen_var_g_t(tgt), tgt->d[1].u3, &idx) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen stkv inv idx");
            if (gen_as_rrmbd(a, tm, rtmp, R(BP), idx, ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            if ((stat = get_reg(s, i->d[2].p, &kvii) != GEN_STAT(OK))) return gen_err(g, ci, e, "gen reg");
            if (arg_id_bd_g(i->d[1].p, &di) != GEN_STAT(OK)) return gen_err(g, ci, e, "gen inv data offset type");
            if (gen_as(a, ai, to, as_arg_i(a, ARG_ID(RM), U3(rtmp)), as_arg_i(a, ARG_ID(RS), as_x64_rs(kvii->d[2].u3, 8)), as_arg_i(a, di, ((te*) i->d[1].p)->d[1]), ci) != AS_STAT(OK)) return gen_err(g, ci, e, __FUNCTION__);
            drop_atm_kv(s, kvii, ci);
            return GEN_STAT(OK);
        } else return gen_err(g, ci, e, "nyi");
    } else return gen_err(g, ci, e, "nyi");
    return GEN_STAT(OK);
}
