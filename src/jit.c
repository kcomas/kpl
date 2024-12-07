
#include "jit.h"

void jit_a(size_t *pos, uint8_t *m, uint8_t b) {
    m[*pos++] = b;
}

void jit_b(size_t *pos, uint8_t *m, size_t len, ...) {
    va_list args;
    va_start(args, len);
    for (size_t i = 0; i < len; i++) jit_a(pos, m, (uint8_t) va_arg(args, int));
    va_end(args);
}

void jit_c(size_t *pos, uint8_t *m, size_t len, uint8_t b[]) {
    for (size_t i = 0; i < len; i++) jit_a(pos, m, b[i]);
}
