
#include "../test.h"

#define BUFFER_SIZE 10

TEST(ser_init) {
    ser_buffer *buffer = ser_buffer_init(BUFFER_SIZE);
    ASSERT(buffer->size == BUFFER_SIZE, "invalid buffer size");
    ser_buffer_free(buffer);
}
