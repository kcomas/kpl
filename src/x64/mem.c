
#include "./mem.h"

uint8_t *x64_mem = nullptr;

static atomic_int_fast32_t x64_mem_len = 0;

extern inline int32_t x64_mem_file_byte_size();

#define X64_MEM_SIZE (X64_MEM_FILES * x64_mem_file_byte_size())

void x64_mem_reset(void) {
    x64_mem_len = 0;
}

[[gnu::constructor(DEF_CONSTRUCTOR_X64)]] static void x64_mem_constructor(void) {
    x64_mem = mmap(nullptr, X64_MEM_SIZE, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

[[gnu::destructor(DEF_DESTRUCTOR_X64)]] static void x64_mem_destructor(void) {
    munmap(x64_mem, X64_MEM_SIZE);
}

void x64_mem_unlock(size_t start_idx) {
    mprotect(x64_mem + start_idx, x64_mem_file_byte_size(), PROT_WRITE);
}

void x64_mem_lock(size_t start_idx) {
    mprotect(x64_mem + start_idx, x64_mem_file_byte_size(), PROT_READ | PROT_EXEC);
}

int32_t x64_mem_get_page_start(void) {
    if (x64_mem_len == X64_MEM_SIZE)
        return -1;
    return atomic_fetch_add(&x64_mem_len, x64_mem_file_byte_size());
}

def_status x64_mem_write_byte(int32_t *byte_pos, uint8_t byte) {
    if (*byte_pos != 0 && *byte_pos % x64_mem_file_byte_size() == 0)
        return DEF_STATUS(ERROR);
    x64_mem[(*byte_pos)++] = byte;
    return DEF_STATUS(OK);
}
