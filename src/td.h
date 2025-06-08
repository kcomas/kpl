
#include "kpl.h"
#include "er.h"

#define THREAD_STACK_PAGE_MUL 100

inline void tdr_stk_i(tdr *const r) {
    r->stks = getpagesize() * THREAD_STACK_PAGE_MUL;
    r->stk = mmap(NULL, r->stks, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    r->stkp = r->stk + r->stks;
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
    sem_init(&s->a, -1, 1);
    sem_init(&s->l, -1, 0);
    s->size = algn(sizeof(tds), DEFALGN);
    return s;
}

inline void tds_a(tds *volatile s, tdr *const r) {
    sem_wait(&s->a);
    er_c(r->e);
    LST_A(s, r);
    sem_post(&s->l);
    sem_post(&s->a);
}

inline tdr *tds_g(tds *volatile s, bool stk) {
    tdr *r = NULL;
    if (!s->h) {
        sem_wait(&s->a);
        s->total++;
        r = tdr_i(s);
        if (stk) tdr_stk_i(r);
        sem_post(&s->a);
    } else {
        sem_wait(&s->l);
        LST_S(s, r);
        if ((stk) && (!r->stk)) tdr_stk_i(r);
        sem_post(&s->l);
    }
    return r;
}

inline void tds_f(tds *s) {
#ifdef KPL_ALD // thread resources taken and returned
    printf("**RT: %lu, RR: %lu**\n", s->total, s->len);
#endif
    LST_F(s, tdr, tdr_f, NULL);
    sem_destroy(&s->l);
    munmap(s, getpagesize());
}
