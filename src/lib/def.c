
#include "def.h"

static bool is_cont(size_t n, ...) {
    va_list args;
    va_start(args, n);
    while (n > 0) {
        char c = (char) va_arg(args, int);
        if ((c & 0xC0) != 0x80) return false;
        n--;
    }
    va_end(args);
    return true;
}

un c4_g(const char *str, size_t s, size_t *e) {
    if ((str[s] & 0x80) == 0x0) {
        *e = s;
        return C1(str[s]);
    } else if ((str[s] & 0xE0) == 0xC0 && is_cont(1, str[s + 1])) {
        *e = s + 1;
        return C2(str[s], str[s + 1]);
    } else if ((str[s] & 0xF0) == 0xE0 && is_cont(2, str[s + 1], str[s + 2])) {
        *e = s + 2;
        return C3(str[s], str[s + 1], str[s + 2]);
    } else if ((str[s] & 0xF8) == 0xF0 && is_cont(3, str[s + 1], str[s + 2], str[s + 3])) {
        *e = s + 3;
        return C4(str[s], str[s + 1], str[s + 2], str[s + 3]);
    }
    return C1('\0');
}

bool c4_eq(un a, un b) {
    return a.c.a == b.c.a && a.c.b == b.c.b && a.c.c == b.c.c && a.c.d == b.c.d;
}
