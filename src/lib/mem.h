
#pragma once

#include "./def.h"

#define MEM_HEADER(STRUCT_NAME) struct STRUCT_NAME *prev, *next; uint32_t obj_size;

typedef struct _mem_obj {
    MEM_HEADER(_mem_obj);
} mem_obj;

typedef struct {
    atomic_size_t allocs;
    mem_obj *root;
    pthread_mutex_t mutex;
} mem_pool;

void *mem_memcpy(void *dest, const void *src, size_t size);

void *mem_memset(void *dest, uint8_t value, size_t size);

void mem_list_remove(void *header);

#ifndef MEM_POOL_OFF
#define MEM_POOL(POOL_NAME) mem_pool POOL_NAME = (mem_pool) {}; \
[[gnu::constructor(DEF_CONSTRUCTOR_MEM)]] static void POOL_NAME##_constructor(void) { \
    mem_pool_init(&POOL_NAME); \
} \
[[gnu::destructor(DEF_DESTRUCTOR_MEM)]] static void POOL_NAME##_destructor(void) { \
    mem_pool_free(&POOL_NAME); \
}

void mem_pool_init(mem_pool *pool);

void mem_pool_free(mem_pool *pool);
#else
#define MEM_POOL(POOL_NAME) mem_pool POOL_NAME = (mem_pool) {};
#endif

void *mem_alloc(mem_pool *pool, size_t new_size);

void mem_free(mem_pool *pool, void *data);
