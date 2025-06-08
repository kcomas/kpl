
#pragma once

#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/mman.h>
#include <sched.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
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
    TYPE(REPL),
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
    TYPE(FD),
    TYPE(TD)
} type;

const char *type_get_str(type t);

typedef struct _er er;

typedef struct _ast ast;

typedef struct _fn_node fn_node;

typedef struct _type_node type_node;

typedef struct _code code;

typedef int jit_fn(void*);

typedef struct _jit {
    uint8_t flgs;
    size_t len, size; // len, num pages, total size
    uint8_t *h; // address
} jit;

void jit_f(jit *j);

typedef union _var var;

typedef struct _var_td var_td;

#define THREAD_STACK_SIZE_PAGE_MUL 4

typedef struct _tdr {
    struct _tdr *prev, *next;
    size_t stks;
    al *a;
    er *e;
    jit *j;
    void *stk;
} tdr; // thread resource

typedef struct _tds {
    size_t size, len, total; // len is number in lst, total is number used
    tdr *h, *t;
} tds; // threads

typedef struct {
    uint8_t ng; // number of globals
    pid_t id;
    struct {
        struct statx sxb;
        char *path, *str;
    } src;
    tds *s;
    tdr *r; // thread resource
    fn_node *fns; // ast root
    code *c;
    var *g; // globals
    type_node *tn;
} mod;

void mod_f(mod *m);

#define RC ssize_t rc

typedef struct {
    RC;
    size_t len, size; // len does not inc null term
    char *str; // null term
} var_sg;

void var_sg_f(var_sg *sg);

typedef struct _var_tsv var_tsv;

typedef struct _var_hh var_hh;

// free not null w fn
#define FNNF(V, F) if (V) F(V)

// free not null
#define FNN(V) FNNF(V, free)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))

// print char x times
#define PCX(C, X) for (size_t i = 0; i < X; i++) putchar(C)

// return length of str to last /
inline size_t str_dir_len(const char *const s) {
    if (!s) return 0;
    size_t len = strlen(s);
    while (len > 1) {
        if (s[len - 1] == '/') return len;
        len--;
    }
    return 0;
}

#ifndef TSVML
    #define TSVML 4 // vr mul
#endif

#ifndef TSV_DEF_SIZE
    #define TSV_INIT_SIZE 1
#endif
