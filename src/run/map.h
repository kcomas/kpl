
#pragma once

#include "./header.h"
#include "./error.h"

typedef struct _map_bucket {
    MEM_HEADER(_map_bucket);
    def_data data;
} map_bucket;

typedef struct _map {
    RUN_HEADER(_map);
    size_t size, used;
    def_hash_fn *hash_fn;
    def_eq_fn *eq_fn;
    map_bucket *head, *tail;
    map_bucket *buckets[];
} map;

#ifndef MAP_MIN_SIZE
#define MAP_MIN_SIZE 10
#endif

map *map_init(size_t size, def_hash_fn hash_fn, def_eq_fn eq_fn, def_print_fn print_fn, uint32_t print_opts,
    def_free_fn free_fn);

void map_free(map *ma);

#ifndef MAP_MAX_SEARCH_PERCENT
#define MAP_MAX_SEARCH_PERCENT 0.20
#endif

#ifndef MAP_RESIZE_MUL
#define MAP_RESIZE_MUL 10
#endif

#define MAP_MODE(NAME) MAP_MODE_##NAME

typedef enum [[gnu::packed]] {
    MAP_MODE(INSERT),
    MAP_MODE(UPSERT),
    MAP_MODE(FIND),
    MAP_MODE(REMOVE)
} map_mode;

def_status map_action(map **ma, map_mode mode, def_data search, def_data *found);

#define MAP_PRINT(NAME) MAP_PRINT_##NAME

typedef enum [[gnu::packed]] {
    MAP_PRINT(NL_END)       = 1 << 0,
} map_print_opts;

void map_print(const map *ma, FILE *file, size_t idnt, map_print_opts opts);
