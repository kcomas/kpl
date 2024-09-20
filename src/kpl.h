
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include "al.h"
#include "lst.h"

#define KPLE 'k'+'p'+'l'

#define TYPE(N) TYPE_##N

typedef enum {
    // internal
    TYPE(STMT),
    TYPE(ARGS),
    TYPE(OP),
    TYPE(IF),
    TYPE(COND),
    TYPE(LOP),
    TYPE(VAR),
    TYPE(CODE),
    // ast types
    TYPE(MOD), // file
    TYPE(INT),
    TYPE(FLT),
    TYPE(STR),
    // data types
    TYPE(VD),
    TYPE(BL),
    TYPE(U3),
    TYPE(U4),
    TYPE(U5),
    TYPE(U6),
    TYPE(I3),
    TYPE(I4),
    TYPE(I5),
    TYPE(I6),
    TYPE(F5),
    TYPE(F6),
    TYPE(DT),
    TYPE(CR),
    TYPE(SL),
    TYPE(SG),
    TYPE(VR),
    TYPE(TE),
    TYPE(HH),
    TYPE(ST),
    TYPE(FN),
    TYPE(ER),
    TYPE(FD)
} type;

const char *type_get_str(type t);

typedef struct _er er;

typedef struct _ast ast;

typedef struct _fn_node fn_node;

typedef struct _code code;

typedef void jit_fn(void);

typedef struct _jit jit;

typedef union _var var;

// free not null w fn
#define FNNF(V, F) if (V) F(V)

// free not null
#define FNN(V) FNNF(V, free)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))

// print char x times
#define PCX(C, X) for (size_t i = 0; i < X; i++) putchar(C)

typedef struct {
    uint8_t ng; // number of globals
    struct {
        struct stat sb;
        char *path, *str;
    } src;
    al *a; // allocator
    er *e; // error
    fn_node *fns; // ast root
    code *c;
    jit *j;
    var *g; // globals
} mod;

#define RC ssize_t rc

typedef struct {
    RC;
    size_t len, size; // len does not inc null term
    char str[]; // null term
} var_sg;

void var_sg_f(var_sg *sg);
