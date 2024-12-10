
#include <stdio.h>
#include <string.h>
#include "../src/vr.h"

void itest(void) {
    vr *v = vr_i(2, &malloc, NULL, &free);
    vr_a(&v, I5(1));
    vr_a(&v, I5(2));
    vr_a(&v, I5(3));
    vr_a(&v, I5(4));
    for (size_t i = 0; i < v->l; i++) printf("%d ", v->d[i].i5);
    putchar('\n');
    vr_f(v);
}

void dtest(void) {
    vr *v = vr_i(2, &malloc, NULL, &free);
    vr_a(&v, D(1.1));
    vr_a(&v, D(2.2));
    vr_a(&v, D(3.3));
    vr_a(&v, D(4.4));
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].d);
    un u;
    for (size_t i = 0; i < 4; i++) {
        if (vr_s(v, &u) != VR_STAT(OK)) exit(12);
        printf("%f ", u.d);
    }
    putchar('\n');
    if (vr_s(v, &u) != VR_STAT(SUB)) exit(13);
    vr_f(v);
}

void stest(void) {
    char *msg = "Hi: 0";
    size_t ml = strlen(msg);
    vr *v = vr_i(5, &malloc, &free, &free);
    for (size_t i = 0; i < 10; i++) {
        char *s = calloc(1, ml + sizeof(char));
        strcpy(s, msg);
        s[ml - 1] = i + '0';
        vr_a(&v, P(s));
    }
    for (size_t i = 0; i < 9; i++) printf("%s\n", (char*) v->d[i].p);
    un u;
    if (vr_g_i(v, 2, &u) != VR_STAT(OK)) exit(14);
    printf("2: %s\n", (char*) u.p);
    vr_f(v);
}

int main(void) {
    itest();
    dtest();
    stest();
    return 0;
}
