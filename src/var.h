
#pragma once

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
   Higher Level Coding in C
   Use pre built rc'd types for the engine and the language itself
   Every var is heap allocated and ref counted
*/

#define VAR_TYPE(N) VAR_TYPE_##N

typedef enum {
    VAR_TYPE(UN), // unknown
    VAR_TYPE(VD),
    VAR_TYPE(BL),
    VAR_TYPE(I3),
    VAR_TYPE(I4),
    VAR_TYPE(I5),
    VAR_TYPE(I6),
    VAR_TYPE(U3),
    VAR_TYPE(U4),
    VAR_TYPE(U5),
    VAR_TYPE(U6),
    VAR_TYPE(F5),
    VAR_TYPE(F6),
    VAR_TYPE(CR), // utf8
    VAR_TYPE(SG),
    VAR_TYPE(LST), // linked list
    VAR_TYPE(VR), // vector
    VAR_TYPE(HH), // hash map
    VAR_TYPE(FN),
    VAR_TYPE(_END)
} var_type;

typedef struct _var var;

typedef struct _var_lst var_lst;

typedef struct _var_vr var_vr;

typedef struct _var_hh var_hh;

typedef struct _var_fn var_fn;

typedef struct {
    char a, b, c, d;
} c4;

#define C4(A, B, C, D) ((c4) { .a = A, .b = B, .c = C, .d = D })
#define C1(A) C4(A, '\0', '\0', '\0')
#define C2(A, B) C4(A, B, '\0', '\0')
#define C3(A, B, C) C4(A, B, C, '\0')

typedef struct {
    var *v;
    void *vd;
    bool bl;
    int8_t i3;
    int16_t i4;
    int32_t i5;
    int64_t i6;
    uint8_t u3;
    uint16_t u4;
    uint32_t u5;
    uint64_t u6;
    float f5;
    double f6;
    c4 cr;
    char *sg;
    var_lst *lst;
    var_vr *vr;
    var_hh *hh;
    var_fn *fn;
} var_data;

#define VAR_DATA(D, V) ((var_data) { .D = V })

typedef uint32_t RC;

typedef struct _var {
    var_type t;
    RC r;
    var_data d;
} var;

var *var_i(var_type t, var_data d);

#define VAR_I(T, D, V) var_i(VAR_TYPE(T), VAR_DATA(D, V))

var *var_c(var *v);

void var_p(const var *const v, const var *const s, const var *const e, const var *const sep);

void var_f(var *v);

typedef struct _var_lst_itm {
    struct _var_lst_itm *p, *n;
    var *v;
} var_lst_itm;

typedef struct _var_lst {
    size_t len;
    var_lst_itm *h, *t;
} var_lst;

var_lst *var_lst_i(void);

void var_lst_a(var_lst *const lst, var *v);

void var_lst_p(const var_lst *const lst, const var *const sep);

void var_lst_f(var_lst *v);

typedef struct _var_vr {
    size_t len, size;
    var *v[];
} var_vr;

typedef struct _var_hh {
    size_t used, size;
    var_lst *lst;
    var *b[];
} var_hh;

typedef struct _var_fn {
    uint8_t na; // num args needed for calling
    size_t len, size; // num args, len for jit, mapped size
    void *mm; // mmap ptr to jited code
} var_fn;
