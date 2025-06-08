
#include "kpl.h"
#include "er.h"

#define THREAD_STACK_PAGE_MUL 100

inline void tdr_stk_i(tdr *const r) {
    r->stks = getpagesize() * THREAD_STACK_PAGE_MUL;
    r->stk = mmap(NULL, r->stks, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

inline tdr *tdr_i(tds *const s) {
    al *a = al_i((void*) s + s->size);
    s->size += algn(sizeof(al), DEFALGN);
    er *e = er_i(a);
    tdr *r = ala(a, sizeof(tdr));
    r->a = a;
    r->e = e;
    r->j = ala(a, sizeof(jit));
    return r;
}

inline void tdr_f(tdr *r, void *fn) {
    (void) fn;
    er_f(r->e);
    jit_f(r->j);
    if(r->stk) munmap(r->stk, r->stks);
    al *a = r->a;
    alf(r);
    al_f(a);
}

#ifndef MAX_TD
    #define MAX_TD 10
#endif

inline tds *tds_i(void) {
    tds *s = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    s->size = algn(sizeof(tds), DEFALGN);
    return s;
}

inline void tds_a(tds *volatile s, tdr *const r) {
    er_c(r->e);
    LST_A(s, r);
}

inline tdr *tds_g(tds *volatile s, bool stk) {
    tdr *r = NULL;
    if (!s->h) {
        s->total++;
        r = tdr_i(s);
        if (stk) tdr_stk_i(r);
    } else {
        LST_S(s, r);
        if ((stk) && (!r->stk)) tdr_stk_i(r);
    }
    return r;
}

inline void tds_f(tds *s) {
#ifdef KPL_ALD // thread resources taken and returned
    printf("**RT: %lu, RR: %lu**\n", s->total, s->len);
#endif
    LST_F(s, tdr, tdr_f, NULL);
    munmap(s, getpagesize());
}
