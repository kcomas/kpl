
#include "../src/lib/vr.h"
#include "t.h"

I;

static const alfr vm = { .a = malloc, .f = free };

T(vr_ab_s_i, {
    vr *v = vr_i(2, &vm, NULL);
    vr_ab(&v, I5(1));
    vr_ab(&v, I5(2));
    vr_ab(&v, I5(3));
    vr_ab(&v, I5(4));
    A(v->l == 4, "len");
    vr_s_i(v, 1, I5(8));
    A(v->d[1].i5 == 8, "substitute");
    for (size_t i = 0; i < v->l; i++) printf("%d ", v->d[i].i5);
    putchar('\n');
    vr_f(v);
});

T(reverse, {
    vr *v = vr_i(10, &vm, NULL);
    vr_ab(&v, D(1.1));
    vr_ab(&v, D(2.2));
    vr_ab(&v, D(3.3));
    vr_ab(&v, D(4.4));
    vr_af(&v, D(5.5));
    A(v->l == 5, "len");
    A(v->d[0].d == 5.5, "vr_af");
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].d);
    putchar('\n');
    vr_r(v);
    A(v->d[4].d == 5.5, "vr_r");
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].d);
    putchar('\n');
    vr_r(v);
    A(v->d[0].d == 5.5, "vr_r");
    un u;
    for (size_t i = 0; i < 5; i++) {
        A(vr_sb(v, &u) == VR_STAT(OK), "vr_sb");
        printf("%f,", u.d);
    }
    putchar('\n');
    A(vr_sb(v, &u) == VR_STAT(SUB), "not empty");
    vr_f(v);
});

T(strings, {
    char *msg = "Hi: 0";
    size_t ml = strlen(msg);
    vr *v = vr_i(5, &vm, &free);
    for (size_t i = 0; i < 10; i++) {
        char *s = calloc(1, ml + sizeof(char));
        strcpy(s, msg);
        s[ml - 1] = i + '0';
        vr_ab(&v, P(s));
    }
    for (size_t i = 0; i < v->l; i++) printf("%s, ", (char*) v->d[i].p);
    putchar('\n');
    un u;
    A(vr_g_i(v, 2, &u) == VR_STAT(OK), "vr_g_i");
    printf("2: %s\n", (char*) u.p);
    char *hw = "Hello World";
    ml = strlen(hw);
    char *s2 = calloc(ml + sizeof(char), sizeof(char));
    strcpy(s2, hw);
    vr_s_i(v, 2, P(s2));
    vr_sf(v, &u);
    free(u.p);
    for (size_t i = 0; i < v->l; i++) printf("%s, ", (char*) v->d[i].p);
    putchar('\n');
    vr_r(v);
    for (size_t i = 0; i < v->l; i++) printf("%s, ", (char*) v->d[i].p);
    putchar('\n');
    A(strcmp(v->d[0].p, "Hi: 9") == 0, "vr_r str");
    vr_f(v);
});
