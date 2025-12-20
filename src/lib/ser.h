
#pragma once

#include "./mem.h"

typedef struct _ser_buffer {
    MEM_HEADER(_ser_buffer);
    uint32_t size;
    uint8_t data[];
} ser_buffer;

ser_buffer *ser_buffer_init(uint32_t size);

void ser_buffer_free(ser_buffer *buffer);
