
#include "../src/lib/vr.h"
#include "t.h"

T(vr_ab_s_i) {
    vr *v = vr_i(2, &al_vr, NULL);
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
}

T(reverse) {
    vr *v = vr_i(10, &al_vr, NULL);
    vr_ab(&v, F6(1.1));
    vr_ab(&v, F6(2.2));
    vr_ab(&v, F6(3.3));
    vr_ab(&v, F6(4.4));
    vr_af(&v, F6(5.5));
    A(v->l == 5, "len");
    A(v->d[0].f6 == 5.5, "vr_af");
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].f6);
    putchar('\n');
    vr_r(v);
    A(v->d[4].f6 == 5.5, "vr_r");
    for (size_t i = 0; i < v->l; i++) printf("%f ", v->d[i].f6);
    putchar('\n');
    vr_r(v);
    A(v->d[0].f6 == 5.5, "vr_r");
    un u;
    for (size_t i = 0; i < 5; i++) {
        A(vr_sb(v, &u) == VR_STAT(OK), "vr_sb");
        printf("%f,", u.f6);
    }
    putchar('\n');
    A(vr_sb(v, &u) == VR_STAT(SUB), "not empty");
    vr_f(v);
}

T(strings) {
    char *msg = "Hi: 0";
    size_t ml = strlen(msg);
    vr *v = vr_i(5, &al_vr, free);
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
}

static bool i6_eq(un a, un b) {
    return a.i6 == b.i6;
}

T(eq) {
    vr *a = vr_i(2, &al_vr, NULL);
    vr_ab(&a, I6(1));
    vr_ab(&a, I6(2));
    vr *b = vr_i(2, &al_vr, NULL);
    vr_ab(&b, I6(1));
    vr_ab(&b, I6(2));
    A(vr_eq(a, b, i6_eq), "vr_eq");
    vr *c = vr_i(5, &al_vr, NULL);
    vr_ab(&c, I6(1));
    vr_ab(&c, I6(4));
    A(!vr_eq(b, c, i6_eq), "!vr_eq");
    vr_f(a);
    vr_f(b);
    vr_f(c);
}

typedef void vr_test_print(un u);

static void print_vr(vr *v, vr_test_print fn) {
    for (size_t i = 0; i < v->l; i++) fn(v->d[i]);
    putchar('\n');
}

static void vr_i6_p(un u) {
    printf("%ld ", u.i6);
}

static ssize_t vr_i6_lt_cmp(un a, un b) {
    return a.i6 - b.i6;
}

static ssize_t vr_i6_gt_cmp(un a, un b) {
    return b.i6 - a.i6;
}

T(sort) {
    vr *a = vr_i_v(&al_vr, NULL, 5, 5, 2, 1, 3, 4);
    print_vr(a, vr_i6_p);
    vr_s(a, vr_i6_lt_cmp);
    for (size_t i = 1; i < a->l; i++) A(a->d[i-1].i6 < a->d[i].i6, "inv sort");
    print_vr(a, vr_i6_p);
    vr_s(a, vr_i6_gt_cmp);
    for (size_t i = 1; i < a->l; i++) A(a->d[i-1].i6 > a->d[i].i6, "inv sort");
    print_vr(a, vr_i6_p);
    vr_f(a);
}
