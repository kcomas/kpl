
#include "er.h"

static const char *const ets[] = {
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
    if (ei->path) printf(",%s,l:%lu,c:%lu,", ei->path, ei->lno, ei->cno);
    if (ei->msg) printf(",%s", ei->msg->str);
    printf("!!\n");
}

void er_itm_f(er_itm *ei, void *fn) {
    (void) fn;
    if (ei->msg) var_sg_f(ei->msg);
    alf(ei);
}

extern inline er *er_i(al *const a);

void er_a(er *const e, er_itm *const ei) {
    LST_A(e, ei);
}

extern inline void er_p(er *const e);

void er_c(er *const e) {
    LST_F(e, er_itm, er_itm_f, NULL);
}

extern inline void er_f(er *e);
