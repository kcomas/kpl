
#include "../test.h"

#ifndef MEM_POOL_OFF
mem_pool(test_pool);

TEST(mem_pooling) {
    for (size_t idx = 0;  idx < 5; idx++)
        mem_free(&test_pool, mem_alloc(&test_pool, sizeof(size_t)));
    ASSERT(test_pool.allocs == 1, "should have reused allocs");
    size_t *x = mem_alloc(&test_pool, sizeof(size_t));
    size_t *y = mem_alloc(&test_pool, sizeof(size_t));
    size_t *z = mem_alloc(&test_pool, sizeof(size_t));
    mem_free(&test_pool, x);
    mem_free(&test_pool, y);
    mem_free(&test_pool, z);
    ASSERT(test_pool.allocs == 3, "should have reused allocs");
    x = mem_alloc(&test_pool, sizeof(size_t));
    y = mem_alloc(&test_pool, sizeof(size_t));
    mem_free(&test_pool, x);
    mem_free(&test_pool, y);
    ASSERT(test_pool.allocs == 3, "should have reused allocs");
}
#endif
