
#include "psr.h"

psr *psr_i(alfn *pa, frfn *pf, frfn *pef, psr_tbl_i *pti, tkn *tt, vr *ts) {
    psr *p = pa(sizeof(psr));
    p->r = 1;
    p->idc = PARSER(_);
    p->pa = pa;
    p->pf = pf;
    p->pef = pef;
    p->pti = pti;
    p->tt = tt;
    p->ts = ts;
    p->pt = pti();
    return p;
}

size_t psr_a(psr *const p, size_t pid, size_t mode, te *const st, psr_each_fn *ef, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...) {
    tbl *pt = p->pt;
    te *kv;
    size_t nid = 0;
    va_list tkns;
    va_start(tkns, nt);
    while (nt > 0) {
        size_t iid = va_arg(tkns, size_t);
        if (tbl_g_i(pt, U6(iid), &kv) == TBL_STAT(NF)) {
            while (nt > 0) {
                kv = te_i(8, p->pa, p->pef);
                kv->d[0] = U6(iid);
                kv->d[7] = P(p->pti());
                tbl_a(pt, kv);
                pt = kv->d[7].p;
                iid = va_arg(tkns, size_t);
                nt--;
            }
            kv->d[1] = pid ? U6(pid) : U6(p->idc++);
            kv->d[2] = U6(mode);
            kv->d[3] = P(st);
            kv->d[4] = P(ef);
            kv->d[5] = P(mf);
            kv->d[6] = P(nf);
            va_end(tkns);
            return pid;
        }
        nid = kv->d[1].u6;
        pt = kv->d[7].p;
        nt--;
    }
    va_end(tkns);
    return nid;
}

psr_stat psr_n(psr *const p, te *const nh) {
    tkn_stat tstat;
    psr_stat pstat;
    te *m = te_i(5, p->pa, p->pf);
    for (;;) {
        if ((tstat = tkn_n(p->tt, m)) != TKN_STAT(OK)) break;
        psr_mode pm = PSR_MODE(NONE);
        te *st = NULL;
        psr_each_fn *ef = NULL;
        psr_megre_fn *mf = NULL;
        psr_node_fn *nf = NULL;
        tbl *pt = p->pt;
        te *kv, *pn;
        while (tbl_g_i(pt, m->d[0], &kv) == TBL_STAT(OK)) {
            vr_ab(&p->ts, P(m));
            pm = kv->d[2].u6;
            st = kv->d[3].p;
            ef = kv->d[4].p;
            mf = kv->d[5].p;
            nf = kv->d[6].p;
            pt = kv->d[7].p;
            m = te_i(5, p->pa, p->pf);
            if ((tstat = tkn_n(p->tt, m)) != TKN_STAT(OK)) break;
        }
        tkn_s(p->tt, m);
        if (pm == PSR_MODE(NONE)) return PSR_STAT(INV);
        else if (pm == PSR_MODE(STOP)) {
            te_f(m);
            return PSR_STAT(END);
        } else if (pm == PSR_MODE(LOOP)) {
            if (!st || !ef || !nf || !mf) return PSR_STAT(INV);
            if ((pstat = nf(p, &pn)) != PSR_STAT(OK)) return pstat;
            vr_d(p->ts);
            te *lnh = te_i(3, p->pa, p->pf);
            for (;;) {
                lnh->d[0] = P(NULL);
                lnh->d[1] = P(NULL);
                lnh->d[2] = P(NULL);
                if ((pstat = psr_n(p, lnh)) != PSR_STAT(END)) return pstat;
                if ((pstat = ef(p, pn, lnh->d[0].p ? lnh->d[0].p : lnh->d[2].p)) != PSR_STAT(OK)) return pstat;
                for (size_t i = 0; i < st->l; i++) if (((te*) p->ts->d[p->ts->l - 1].p)->d[0].u6 == st->d[i].u6) goto el;
                vr_d(p->ts);
            }
            el:
            if ((pstat = mf(p, nh, pn)) != PSR_STAT(OK)) return pstat;
            te_f(lnh);
            vr_d(p->ts);
            te_f(m);
            m = te_i(5, p->pa, p->pf);
        } else if (nf && mf) {
            if ((pstat = nf(p, &pn)) != PSR_STAT(OK)) return pstat;
            vr_d(p->ts);
            if ((pstat = mf(p, nh, pn)) != PSR_STAT(OK)) return pstat;
        }
    }
    te_f(m);
    return tstat == TKN_STAT(END) ? PSR_STAT(END) : PSR_STAT(INV);
}

void psr_f(psr *p) {
    if (!p || --p->r > 0) return;
    tkn_f(p->tt);
    vr_f(p->ts);
    tbl_f(p->pt);
    p->pf(p);
}
