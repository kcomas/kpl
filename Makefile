
.RECIPEPREFIX = >
CC = gcc
CFLAGS = -std=c17 -g -Wall -Wextra
SRC = ./src
TEST = ./test
SRCS = $(wildcard $(SRC)/*.c)
TSRCS = $(wildcard $(TEST)/*.c)
CCOBJ = $(CC) -o $@ $^
OBJS = $(patsubst %.c, %.o, $(SOURCES))
NAME = kpl
TNAME = _test

MOD_OBJS = $(SRC)/mod.o
mod$(TNAME): $(MOD_OBJS) $(TEST)/mod.o
> $(CCOBJ)

%.o: %.c
> $(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
> rm -f $(SRC)/*.o $(TEST)/*.o $(NAME) *$(TNAME)
