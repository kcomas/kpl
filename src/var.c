
#include "var.h"

var *var_i(var_type t, var_data d) {
    var *v = calloc(1, sizeof(var));
    v->t = t;
    v->r = 1;
    v->d = d;
    return v;
}

var *var_c(var *v) {
    switch (v->t) {
        case VAR_TYPE(VD):
        case VAR_TYPE(BL):
        case VAR_TYPE(I3):
        case VAR_TYPE(I4):
        case VAR_TYPE(I5):
        case VAR_TYPE(I6):
        case VAR_TYPE(U3):
        case VAR_TYPE(U4):
        case VAR_TYPE(U5):
        case VAR_TYPE(U6):
        case VAR_TYPE(F5):
        case VAR_TYPE(F6):
        case VAR_TYPE(CR):
            return var_i(v->t, v->d);
        default:
            v->r++;
    }
    return v;
}

void var_p(const var *const v, const var *const s, const var *const e, const var *const sep) {
    if (!v) return;
    if (s) var_p(s, NULL, NULL, NULL);
    switch (v->t) {
        case VAR_TYPE(UN):
            var_p(v->d.v, s, e, sep);
            break;
        case VAR_TYPE(VD):
            printf("%p", v->d.vd);
            break;
        case VAR_TYPE(BL):
            putchar(v->d.bl ? 'T' : 'F');
            break;
        case VAR_TYPE(I3):
            printf("%d", v->d.i3);
            break;
        case VAR_TYPE(I4):
            printf("%d", v->d.i4);
            break;
        case VAR_TYPE(I5):
            printf("%d", v->d.i5);
            break;
        case VAR_TYPE(I6):
            printf("%ld", v->d.i6);
            break;
        case VAR_TYPE(U3):
            printf("%u", v->d.u3);
            break;
        case VAR_TYPE(U4):
            printf("%u", v->d.u4);
            break;
        case VAR_TYPE(U5):
            printf("%u", v->d.u5);
            break;
        case VAR_TYPE(U6):
            printf("%lu", v->d.u6);
            break;
        case VAR_TYPE(F5):
            printf("%f", v->d.f5);
            break;
        case VAR_TYPE(F6):
            printf("%lf", v->d.f6);
            break;
        case VAR_TYPE(CR):
            putchar(v->d.cr.a);
            putchar(v->d.cr.b);
            putchar(v->d.cr.c);
            putchar(v->d.cr.d);
            break;
         case VAR_TYPE(SG):
            printf("%s", v->d.sg);
            break;
         case VAR_TYPE(LST):
            var_lst_p(v->d.lst, sep);
            break;
    }
    if (e) var_p(e, NULL, NULL, NULL);
}

void var_f(var *v) {
    if (--v->r > 0) return;
    switch (v->t) {
        case VAR_TYPE(UN):
            var_f(v->d.v);
            break;
        case VAR_TYPE(VD):
        case VAR_TYPE(BL):
        case VAR_TYPE(I3):
        case VAR_TYPE(I4):
        case VAR_TYPE(I5):
        case VAR_TYPE(I6):
        case VAR_TYPE(U3):
        case VAR_TYPE(U4):
        case VAR_TYPE(U5):
        case VAR_TYPE(U6):
        case VAR_TYPE(F5):
        case VAR_TYPE(F6):
            break;
        // TODO cr sg
        case VAR_TYPE(LST):
            var_lst_f(v->d.lst);
            break;
    }
    free(v);
}

var_lst *var_lst_i(void) {
    return calloc(1, sizeof(var_lst));
}

void var_lst_a(var_lst *const lst, var *v) {
    var_lst_itm *itm = calloc(1, sizeof(var_lst_itm));
    itm->v = var_c(v);
    if (!lst->h) lst->h = lst->t = itm;
    else {
        itm->p = lst->t;
        lst->t = lst->t->n = itm;
    }
}

void var_lst_p(const var_lst *const lst, const var *const sep) {
    var_lst_itm *h = lst->h;
    while (h) {
        var_p(h->v, NULL, NULL, NULL);
        if (h->n) var_p(sep, NULL, NULL, NULL);
        h = h->n;
    }
}

void var_lst_f(var_lst *lst) {
    var_lst_itm *h = lst->h;
    while (h) {
        var_lst_itm *tmp = h;
        h = h->n;
        var_f(tmp->v);
        free(tmp);
    }
    free(lst);
}
