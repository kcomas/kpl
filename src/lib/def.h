
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

#define DEF_EXIT_ERROR 1

#define DEF_U64_MASK(SHIFT) ((uint64_t) 1 << (SHIFT))

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

#define DEF(_) ((def_data) { .u64 = 0 })
#define DEF_B(VALUE) ((def_data) { .b = VALUE })
#define DEF_U8(VALUE) ((def_data) { .u8 = VALUE })
#define DEF_U16(VALUE) ((def_data) { .u16 = VALUE })
#define DEF_U32(VALUE) ((def_data) { .u32 = VALUE })
#define DEF_U64(VALUE) ((def_data) { .u64 = VALUE })
#define DEF_I8(VALUE) ((def_data) { .i8 = VALUE })
#define DEF_I16(VALUE) ((def_data) { .i16 = VALUE })
#define DEF_I32(VALUE) ((def_data) { .i32 = VALUE })
#define DEF_I64(VALUE) ((def_data) { .i64 = VALUE })
#define DEF_F32(VALUE) ((def_data) { .f32 = VALUE })
#define DEF_F64(VALUE) ((def_data) { .f64 = VALUE })
#define DEF_PTR(VALUE) ((def_data) { .ptr = (void*) VALUE })

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

#define DEF_CONSTRUCTOR_MEM 110
#define DEF_CONSTRUCTOR_TASK 111
#define DEF_CONSTRUCTOR_BASE 112

#define DEF_DESTRUCTOR_MEM 110
#define DEF_DESTRUCTOR_TASK 111
#define DEF_DESTRUCTOR_BASE 112

size_t def_hash64shift(size_t key);
