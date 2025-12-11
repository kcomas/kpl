
#include "./mem.h"

#define MEM_MEM(NAME, TYPE) void *mem_mem##NAME(void *dest, TYPE src, size_t size) { \
    size_t obj_size = ((mem_obj*) dest)->obj_size; \
    mem##NAME(dest, src, size); \
    ((mem_obj*) dest)->obj_size = obj_size; \
    return dest; \
}

MEM_MEM(cpy, const void *)
MEM_MEM(set, uint8_t)

#ifndef MEM_POOL_OFF
void mem_pool_init(mem_pool *pool) {
    mtx_init(&pool->mutex, mtx_plain);
}

void mem_pool_free(mem_pool *pool) {
    mem_obj *head = pool->root, *tmp;
    while (head) {
        tmp = head;
        head = head->next;
        free(tmp);
    }
    mtx_destroy(&pool->mutex);
}

void *mem_alloc(mem_pool *pool, size_t new_size) {
    mem_obj *head;
    mtx_lock(&pool->mutex);
    for (head = pool->root; head; head = head->next) {
        if (head->obj_size < new_size)
            continue;
        if (head->prev)
            head->prev->next = head->next;
        if (head->next)
            head->next->prev = head->prev;
        if (head == pool->root) {
            pool->root = pool->root->next;
            if (pool->root) {
                pool->root->prev = NULL;
                if (pool->root->next)
                    pool->root->next->prev = pool->root;
            }
        }
        break;
    }
    mtx_unlock(&pool->mutex);
    if (!head) {
        if (new_size < sizeof(mem_obj))
            new_size += sizeof(mem_obj);
        head = malloc(new_size);
        head->obj_size = new_size;
        pool->allocs++;
    }
    head->prev = head->next = NULL;
    return head;
}

void mem_free(mem_pool *pool, void *data) {
    mtx_lock(&pool->mutex);
    mem_obj *obj = data;
    obj->prev = obj->next = NULL;
    if (pool->root) {
        obj->next = pool->root;
        pool->root->prev = obj;
    }
    pool->root = obj;
    mtx_unlock(&pool->mutex);
}
#else
void *mem_alloc(mem_pool *pool, size_t new_size) {
    (void) pool;
    return malloc(new_size);
}

void mem_free(mem_pool *pool, void *data) {
    (void) pool
    free(data);
}
#endif
