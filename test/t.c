
#include "t.h"

bool _tl = false;

_tests *__t = NULL;

static void add(_tests **r, _tests *t) {
    if (*r) {
        _tests *h = *r;
        while (h->nt) h = h->nt;
        h->nt = t;
    } else *r = t;
}

void _a(const char *n, const char *sf, _test_fn *tf) {
    _tests *t = calloc(1, sizeof(_tests) + strlen(n) + sizeof(char));
    strcpy(t->n, n);
    t->sf = sf;
    t->tf = tf;
    add(&__t, t);
}

#define RG(X) (X ? 91 : 92)

#define H(CT) printf("\e[%dm%s %s\e[0m ", RG(CT->m), CT->sf, CT->n);

#define F(CT) printf("\e[1m%s:%d \e[95m%s\n\e[0m", CT->ef, CT->ln, CT->m);

int main(void) {
#ifdef NTO
    if (_tl) STOP("TO() NEEDS TO BE REMOVED");
#endif
    volatile int p = 0, f = 0;
    _tests *tf = NULL;
    while (__t) {
        _tests *ct = __t;
        __t = __t->nt;
        ct->nt = NULL;
        ct->tf(ct);
        H(ct);
        if (ct->m) {
            F(ct);
            f++;
            add(&tf, ct);
        } else {
            putchar('\n');
            p++;
            free(ct);
        }
    }
    printf("\e[1;%dm%s \e[92m%d \e[91m%d\n\e[0m", RG(f), f ? "FAIL" : "PASS", p, f);
    while (tf) {
        _tests *ct = tf;
        tf = tf->nt;
        H(ct);
        F(ct);
        free(ct);
    }
    return f ? f : 0;
}
