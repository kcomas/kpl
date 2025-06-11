
#include "z.h"

static const alfr z_al = { .a = malloc, .f = free };

static psr *bp = NULL;

static ast *ba = NULL;

static fld *bf = NULL;

static chk *bc = NULL;

static fld *bo = NULL;

static as *bs = NULL;

static gen_st *bst = NULL;

static gen *bg = NULL;

static atg *bt = NULL;

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(5, &al_te, NULL);
    return tbl_i(&al_tbl, tkn_sh, c4_eq, tl, b);
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(30, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
}

static ast_stat pig(te *pn, size_t *pid) {
    if (!pn || pn->l < 2) return AST_STAT(INV);
    *pid = pn->d[1].u6;
    return AST_STAT(OK);
}

static tbl *ati(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_mc_sdbm, tbl_mc_eq, tl, b);
}

static lst *ali(void) {
    return lst_i(&al_lst, &al_te, (void*) te_f);
}

static tbl *mktbls(size_t size) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(size, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
}

static tbl *as_arg_tbl(void) {
    return mktbls(5);
}

static tbl *gen_cls_info_tbl(void) {
    return mktbls(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1);
}

static __attribute__((constructor)) void z_con(void) {
    bp = psr_b(psr_i(&z_al, &al_te, &al_lst, &al_err, psr_entry_f, mktbl, tkn_b(tkn_i(&z_al, &al_te, &al_err, tkn_mktbl, tkn_df, mc_i(0, &al_mc))), vr_i(10, &al_vr, (void*) te_f)));
    ba = ast_b(ast_i(&z_al, &al_te, &al_mc, &al_err, node_err_p, pig, ali, mktbls(NODE_TYPE(_END)), mktbls(TCUST(_END))));
    bf = fld_b(fld_i(&z_al, &al_te, &al_err, ati, ali, NULL, mktbls(AST_CLS(_))));
    bc = chk_b(chk_i(&z_al, &al_te, &al_lst, &al_tbl, &al_err, chk_err, mktbl, NULL));
    bo = opt_b(fld_i(&z_al, &al_te, &al_err, ati, ali, NULL, mktbls(AST_CLS(_))));
    bs = as_b(as_i(&z_al, &al_te, &al_lst, &al_err, as_x64_err_g_p, as_arg_tbl, mktbls(AS_X64(_END)), lst_i(&al_lst, &al_te, (void*) te_f)));
    bst = gen_st_i(&z_al, &al_te, mktbls(10), mktbls(10), vr_i(16, &al_vr, NULL), vr_i(16, &al_vr, NULL));
    bg = gen_b(gen_i(&z_al, &al_te, &al_vr, &al_err, gen_cls_info_tbl, mktbls(GEN_OP(_END)), lst_i(&al_lst, &al_te, (void*) te_f)));
    bt = atg_b(atg_i(&z_al, &al_te, &al_err, atg_err, mktbl, lst_i(&al_lst, &al_te, NULL), lst_i(&al_lst, &al_te, (void*) te_f), tbl_i(&al_tbl, type_un_hsh, type_un_eq, lst_i(&al_lst, &al_te, (void*) te_f), te_i(10, &al_te, NULL)),gen_i_gen(bg), as_i_as(bs)));
}

static __attribute__((destructor)) void z_des(void) {
    psr_f(bp);
    ast_f(ba);
    fld_f(bf);
    chk_f(bc);
    fld_f(bo);
    atg_f(bt);
    as_f(bs);
    gen_st_f(bst);
    gen_f(bg);
}

static void z_e_p(void *d) {
    mc *fn = d;
    printf("\e[1m%s\e[0m", (char*) fn->d);
}

#define APLYLSTS 4 // {}()

static size_t cdl = 0; // current number of destructors

static void z_fn_err_f(void *p) {
    te *t = p;
    mc_f(t->d[0].p);
    err_f(t->d[1].p);
    t->af->f(t);
}

static void z_fn_e_p(void *p) {
    te *t = p;
    z_e_p(t->d[0].p);
    putchar('\n');
    err_p(t->d[1].p, false);
}

static err *z_err(mc *fn, err *e) {
    te *t = te_i(2, &al_te, z_fn_err_f);
    t->d[0] = P(mc_c(fn));
    t->d[1] = P(e);
    return err_i(&al_err, z_fn_e_p, (void*) te_f, t, __FUNCTION__);
}

err *z(mc *fn, tbl **et, uint8_t dflgs) {
    err *e = NULL;
    mc *pgm = NULL;
    int fd;
    if ((fd = open((char*) fn->d, O_RDONLY)) == -1) return err_i(&al_err, z_e_p, (void*) mc_f, mc_c(fn), "inv file, all paths relative to exec");
    struct statx sx;
    if (statx(fd, "", AT_EMPTY_PATH, STATX_MODE | STATX_SIZE, &sx) == -1) {
        close(fd);
        return err_i(&al_err, z_e_p, (void*) mc_f, mc_c(fn), __FUNCTION__);
    }
    if (S_ISDIR(sx.stx_mode)) return err_i(&al_err, z_e_p, (void*) mc_f, mc_c(fn), "path is dir");
    pgm = mc_i(sx.stx_size + APLYLSTS + sizeof(char), &al_mc);
    pgm->d[0] = '{';
    if (read(fd, pgm->d + 1, sx.stx_size) == -1) {
        close(fd);
        mc_f(pgm);
        return err_i(&al_err, z_e_p, (void*) mc_f, mc_c(fn), __FUNCTION__);
    }
    close(fd);
    pgm->d[sx.stx_size + 1] = '}';
    pgm->d[sx.stx_size + 2] = '(';
    pgm->d[sx.stx_size + 3] = ')';
    psr *zp = psr_i_psr(bp, mc_c(pgm));
    te *nh = te_i(3, zp->ta, psr_n_err_f), *an = NULL, *dh;
    if (psr_n(zp, nh, &e) != PSR_STAT(END)) {
        te_f(nh);
        psr_f(zp);
        mc_f(pgm);
        return z_err(fn, e);
    }
    nh = psr_g_rn(zp, nh);
    if (dflgs & Z_D_FLG(P)) {
        node_p(nh->d[2].p, 0);
        putchar('\n');
    }
    ast *za = ast_i_ast(ba);
    if (ast_n(za, NULL, nh, (void**) &an, &e) != AST_STAT(OK)) {
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (dflgs & Z_D_FLG(A)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld *zf = fld_i_fld(bf, za);
    if (fld_n(zf, &an, &e, true) != FLD_STAT(OK)) {
        fld_f(zf);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (dflgs & Z_D_FLG(F)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld_f(zf);
    chk *zc = chk_i_chk(bc, za);
    if (chk_n(zc, an, &e) != CHK_STAT(OK)) {
        chk_f(zc);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (dflgs & Z_D_FLG(C)) {
        ast_p(an, 0);
        putchar('\n');
    }
    chk_f(zc);
    fld *zo = fld_i_fld(bo, za);
    if (fld_n(zo, &an, &e, false) != FLD_STAT(OK)) {
        fld_f(zo);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (dflgs & Z_D_FLG(O)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld_f(zo);
    atg *zt = atg_i_atg(bt);
    if (atg_q(zt, &an, atg_x64_enq) != ATG_STAT(OK)) {
        ast_f(za);
        atg_f(zt);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (zt->dt->i->l > cdl) {
        dh = zt->dt->i->t;
        cdl++;
        while (cdl < zt->dt->i->l) {
            dh = dh->d[1].p;
            cdl++;
        }
        while (dh) {
            te *h = dh->d[0].p;
            gen *dg = NULL;
            void *dfn = NULL;
            if (atg_d_n(zt, h, &dg, &dfn, gen_type_des, &e) != ATG_STAT(OK)) {
                ast_f(za);
                atg_f(zt);
                te_f(an);
                mc_f(pgm);
                gen_f(dg);
                return z_err(fn, e);
            }
            if (dfn) h->d[1] = P(fn);
            else if (dg) {
                gen_st *dst = gen_st_i_gen_st(bst);
                if (gen_st_p1(dg, dst) != GEN_STAT(OK)) {
                    ast_f(za);
                    atg_f(zt);
                    te_f(an);
                    mc_f(pgm);
                    gen_st_f(dst);
                    gen_f(dg);
                    return z_err(fn, err_i(&al_err, z_e_p, (void*) mc_f, mc_c(fn), "des gen st p1"));
                }
                gen_x64_opt(dg, dst);
                size_t pc = p;
                if (gen_n(dg, dst, zt->a, &e) != GEN_STAT(OK)) {
                    ast_f(za);
                    atg_f(zt);
                    te_f(an);
                    mc_f(pgm);
                    gen_st_f(dst);
                    gen_f(dg);
                    return z_err(fn, e);
                }
                if (as_n(zt->a, &p, m, &e) != AS_STAT(OK)) {
                    ast_f(za);
                    atg_f(zt);
                    te_f(an);
                    mc_f(pgm);
                    gen_st_f(dst);
                    gen_f(dg);
                    return z_err(fn, e);
                }
                gen_st_f(dst);
                gen_f(dg);
                h->d[1] = P(&m[pc]);
            }
            dh = dh->d[2].p;
        }
    }
    while (zt->q->l > 0) {
        gen *zg = NULL;
        if (atg_n(zt, &zg, za, &e) != ATG_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            te_f(an);
            mc_f(pgm);
            return z_err(fn, e);
        }
        gen_st *zst = gen_st_i_gen_st(bst);
        if (gen_st_p1(zg, zst) != GEN_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            gen_st_f(zst);
            te_f(an);
            mc_f(pgm);
            return z_err(fn, e);
        }
        gen_x64_opt(zg, zst);
        if (dflgs & Z_D_FLG(G)) {
            gen_p(zg, NULL);
            putchar('\n');
        }
        if (gen_n(zg, zst, zt->a, &e) != GEN_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            gen_st_f(zst);
            te_f(an);
            mc_f(pgm);
            return z_err(fn, e);
        }
        gen_st_f(zst);
        gen_f(zg);
    }
    ast_f(za);
    if (as_n(zt->a, &p, m, &e) != AS_STAT(OK)) {
        atg_f(zt);
        te_f(an);
        mc_f(pgm);
        return z_err(fn, e);
    }
    if (dflgs & Z_D_FLG(S)) {
        as_code_p(zt->a, m);
        putchar('\n');
    }
    ssize_t ep = as_lbl_g_c_i(zt->a, ((te*) an->d[4].p)->d[4].u5);
    if (ep < 0) return err_i(&al_err, NULL, NULL, NULL, __FUNCTION__);
    if (an->d[3].p) *et = tbl_c(an->d[3].p);
    te_f(an);
    e = atg_z(zt, *et, m, ep);
    atg_f(zt);
    mc_f(pgm);
    return e;
}
