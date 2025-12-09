
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

OBJECTS := $(patsubst %.c,%.o,$(wildcard ./src/**/*.c))

kpl: OO := -O2
kpl: WFLAGS += -Werror
kpl: $(OBJECTS) ./src/main.o
> $(CCOBJ)

TEST_OBJECTS := $(patsubst %.c,%.o,$(wildcard ./test/**/*.c))

FSAN := -fsanitize=address,leak,undefined

ifdef TEST_THREAD
FSAN := -fsanitize=thread
endif

tests: FFLAGS += $(FSAN)
tests: WFLAGS += -Werror
tests: $(OBJECTS) $(TEST_OBJECTS) ./test/test.o
> $(CCOBJ)

clean:
> find \
-not -path "./.git/*" \
-type f \
-not -regex  "^./\(LICENSE\|\|\.gitignore\|Makefile\|.*\.\(md\|c\|kpl\)\)$$" \
-exec rm -v {} \;
