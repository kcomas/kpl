
#include "t.h"

extern _tests *__t;

extern const char *__f;

void _a(const char *n, _test_fn *tf) {
    _tests *t = calloc(1, sizeof(_tests) + strlen(n) + sizeof(char));
    strcpy(t->n, n);
    t->tf = tf;
    if (__t) {
        _tests *h = __t;
        while (h->nt) h = h->nt;
        h->nt = t;
    } else __t = t;
}

#define RG(X) (X ? 91 : 92)

int main(void) {
    volatile int p = 0, f = 0; // can't optimize with jit fns
    while (__t) {
        _tests *ct = __t;
        __t = __t->nt;
        ct->tf(ct);
        printf("\e[%dm%s %s\e[0m ", RG(ct->m), __f, ct->n);
        if (ct->m) {
            printf("\e[1m%s:%d \e[95m%s\n\e[0m", ct->f, ct->ln, ct->m);
            f++;
        } else {
            putchar('\n');
            p++;
        }
        free(ct);
    }
    printf("\e[1;%dm%s \e[92m%d \e[91m%d\n\e[0m", RG(f), __f, p, f);
    return f ? f : 0;
}
