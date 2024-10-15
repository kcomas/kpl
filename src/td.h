
#include "kpl.h"
#include "er.h"

#define THREAD_STACK_PAGE_MUL 100

inline tdr *tdr_i(void) {
    al *a = al_i();
    er *e = er_i(a);
    tdr *r = ala(a, sizeof(tdr));
    r->a = a;
    r->e = e;
    r->j = ala(a, sizeof(jit));
    r->stks = getpagesize() * THREAD_STACK_PAGE_MUL;
    r->stk = mmap(NULL, r->stks, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    r->stkp = r->stk + r->stks;
    return r;
}

inline void tdr_f(tdr *r, void *fn) {
    (void) fn;
    er_f(r->e);
    jit_f(r->j);
    munmap(r->stk, r->stks);
    al *a = r->a;
    alf(r);
    al_f(a);
}

inline tds *tds_i() {
    return mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

inline void tds_a(tds *volatile s, tdr *const r) {
    while (s->lock) {}
    s->lock = true;
    er_c(r->e);
    LST_A(s, r);
    s->lock = false;
}

inline tdr *tds_g(tds *volatile s) {
    while (s->lock) {}
    s->lock = true;
    tdr *r = NULL;
    if (!s->h) {
        s->total++;
        r = tdr_i();
    } else {
        LST_S(s, r);
    }
    s->lock = false;
    return r;
}

inline void tds_f(tds *s) {
#ifdef ALD // thread resources taken and returned
    printf("**RT: %lu, RR: %lu**\n", s->total, s->len);
#endif
    LST_F(s, tdr, tdr_f, NULL);
    munmap(s, getpagesize());
}
