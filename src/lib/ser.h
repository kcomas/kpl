
#pragma once

#include "./mem.h"

typedef struct _ser_buffer {
    MEM_HEADER(_ser_buffer);
    size_t size;
    uint8_t data[];
} ser_buffer;

ser_buffer *ser_buffer_init(size_t size);

void ser_buffer_free(ser_buffer *buffer);
