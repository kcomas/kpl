
#include "t.h"

extern _tests *__t;

extern const char *__f;

void _a(const char *name, _test_fn *tf) {
    _tests *t = calloc(1, sizeof(_tests) + strlen(name) + sizeof(char));
    strcpy(t->name, name);
    t->tf = tf;
    if (__t) {
        _tests *h = __t;
        while (h->n) h = h->n;
        h->n = t;
    } else __t = t;
}

int main(void) {
    volatile int p = 0, f = 0; // can't optimize with jit fns
    printf("\e[1m%s\n\e[0m", __f);
    while (__t) {
        _tests *ct = __t;
        __t = __t->n;
        ct->tf(ct);
        printf("\e[%dm%s %s\e[0m", ct->m ? 91 : 92, __f, ct->name);
        if (ct->m) {
            printf("\e[90m:\e[96m%d\e[0m %s\n", ct->ln, ct->m);
            f++;
        } else {
            putchar('\n');
            p++;
        }
        free(ct);
    }
    printf("\e[1;%dm%s \e[92m%d \e[91m%d\n\e[0m", f ? 91 : 92, __f, p, f);
    return f ? f : 0;
}
