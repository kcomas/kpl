
#include "t.h"

extern _tests *_t;

extern const char *_f;

void _a(const char *name, _test_fn *tf) {
    _tests *t = calloc(1, sizeof(_tests) + strlen(name) + sizeof(char));
    strcpy(t->name, name);
    t->tf = tf;
    if (_t) {
        _tests *h = _t;
        while (h->n) h = h->n;
        h->n = t;
    } else _t = t;
}

int main(void) {
    size_t p = 0, f = 0;
    printf("\e[97m%s\n\e[0m", _f);
    while (_t) {
        _tests *ct = _t;
        _t = _t->n;
        ct->tf(ct);
        if (ct->m) {
            printf("\e[1;31m%s\e[0m:\e[96m%d\e[0m %s\n", ct->name, ct->ln, ct->m);
            f++;
        } else {
            printf("\e[1;32m%s\n\e[0m", ct->name);
            p++;
        }
        free(ct);
    }
    printf("\e[%dm%s \e[1;32m%lu \e[1;31m%lu\n\e[0m", f ? 91 : 92, _f, p, f);
    return f ? f : 0;
}
