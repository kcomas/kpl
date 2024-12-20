
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "lst.h"

#ifndef DEFALGN
    #define DEFALGN 16
#endif

inline size_t algn(size_t size, size_t an) {
    size_t mo = size % an;
    if (mo) size = size - mo + an;
    return size;
}

typedef struct _alc alc;

typedef struct {
    size_t len, size, u, f; // used freed
    alc *h, *t;
} al;

inline al *al_i(void *nal) { // zero for tests
    al *a = (al*) nal;
    a->len = a->size = a->u = a->f = 0;
    a->h = a->t = NULL;
    return a;
}

#ifndef ALC_USED_FREE_PCT
    #define ALC_USED_FREE_PCT 0.90
#endif

typedef struct _alc {
    al *a;
    alc *prev, *next;
    size_t aus, len, size; // active used size, total used, size of chunk
} alc; // allocator chunk no frees filled then freed

typedef struct {
    size_t size;
    alc *ac;
} alci;

#ifndef AL_PS_MUL
    #define AL_PS_MUL 1
#endif

inline alc *alc_i(size_t size) {
    size = algn(size + sizeof(alc), DEFALGN);
    size_t ps = (size_t) getpagesize() * AL_PS_MUL;
    size = size <= ps ? ps : (size / ps + 1) * ps;
    alc *ac = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    ac->size = size;
    ac->len = algn(sizeof(alc), DEFALGN);
    return ac;
}

void alc_f(alc *ac, void *fn);

inline void al_f(al *a) {
#ifdef KPL_ALD
    printf("==Used: %lu, Freed: %lu, Total: %lub==\n", a->u, a->f, a->size);
#endif
    LST_F(a, alc, alc_f, NULL);
}

void *ala(al *const a, size_t size);

void alf(void *ptr);
