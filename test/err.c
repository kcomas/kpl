
#include "../src/lib/err.h"
#include "t.h"

T(b) {
    err *e = err_i(&al_err, NULL, NULL, NULL, "basic error");
    err_p(e, 0, true);
    err_f(e);
}

T(nerr) {
    err_p(NULL, 0, true);
}
