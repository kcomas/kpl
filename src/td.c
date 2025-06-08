
#include "td.h"

extern inline void tdr_stk_i(tdr *const r);

extern inline tdr *tdr_i(tds *const s);

extern inline void tdr_f(tdr *r, void *fn);

extern inline tds *tds_i(void);

void tds_a(tds *volatile s, tdr *const r) {
    sem_wait(&s->l);
    er_c(r->e);
    LST_A(s, r);
    sem_post(&s->l);
}

tdr *tds_g(tds *volatile s, bool stk) {
    sem_wait(&s->l);
    tdr *r = NULL;
    if (!s->h) {
        s->total++;
        r = tdr_i(s);
        if (stk) tdr_stk_i(r);
    } else {
        LST_S(s, r);
        if ((stk) && (!r->stk)) tdr_stk_i(r);
    }
    sem_post(&s->l);
    return r;
}

extern inline void tds_f(tds *s);
