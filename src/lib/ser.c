
#include "./ser.h"

MEM_POOL(ser_pool);

constexpr uint32_t buffer_max_size = UINT32_MAX - sizeof(ser_buffer);

ser_buffer *ser_buffer_init(uint32_t size) {
    if (size > buffer_max_size)
        return nullptr;
    ser_buffer *buffer = mem_alloc(&ser_pool, sizeof(ser_buffer) + sizeof(uint8_t) * size);
    buffer->size = size;
    return buffer;
}

void ser_buffer_free(ser_buffer *buffer) {
    mem_free(&ser_pool, buffer);
}
