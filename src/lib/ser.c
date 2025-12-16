
#include "./ser.h"

MEM_POOL(ser_pool);

ser_buffer *ser_buffer_init(size_t size) {
    ser_buffer *buffer = mem_alloc(&ser_pool, sizeof(ser_buffer) + sizeof(uint8_t) * size);
    buffer->size = size;
    return buffer;
}

void ser_buffer_free(ser_buffer *buffer) {
    mem_free(&ser_pool, buffer);
}
