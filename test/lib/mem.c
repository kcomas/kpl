
#include "../test.h"

MEM_POOL(test_pool);

TEST(mem_pooling) {
    for (size_t idx = 0;  idx < 5; idx++)
        mem_free(&test_pool, mem_alloc(&test_pool, sizeof(size_t)));
#ifndef MEM_POOL_OFF
    ASSERT(test_pool.allocs == 1, "should have reused allocs");
#endif
    size_t *x = mem_alloc(&test_pool, sizeof(size_t));
    size_t *y = mem_alloc(&test_pool, sizeof(size_t));
    size_t *z = mem_alloc(&test_pool, sizeof(size_t));
    mem_free(&test_pool, x);
    mem_free(&test_pool, y);
    mem_free(&test_pool, z);
#ifndef MEM_POOL_OFF
    ASSERT(test_pool.allocs == 3, "should have reused allocs");
#endif
    x = mem_alloc(&test_pool, sizeof(size_t));
    y = mem_alloc(&test_pool, sizeof(size_t));
    mem_free(&test_pool, x);
    mem_free(&test_pool, y);
#ifndef MEM_POOL_OFF
    ASSERT(test_pool.allocs == 3, "should have reused allocs");
    ASSERT(test_pool.root, "should have root");
    ASSERT(test_pool.root->next, "pool next 1");
    ASSERT(test_pool.root->next->next, "pool next 2");
    ASSERT(!test_pool.root->next->next->next, "should be null");
#endif
}
