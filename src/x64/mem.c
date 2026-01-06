
#include "./mem.h"

uint8_t *x64_mem = NULL;

static atomic_size_t x64_mem_len = 0;

void x64_mem_reset(void) {
    x64_mem_len = 0;
}

[[gnu::constructor(DEF_CONSTRUCTOR_X64)]] static void x64_mem_constructor(void) {
    x64_mem = mmap(NULL, X64_MEM_SIZE, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

[[gnu::destructor(DEF_DESTRUCTOR_X64)]] static void x64_mem_destructor(void) {
    munmap(x64_mem, X64_MEM_SIZE);
}

void x64_mem_unlock(size_t start_idx) {
    mprotect(x64_mem + start_idx, getpagesize(), PROT_WRITE);
}

void x64_mem_lock(size_t start_idx) {
    mprotect(x64_mem + start_idx, getpagesize(), PROT_READ | PROT_EXEC);
}

size_t x64_mem_get_page_start(void) {
    return atomic_fetch_add(&x64_mem_len, getpagesize());
}

def_status x64_mem_write_byte(size_t *pos, uint8_t byte) {
    if (*pos != 0 && *pos % getpagesize() == 0)
        return DEF_STATUS(ERROR);
    x64_mem[(*pos)++] = byte;
    return DEF_STATUS(OK);
}
