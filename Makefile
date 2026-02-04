
.PHONY: clean
.RECIPEPREFIX := >

ifndef THREADS
THREADS := $(shell nproc)
endif

FLAGS += -DTHREADS=$(THREADS)

MAKEFLAGS := -j$(THREADS)

CC := gcc
OO := -g -Og
FFLAGS := -fno-omit-frame-pointer -fhardened
MFLAGS := -mincoming-stack-boundary=3
WFLAGS := -Wall -Wextra -Wstack-protector
CFLAGS = -std=gnu23 $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS)
CCOBJ = $(CC) -o $@ $^ $(CFLAGS)

SOURCES := $(wildcard ./src/**/*.c) $(wildcard ./src/**/**/*.c)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))

ifdef COLOR_OFF
FLAGS += -DCOLOR_OFF
endif

ifdef MEM_POOL_OFF
FLAGS += -DMEM_POOL_OFF
endif

kpl: OO := -O2
kpl: WFLAGS += -Werror
kpl: $(OBJECTS) ./src/main.o
> $(CCOBJ)

x64_gen_table: OO := -O2
x64_gen_table: WFLAGS += -Werror
x64_gen_table: $(OBJECTS) ./src/x64_gen_table.o
> $(CCOBJ)
> ./$@ > ./src/x64/table.c
> rm -v ./$@

TEST_SOURCES := $(wildcard ./test/*.c) $(wildcard ./test/**/*.c) $(wildcard ./test/**/**/*.c)
TEST_OBJECTS := $(patsubst %.c,%.o,$(TEST_SOURCES))

ifndef FSAN_OFF
FSAN := -fsanitize=address,leak,undefined
endif

ifdef TEST_THREAD
FSAN := -fsanitize=thread
kpl: FFLAGS += $(FSAN)
endif

ifdef TEST_MAX_COUNT
FLAGS += -DTEST_MAX_COUNT=$(TEST_MAX_COUNT)
endif

ifdef X64_ASM_DEBUG_PRINT
FLAGS += -DX64_ASM_DEBUG_PRINT
endif

ifdef X64_DIS_DEBUG_PRINT
FLAGS += -DX64_DIS_DEBUG_PRINT
endif

ifdef CORE_QUEUE_DEBUG_PRINT
FLAGS += -DCORE_QUEUE_DEBUG_PRINT
endif

tests: FFLAGS += $(FSAN)
tests: $(OBJECTS) $(TEST_OBJECTS)
> $(CCOBJ)

clean:
> find \
-not -path "./.git/*" \
-type f \
-not -regex  "^./\(LICENSE\|\|\.gitignore\|Makefile\|src/VERSION\|.*\.\(md\|c\|h\|kpl\)\)$$" \
-exec rm -v {} \;
