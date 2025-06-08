
#pragma once

#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MOD_STAT(NAME) MOD_STAT_##NAME

typedef enum {
    MOD_STAT(OK),
    MOD_STAT(FLF) // file load fail
} mod_stat;

typedef struct {
    struct {
        struct statx info;
        char *path, *str;
    } src;
} mod;

mod_stat mod_file(mod *const m, const char *const path);
