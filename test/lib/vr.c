
#include <stdio.h>
#include <string.h>
#include "../../src/lib/vr.h"

void itest(void) {
    vr *v = vr_i(2, &malloc, NULL, &free);
    vr_ab(&v, I5(1));
    vr_ab(&v, I5(2));
    vr_ab(&v, I5(3));
    vr_ab(&v, I5(4));
    vr_s_i(v, 1, I5(8));
    for (size_t i = 0; i < v->l; i++) printf("%d ", v->d[i].i5);
    putchar('\n');
    vr_f(v);
}

void dtest(void) {
    vr *v = vr_i(2, &malloc, NULL, &free);
    vr_ab(&v, D(1.1));
    vr_ab(&v, D(2.2));
    vr_ab(&v, D(3.3));
    vr_ab(&v, D(4.4));
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].d);
    un u;
    for (size_t i = 0; i < 4; i++) {
        if (vr_sb(v, &u) != VR_STAT(OK)) exit(12);
        printf("%f ", u.d);
    }
    putchar('\n');
    if (vr_sb(v, &u) != VR_STAT(SUB)) exit(13);
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
        vr_ab(&v, P(s));
    }
    for (size_t i = 0; i < 9; i++) printf("%s, ", (char*) v->d[i].p);
    putchar('\n');
    un u;
    if (vr_g_i(v, 2, &u) != VR_STAT(OK)) exit(14);
    printf("2: %s\n", (char*) u.p);
    char *hw = "Hello World";
    ml = strlen(hw);
    char *s2 = calloc(ml + sizeof(char), sizeof(char));
    strcpy(s2, hw);
    vr_s_i(v, 2, P(s2));
    for (size_t i = 0; i < 9; i++) printf("%s, ", (char*) v->d[i].p);
    putchar('\n');
    vr_f(v);
}

int main(void) {
    itest();
    dtest();
    stest();
    return 0;
}
