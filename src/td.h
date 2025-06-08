
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

inline tds *tds_i(void) {
    tds *s = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    s->size = algn(sizeof(tds), DEFALGN);
    sem_init(&s->l, -1, 1);
    return s;
}

void tds_a(tds *volatile s, tdr *const r);

tdr *tds_g(tds *volatile s, bool stk);

inline void tds_f(tds *s) {
#ifdef KPL_ALD // thread resources taken and returned
    printf("**RT: %lu, RR: %lu**\n", s->total, s->len);
#endif
    LST_F(s, tdr, tdr_f, NULL);
    sem_destroy(&s->l);
    munmap(s, getpagesize());
}
