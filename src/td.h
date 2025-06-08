
#include "kpl.h"
#include "er.h"

inline tdr *tdr_i(void) {
    al *a = al_i();
    er *e = er_i(a);
    tdr *r = ala(a, sizeof(tdr));
    r->a = a;
    r->e = e;
    return r;
}

inline void tdr_f(tdr *r, void *fn) {
    (void) fn;
    er_f(r->e);
    FNNF(r->j, jit_f);
    al *a = r->a;
    alf(r);
    al_f(a);
}

inline tds *tds_i() {
    tds *s = calloc(1, sizeof(tds));
    pthread_mutex_init(&s->pm, NULL);
    return s;
}

inline void tds_a(tds *const s, tdr *const r) {
    er_c(r->e);
    pthread_mutex_lock(&s->pm);
    LST_A(s, r);
    pthread_mutex_unlock(&s->pm);
}

inline tdr *tds_g(tds *const s) {
    tdr *r;
    pthread_mutex_lock(&s->pm);
    if (!s->h) {
        s->total++;
        r = tdr_i();
    } else {
        LST_S(s, r);
    }
    pthread_mutex_unlock(&s->pm);
    return r;
}

inline void tds_f(tds *s) {
#ifdef ALD // thread resources taken and returned
    printf("**RT: %lu, RR: %lu**\n", s->total, s->len);
#endif
    LST_F(s, tdr, tdr_f, NULL);
    free(s);
}
