
#include "def.h"

#define _UOGC(CT, T, O) CT T##_g_o(un u, size_t o) { \
    un a; \
    a.u6 = u.u6 >> (O * o) & (((uint64_t) 1 << O) - 1); \
    return a.T; \
}

#define _UOSC(CT, T, O) un T##_s_o(un u, size_t o, CT v) { \
    un a = UN(T, v); \
    uint64_t b = (((uint64_t) 1 << O) - 1) << (O * o); \
    return UN(u6, (u.u6 & ~b) | (a.u6 << (O * o))); \
}

#define _UOC(CT, T, O) _UOGC(CT, T, O); \
    _UOSC(CT, T, O)

_UOC(uint8_t, u3, 8);
_UOC(int8_t, i3, 8);
_UOC(uint16_t, u4, 16);
_UOC(int16_t, i4, 16);
_UOC(uint32_t, u5, 32);
_UOC(int32_t, i5, 32);
_UOC(float, f5, 32);

#define _UN_OP_C(N, S, O, X) un S##_##O(un a, un b) { return N(a.S X b.S); }

#define _UN_OP_CC(O, X) _UN_OP_C(U3, u3, O, X) \
    _UN_OP_C(U4, u4, O, X) \
    _UN_OP_C(U5, u5, O, X) \
    _UN_OP_C(U6, u6, O, X) \
    _UN_OP_C(I3, i3, O, X) \
    _UN_OP_C(I4, i4, O, X) \
    _UN_OP_C(I5, i5, O, X) \
    _UN_OP_C(I6, i6, O, X) \
    _UN_OP_C(F5, f5, O, X) \
    _UN_OP_C(F6, f6, O, X)

_UN_OP_CC(add, +)
_UN_OP_CC(sub, -)
_UN_OP_CC(mul, *)
_UN_OP_CC(div, /)

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
