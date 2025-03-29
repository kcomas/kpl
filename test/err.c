
#include "../src/lib/err.h"
#include "t.h"

static const alfr em = { .a = malloc, .f = free };

T(b) {
    err *e = err_i(&em, NULL, NULL, NULL, "basic error");
    err_p(e);
    err_f(e);
}

T(nerr) {
    err_p(NULL);
}
