
#pragma once

#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/stat.h>
#include "./atg_x64.h"

err *z(mc *fn, tbl **et); // file name, exports tbl
