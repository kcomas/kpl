
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
    printf("!!%s", er_type_str(ei->et));
    if (ei->fnn[0]) printf(",%s",ei->fnn);
    if (ei->stat) printf(",%s", ei->stat);
    if (ei->path) printf(",%s", ei->path);
    if (ei->lno > 0 && ei->cno > 0) printf(",l:%lu,c:%lu", ei->lno, ei->cno);
    if (ei->sg) printf(",%s", ei->sg->str);
    printf("!!\n");
}

void er_itm_f(er_itm *ei, void *fn) {
    (void) fn;
    if (ei->sg) var_sg_f(ei->sg);
    alf(ei);
}

extern inline er *er_i(al *const a);

void er_a(er *const e, er_itm *const ei) {
    LST_A(e, ei);
}

er_itm *er_g(er *const e) {
    er_itm *ei = e->h;
    e->h = e->t = NULL;
    return ei;
}

extern inline void er_p(er *const e);

void er_e(er *const e) {
    if (!e->h) return;
    er_p(e);
    exit(KPLE);
}

void er_c(er *const e) {
    LST_F(e, er_itm, er_itm_f, NULL);
}

extern inline void er_f(er *e);
