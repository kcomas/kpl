
#pragma once

#include "./table.h"

extern uint8_t *x64_mem;

#ifndef X64_MEM_PAGE_MUL
#define X64_MEM_PAGE_MUL 1
#endif

inline int32_t x64_mem_file_byte_size() {
    return getpagesize() * X64_MEM_PAGE_MUL;
}

#ifndef X64_MEM_PAGES
#define X64_MEM_PAGES 100
#endif

void x64_mem_reset(void);

void x64_mem_unlock(size_t start_idx);

void x64_mem_lock(size_t start_idx);

int32_t x64_mem_get_page_start(void);

def_status x64_mem_write_byte(int32_t *byte_pos, uint8_t byte);
