
#pragma once

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <stdatomic.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <libgen.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "./color.h"

#define DEF_EXIT_ERROR 1

#define DEF_U64_MASK(SHIFT) ((uint64_t) 1 << (SHIFT))

#define DEF_STATIC_ARRAY_SIZE(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

#define DEF_STATUS(NAME) DEF_STATUS_##NAME

typedef enum [[gnu::packed]] {
    DEF_STATUS(OK),
    DEF_STATUS(ERROR)
} def_status;

typedef union {
    bool b;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    float f32;
    double f64;
    void *ptr;
} def_data;

inline def_data def(void) {
    return (def_data) { .ptr = nullptr };
}

inline def_data def_b(bool b) {
    return (def_data) { .b = b };
}

inline def_data def_u8(uint8_t u8) {
    return (def_data) { .u8 = u8 };
}

inline def_data def_u16(uint16_t u16) {
    return (def_data) { .u16 = u16 };
}

inline def_data def_u32(uint32_t u32) {
    return (def_data) { .u32 = u32 };
}

inline def_data def_u64(uint64_t u64) {
    return (def_data) { .u64 = u64 };
}

inline def_data def_i8(int8_t i8) {
    return (def_data) { .i8 = i8 };
}

inline def_data def_i16(int16_t i16) {
    return (def_data) { .i16 = i16 };
}

inline def_data def_i32(int32_t i32) {
    return (def_data) { .i32 = i32 };
}

inline def_data def_i64(int64_t i64) {
    return (def_data) { .i64 = i64 };
}

inline def_data def_f32(float f32) {
    return (def_data) { .f32 = f32 };
}

inline def_data def_f64(double f64) {
    return (def_data) { .f64 = f64 };
}

inline def_data def_ptr(void *ptr) {
    return (def_data) { .ptr = ptr };
}

extern def_data def_unused;

typedef size_t def_hash_fn(const def_data data);

typedef ssize_t def_cmp_fn(const def_data data_a, const def_data data_b);

typedef bool def_eq_fn(const def_data data_a, const def_data data_b);

typedef def_data def_copy_fn(const def_data data);

typedef struct _ser_buffer ser_buffer;

typedef ser_buffer *def_serialize_fn(const def_data data);

typedef void def_print_fn(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts);

typedef void def_free_fn(void *data);

typedef struct {
    def_hash_fn *hash_fn;
    def_cmp_fn *cmp_fn;
    def_eq_fn *eq_fn;
    def_copy_fn *copy_fn;
    def_serialize_fn *serialize_fn;
    def_print_fn *print_fn;
    def_free_fn *free_fn;
} def_fn_table;

extern def_fn_table def_unused_fn_table;

#define DEF_CONSTRUCTOR_MEM     110
#define DEF_CONSTRUCTOR_TASK    111
#define DEF_CONSTRUCTOR_STD     112
#define DEF_CONSTRUCTOR_IMPORT  113
#define DEF_CONSTRUCTOR_X64     114
#define DEF_CONSTRUCTOR_CORE    115

#define DEF_DESTRUCTOR_MEM      110
#define DEF_DESTRUCTOR_TASK     111
#define DEF_DESTRUCTOR_STD      112
#define DEF_DESTRUCTOR_IMPORT   113
#define DEF_DESTRUCTOR_X64      114
#define DEF_DESTRUCTOR_CORE     115

size_t def_hash64shift(size_t key);

typedef const char *def_bit_str_fn(int32_t bit_idx);

void def_mask_print(uint64_t mask, int32_t max_bit, const char *color_str, def_bit_str_fn str_fn, FILE *file);
