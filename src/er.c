
#include "er.h"

static const char *const ets[] = {
   "OK",
   "MOD",
   "TKN",
   "AST",
   "TYPE",
   "CODE",
   "JIT",
   "RUN"
};

const char *er_type_str(er_type et) {
    const char *s = "INVALID_ER";
    if (et >= ER(MOD) && et <= ER(RUN)) s = ets[et];
    return s;
}

extern inline er_itm *er_itm_i(al *const a, er_type et, const char *const fnn, const char *const stat);

void er_itm_p(er_itm *const ei) {
    fprintf(stderr, "!!%s", er_type_str(ei->et));
    if (ei->fnn[0]) fprintf(stderr, ",%s", ei->fnn);
    if (ei->stat) fprintf(stderr, ",%s", ei->stat);
    if (ei->path) fprintf(stderr, ",%s", ei->path);
    if (ei->lno > 0 && ei->cno > 0) fprintf(stderr, ",l:%lu,c:%lu", ei->lno, ei->cno);
    if (ei->sg) fprintf(stderr, ",%s", ei->sg->str);
    fprintf(stderr, "!!\n");
}

void er_itm_f(er_itm *ei, void *fn) {
    (void) fn;
    if (ei->sg) var_sg_f(ei->sg);
    alf(ei);
}

void er_itm_rci(er_itm *const ei) {
    ei->rc++;
}

void er_itm_rcd(er_itm *const ei) {
    ei->rc--;
}

void er_itm_d(er_itm *ei) {
    while (ei) {
        er_itm *tmp = ei;
        ei = ei->next;
        er_itm_f(tmp, NULL);
    }
}

void er_itm_gc(er_itm *ei) {
    if (--ei->rc >= 0) return; // only check head
    er_itm_d(ei);
}

extern inline er *er_i(al *const a);

void er_a(er *const e, er_itm *const ei) {
    LST_A(e, ei);
}

er_itm *er_g(al *const a, er *const e) {
    if (!e->h) return er_itm_i(a, ER(OK), __func__, NULL);
    er_itm *ei = e->h;
    e->h = e->t = NULL;
    return ei;
}

extern inline void er_p(er *const e);

void er_e(mod *const m, er *const e, size_t lno, size_t cno) {
    if (!e->h) return;
    er_itm *ei = er_itm_i(m->r->a, ER(RUN), __func__, "THROW");
    ei->lno = lno;
    ei->cno = cno;
    er_a(e, ei);
    er_p(e);
    exit(KPLE);
}

void er_c(er *const e) {
    LST_F(e, er_itm, er_itm_f, NULL);
}

extern inline void er_f(er *e);
