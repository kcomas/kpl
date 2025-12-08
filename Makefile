
.PHONY: clean
.RECIPEPREFIX := >

ifndef THREADS
THREADS := $(shell nproc)
endif

MAKEFLAGS := -j$(THREADS)

CC := gcc
OO := -g -Og
FFLAGS := -fno-omit-frame-pointer -fhardened
MFLAGS := -mincoming-stack-boundary=3
WFLAGS := -Wall -Wextra -Wstack-protector -Werror
CFLAGS = -std=gnu23 $(FLAGS) $(OO) $(FFLAGS) $(MFLAGS) $(WFLAGS) -pthread -lm
CCOBJ = $(CC) -o $@ $^ $(CFLAGS)

OBJECTS := $(patsubst %.c,%.o,$(wildcard ./src/**/*.c))

kpl: OO := -O2
kpl: $(OBJECTS) ./src/main.o
> $(CCOBJ)

TEST_OBJECTS := $(patsubst %.c,%.o,$(wildcard ./test/**/*.c))

FSAN := -fsanitize=address,leak,undefined

tests: FFLAGS += $(FSAN)
tests: $(OBJECTS) $(TEST_OBJECTS) ./test/main.o
> $(CCOBJ)

clean:
> find -not -path "./.git/*" -type f -not -regex "^./\(LICENSE\|\|\.gitignore\|Makefile\|.*\.\(md\|c\|kpl\)\)$$" | xargs rm -fv
