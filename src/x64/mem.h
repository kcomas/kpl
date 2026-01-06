
#pragma once

#include "./table.h"

extern uint8_t *x64_mem;

#ifndef X64_MEM_PAGES
#define X64_MEM_PAGES ((1024 * 1000) / getpagesize())
#endif

#define X64_MEM_SIZE (X64_MEM_PAGES * getpagesize())

void x64_mem_reset(void);

void x64_mem_unlock(size_t start_idx);

void x64_mem_lock(size_t start_idx);

size_t x64_mem_get_page_start(void);

def_status x64_mem_write_byte(size_t *pos, uint8_t byte);
